#include <iomanip>
#include <util.h>

#include "vcstuff.h"
#include "ansiColor.h"
#include "command.h"
#include "console.h"

using namespace stratego::view;
using namespace stratego::model;
using namespace stratego;

View::View(Model* model, Controller& controller) :
    model_ {model},
    controller_ {controller}
{}

void View::update(std::initializer_list<Observable*> args){
    StateGraph::State currentState {model_ -> currentState()};
    std::string color {};
    AnsiColor red {AnsiColor::RED};
    AnsiColor green {AnsiColor::GREEN};
    AnsiColor blue {AnsiColor::BLUE};
    AnsiColor yellow {AnsiColor::YELLOW};
    AnsiColor stop {AnsiColor::RESET};

    static bool hasDisplayBoard {};
    static std::array<Color, 2> setup_f {Color::RED, Color::BLUE};
    static int setup_p {-1};

    switch(currentState){
        case StateGraph::NOT_STARTED:
            setup_p = -1;
            Console::clear();
            displayIntro();
            controller_.init();
            break;
        case StateGraph::SET_UP:
            if(setup_p == setup_f.size() - 1){
                controller_.load();
            } else{
                setup_p++;
                controller_.load(setup_f[setup_p]);
            }
            break;
        case StateGraph::ERROR_SETUP:
            std::cout << "[" << red << "FAILURE" << stop << "] "
              << model_ -> history().lastFailure() << std::endl;
            setup_p--;
            controller_.errorProcessed();
            break;
        case StateGraph::PLAYER_TURN:
            if(!hasDisplayBoard){
                displayBoard();
                color = model_ -> currentPlayer().color() == Color::RED ? "rouge" : "bleu";
                std::cout << "[" << (color == "rouge" ? red : blue) << "Joueur " << color
                          << stop << "]"
                          << " Au tour de "
                          << model_ -> currentPlayer().pseudo()
                          << std::endl;
                displayEatenPieces();
                hasDisplayBoard = true;
            }

            controller_.processAction();
            break;
        case StateGraph::ERROR_ACTION:
            std::cout << "[" << red << "FAILURE" << stop << "] "
                      << model_ -> history().lastFailure() << std::endl;
            std::cout << "[" << yellow << "HINT" << stop << "] "
                      << model_ -> history().lastHint() << std::endl;
            controller_.errorProcessed();
            break;
        case StateGraph::GAME_TURN:
            std::cout << "[" << green << "SUCCESS" << stop << "] "
                      << model_ -> history().lastSuccess() << std::endl;
            controller_.nextTurn();
            break;
        case StateGraph::PLAYER_SWAP:
            hasDisplayBoard = false;
            std::cout << AnsiColor{AnsiColor::BLACK} << AnsiColor::colorText("Appuyez sur [enter] pour continuer", 15, true) << AnsiColor{AnsiColor::RESET};
            controller_.nextPlayer();
            break;
        case StateGraph::GAME_OVER:
            Console::clear();
            displayBoard();
            std::cout << "[" << blue << "PROMPT" << stop << "] "
                      << model_ -> history().lastSuccess() << std::endl;
            controller_.replay();
            break;
        case StateGraph::EOG:
            std::cout << "[" << blue << "PROMPT" << stop << "] "
                      << model_ -> history().lastSuccess() << std::endl;
            break;
        default: throw std::logic_error("Unknown state");
    }
}

void View::processAction(const std::string& input) noexcept{
    Command* cmd {};
    ActionMatcher actionMatcher{Action{input}};
    std::pair<Position, Position> positions {};
    Action action {actionMatcher.action()};
    int rank;

    actionMatcher.match(input);
    switch(action){
        case Action::MOVE:
            positions = processMoveAttackCommand(actionMatcher);
            cmd = new MoveCommand{controller_, positions.first, positions.second};
            break;
        case Action::ATTACK:
            positions = processMoveAttackCommand(actionMatcher);
            cmd = new AttackCommand{controller_, positions.first, positions.second};
            break;
        case Action::HELP:
            if(!actionMatcher.matcher()[1].str().empty()){
                action = Action{actionMatcher.matcher()[1]};
                cmd = new HelpCommand{&action};
            } else{
                cmd = new HelpCommand{};
            }
            break;
        case Action::RULES:
            cmd = new RulesCommand{std::string{Config::RESOURCES_PATH} + "rules"};
            break;
        case Action::PIECE:
            rank = processPieceStatCommand(actionMatcher);
            cmd = new PieceCommand{rank};
            break;
        case Action::STOP:
            cmd = new StopCommand{controller_};
            break;
        case Action::STAT:
            rank = processPieceStatCommand(actionMatcher);
            cmd = new StatCommand{model_ -> currentPlayer(), rank};
            break;
        case Action::HISTORY:
            cmd = new HistoryCommand{model_ -> history()};
            break;
        case Action::CLEAR:
            cmd = new ClearCommand{*this, model_ -> currentPlayer().color(), model_ -> currentPlayer().pseudo()};
    }

    cmd -> exec();
    delete cmd;
}

std::pair<Position, Position> View::processMoveAttackCommand(ActionMatcher& actionMatcher){
    std::smatch matcher {actionMatcher.matcher()};
    std::string posId {matcher[1]};
    std::string way {matcher[2]};
    Direction direction {Direction::from(way)};

    Position startPos {Position::from(posId)};
    Position endPos {startPos + direction};
    if(!matcher[3].str().empty()){
        int count {std::stoi(matcher[3])};
        switch(direction){
            case Direction::UP:
                endPos = startPos + Position{0, -count};
                break;
            case Direction::DOWN:
                endPos = startPos + Position{0, count};
                break;
            case Direction::LEFT:
                endPos = startPos + Position{-count, 0};
                break;
            case Direction::RIGHT:
                endPos = startPos + Position{count, 0};
                break;
        }
    }

    return {startPos, endPos};
}

int View::processPieceStatCommand(view::ActionMatcher &actionMatcher){
    std::smatch matcher {actionMatcher.matcher()};
    int result = -1;
    if(!matcher[1].str().empty()){
        result = Piece::toRank(matcher[1].str());
    }

    return result;
}

void View::displayEatenPieces() const noexcept{
    AnsiColor player_color {model_ -> currentPlayer().color() == Color::RED ? AnsiColor::RED : AnsiColor::BLUE};
    AnsiColor stop {AnsiColor::RESET};

    std::cout << "Pièces mangés de l'ennemi: ";
    for(int i = Config::PIECE_MIN_RANK; i <= Config::PIECE_MAX_RANK; i++){
        std::cout << AnsiColor::colorText(std::string{Piece::pieceInfo[i].symbol}, AnsiColor::BOLD)
                  << "(" << player_color << model_ -> currentPlayer().eatenPieces(i) << stop << ") ";
    }

    std::cout << std::endl << "Total: " << model_ -> currentPlayer().eatenPieces() << std::endl;
}

void View::displayIntro() const noexcept{
    AnsiColor red {AnsiColor::RED};
    AnsiColor blue {AnsiColor::BLUE};
    AnsiColor stop {AnsiColor::RESET};
    std::cout << red << "Stra" << blue << "tego" << stop << std::endl << std::endl;

    std::cout << "Le jeux de Stratego est un jeux de plateau se jouant à 2 joueurs où le but est de" << std::endl
              << "capturer le drapeau du camp ennemi ou d'empêcher le joueur adverse de se déplacer." << std::endl << std::endl;

    std::cout << "Diverses commandes sont disponibles aux joueurs pour intéragir avec le jeux." << std::endl << std::endl;
    std::cout << "Une fois la partie lancée, la commande HELP vous permettra d'obtenir un aperçu" << std::endl
              << "de toutes les commandes disponibles. Cette commande vous permettra également d'obtenir" << std::endl
              << "plus d'informations sur l'utilisation d'une commande en particulier (Pour la commande" << std::endl
              << "MOVE par exemple: HELP MOVE)." << std::endl << std::endl;

    std::cout << AnsiColor::colorText("Syntaxe générale", AnsiColor::BOLD) << ": CMD <params...>" << std::endl;
    std::cout << "À noter que l'ensemble des commandes sont insensibles à la casse." << std::endl << std::endl;

    std::cout << AnsiColor{AnsiColor::BLACK} << AnsiColor::colorText("Appuyez sur [enter] pour commencer", 15, true) << AnsiColor{AnsiColor::RESET};
}

void View::displayBoard() const noexcept{
    const Board& board {model_ -> board()};
    int cellSize {util::cdigit(stratego::Config::PIECE_MAX_RANK) + util::cdigit(stratego::Config::BOARD_SIZE - 2) + 2};
    int rowIndexSize {util::cdigit(stratego::Config::BOARD_SIZE - 2)};
    int sepSize {3};

    displayBoardHead(rowIndexSize + sepSize + 2, cellSize, sepSize);
    displayBoardOutline(rowIndexSize + 1, cellSize, sepSize);
    for(int i = 1; i < board.size() - 1; i++){
        displayEmptyRow(rowIndexSize + 1, sepSize * (stratego::Config::BOARD_SIZE - 1) + (stratego::Config::BOARD_SIZE - 2) * cellSize);
        std::cout << AnsiColor{AnsiColor::BOLD} << std::setw(rowIndexSize) << i << AnsiColor{AnsiColor::RESET} << " " << stratego::Config::SYMBOL_WALL;

        for(int j = 1; j < board.size() - 1; j++){
            const Cell& cell {board.getCell(j, i)};
            std::cout << util::strrepeat(" ", sepSize);
            switch(cell.type){
                case Cell::NORMAL: displayNormalCell(cell, cellSize);
                    break;
                case Cell::WALL: displayWallCell(cellSize);
                    break;
                case Cell::WATER: displayWaterCell(cellSize);
            }
        }

        std::cout << util::strrepeat(" ", sepSize) << stratego::Config::SYMBOL_WALL << " " << i << std::endl;
    }

    displayEmptyRow(rowIndexSize + 1, sepSize * (stratego::Config::BOARD_SIZE - 1) + (stratego::Config::BOARD_SIZE - 2) * cellSize);
    displayBoardOutline(rowIndexSize + 1, cellSize, sepSize);
    displayBoardHead(rowIndexSize + sepSize + 2, cellSize, sepSize);
}

void View::displayBoardHead(int leftoffs, int cellwidth, int sepwidth) const{
    std::cout << util::strrepeat(" ", leftoffs);
    char bound {'A' + stratego::Config::BOARD_SIZE - 2};
    for(char i = 'A'; i < bound; i++){
        std::cout << AnsiColor{AnsiColor::BOLD}
                  << std::setw(cellwidth)
                  << std::left
                  << i
                  << AnsiColor{AnsiColor::RESET}
                  << util::strrepeat(" ", sepwidth);
    }

    std::cout << util::strrepeat(" ", sepwidth) << std::endl;
}

void View::displayBoardOutline(int leftoffs, int cellwidth, int sepwidth) const{
    std::cout << util::strrepeat(" ", leftoffs) << stratego::Config::SYMBOL_WALL;
    int bound {stratego::Config::BOARD_SIZE - 2};
    for(int i = 0; i < bound; i++){
        std::cout << util::strrepeat(std::string{stratego::Config::SYMBOL_HWSPACE}, sepwidth)
                  << util::strrepeat(std::string{stratego::Config::SYMBOL_WALL}, cellwidth);
    }

    std::cout << util::strrepeat(std::string{stratego::Config::SYMBOL_HWSPACE}, sepwidth)
              << stratego::Config::SYMBOL_WALL
              << std::endl;
}

void View::displayEmptyRow(int leftoffs, int width) const{
    std::cout << util::strrepeat(" ", leftoffs)
              << stratego::Config::SYMBOL_VWSPACE
              << util::strrepeat(" ", width)
              << stratego::Config::SYMBOL_VWSPACE
              << std::endl;
}

void View::displayNormalCell(const stratego::model::Cell &cell, int width) const{
    AnsiColor red {AnsiColor::RED};
    AnsiColor blue {AnsiColor::BLUE};
    AnsiColor stop {AnsiColor::RESET};
    if(cell.piece){
        std::cout << (cell.piece -> color() == Color::RED ? red : blue)
                  << std::setw(width)
                  << (cell.piece -> revealed() ? std::string{*cell.piece} : stratego::Config::SYMBOL_HIDDEN_PIECE)
                  << stop;
    } else{
        std::cout << std::setw(width) << stratego::Config::SYMBOL_EMPTY;
    }
}

void View::displayWallCell(int width) const{
    std::cout << util::strrepeat(std::string{stratego::Config::SYMBOL_WALL}, width);
}

void View::displayWaterCell(int width) const{
    std::cout << util::strrepeat(std::string{stratego::Config::SYMBOL_WATER}, width);
}

