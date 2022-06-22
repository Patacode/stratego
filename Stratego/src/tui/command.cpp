#include <util.h>

#include "ansiColor.h"
#include "command.h"
#include "console.h"

using namespace stratego::view;
using namespace stratego::model;

MoveCommand::MoveCommand(Controller& controller, const Position& startPos, const Position& endPos) :
    controller_ {controller},
    startPos_ {startPos},
    endPos_ {endPos}
{}

void MoveCommand::exec() noexcept{
    controller_.move(startPos_, endPos_);
}

void MoveCommand::undo() noexcept{}

AttackCommand::AttackCommand(Controller& controller, const Position& startPos, const Position& endPos) :
    controller_ {controller},
    startPos_ {startPos},
    endPos_ {endPos}
{}

void AttackCommand::exec() noexcept{
    controller_.attack(startPos_, endPos_);
}

void AttackCommand::undo() noexcept{}

HelpCommand::HelpCommand(Action* action)
    : action_ {action}
{}

void HelpCommand::exec() noexcept{
    AnsiColor ansiColor {AnsiColor::BOLD};
    if(action_){
        std::cout << "\t"
                  << ansiColor.colorize("Description")
                  << ": "
                  << action_ -> description()
                  << std::endl;
        std::cout << "\t"
                  << ansiColor.colorize("Syntaxe")
                  << ": "
                  << action_ -> syntax()
                  << std::endl;
    } else{
        for(int i = 0; i <= Action::CLEAR; i++){
            Action action {static_cast<Action::Value>(i)};
            std::string actionName {Action::valueNames[i]};
            util::strtoupper(actionName);
            std::cout << "\t"
                      << ansiColor.colorize(actionName)
                      << " = "
                      << action.description()
                      << std::endl;
        }
    }
}

void HelpCommand::undo() noexcept{}


RulesCommand::RulesCommand(const std::string& filename) :
    filename_ {filename}
{}

void RulesCommand::exec() noexcept{
    std::ifstream ifs {filename_};
    std::string line {};
    if(ifs.good()){
        while(std::getline(ifs, line)){
            std::cout << line << std::endl;
        }
    } else{
        std::cout << "Aucune règle trouvée." << std::endl;
    }

    ifs.close();
}

void RulesCommand::undo() noexcept{}


PieceCommand::PieceCommand(int rank) :
    rank_ {rank}
{}

void PieceCommand::exec() noexcept{
    if(rank_ == - 1){
        for(int i = Config::PIECE_MIN_RANK; i <= Config::PIECE_MAX_RANK; i++){
            std::cout << "\t"
                      << Piece::pieceInfo[i].symbol
                      << " "
                      << Piece::pieceInfo[i].name
                      << std::endl;
        }
    } else{
        AnsiColor bold {AnsiColor::BOLD};
        std::cout << "\t"
                  << Piece::pieceInfo[rank_].symbol
                  << " "
                  << Piece::pieceInfo[rank_].name
                  << std::endl << "\t"
                  << bold.colorize("Description")
                  << ": " << Piece::description(rank_) << std::endl;
    }
}

void PieceCommand::undo() noexcept{}


StopCommand::StopCommand(Controller& controller) :
    controller_ {controller}
{}

void StopCommand::exec() noexcept{
    controller_.stop();
}

void StopCommand::undo() noexcept{}


StatCommand::StatCommand(const Player& player, int rank) :
    player_ {player},
    rank_ {rank}
{}

void StatCommand::exec() noexcept{
    if(rank_ == -1){
        const std::map<int, int>& stat {player_.stats()};
        for(int i = Config::PIECE_MIN_RANK; i <= Config::PIECE_MAX_RANK; i++){
            int count {stat.at(i)};
            std::cout << "\t" << Piece::pieceInfo[i].symbol << " " << Piece::pieceInfo[i].name
                      << " - " << util::strrepeat("*", count)
                      << " (" << count << ")"
                      << std::endl;
        }

        std::cout << std::endl << "\tTotal: " << player_.remainingPieces() << std::endl;
    } else{
        int count {player_.remainingPieces(rank_)};
        std::cout << "\t" << Piece::pieceInfo[rank_].symbol << " "
                  << Piece::pieceInfo[rank_].name
                  << " - " << util::strrepeat("*", count)
                  << " (" << count << ")"
                  << std::endl;
    }
}

void StatCommand::undo() noexcept{}


HistoryCommand::HistoryCommand(const History& history) :
    hist_ {history}
{}

void HistoryCommand::exec() noexcept{
    const std::vector<tmstring>& history {hist_.allSuccess()};
    std::for_each(history.begin(), history.end(), [](const tmstring& str){
        std::cout << "["
                  << AnsiColor::colorText(str.toReadableTime(), AnsiColor::BOLD)
                  << "] "
                  << str.info << std::endl;
    });
}

void HistoryCommand::undo() noexcept{}

ClearCommand::ClearCommand(View& view, Color color, const std::string& pseudo) :
    view_ {view},
    color_ {color},
    pseudo_ {pseudo}
{}

void ClearCommand::exec() noexcept{
    AnsiColor red {AnsiColor::RED};
    AnsiColor blue {AnsiColor::BLUE};
    AnsiColor stop {AnsiColor::RESET};

    Console::clear();
    view_.displayBoard();
    std::string player_color = color_ == Color::RED ? "rouge" : "bleu";
    std::cout << "[" << (color_ == Color::RED ? red : blue) << "Joueur " << player_color
              << stop << "]"
              << " Au tour de "
              << pseudo_
              << std::endl;
    view_.displayEatenPieces();
}

void ClearCommand::undo() noexcept{}
