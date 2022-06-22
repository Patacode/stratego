#include <QInputDialog>
#include <regex>

#include "qappwindow.h"

using namespace stratego::view;

/* ========== QConfigWindow ========== */
QConfigWindow::QConfigWindow(const Model* model, model::Color playerColor, QWidget* parent) :
    QFrame{parent},
    model_ {model},
    playerColor_ {playerColor},
    playerPointer_ {},
    playerNames_ {},
    container_ {new QVBoxLayout},
    title_ {new QLabel},
    configPanel_ {new QConfigPanel{model_}},
    saveButton_ {new QPushButton{"&Save"}},
    userInput_ {new QInputConfig},
    lastClickedBoardCell_ {},
    lastClickedStorageCell_ {}
{
    addChildren({configPanel_, userInput_});
    setLayout(container_);
}


/* QComponent related (inherited from it) */
void QConfigWindow::compose(){
    updateTitle();

    title_ -> setTextFormat(Qt::RichText);
    title_ -> setAlignment(Qt::AlignCenter);

    container_ -> addWidget(title_);
    container_ -> addWidget(configPanel_);
    container_ -> addWidget(saveButton_);
    container_ -> addWidget(userInput_);

    saveButton_ -> setToolTip("Sauvegarde la configuration du plateau de jeu");

    QComponent::compose();
}

void QConfigWindow::decompose(){
    QComponent::decompose();
}

void QConfigWindow::reload(){
    updateTitle();
    configPanel_ -> board() -> reload(playerColor_);
    configPanel_ -> storage() -> reload();
    userInput_ -> reload();
}

void QConfigWindow::connectSlots(){
    QObject::connect(userInput_, &QInputConfig::loadClicked, this, &QConfigWindow::loaded);
    QObject::connect(userInput_, &QInputConfig::finishClicked, this, &QConfigWindow::finished);
    QObject::connect(userInput_, &QInputConfig::errorOccured, this, &QConfigWindow::error);
    QObject::connect(configPanel_ -> board(), &QBoard::errorOccured, this, &QConfigWindow::error);
    QObject::connect(configPanel_ -> board(), &QBoard::fileDropped, this, &QConfigWindow::dropLoaded);

    QObject::connect(configPanel_ -> board(), &QBoard::cellHovered, this, &QConfigWindow::hovered);
    QObject::connect(configPanel_ -> storage(), &QPieceStorage::cellHovered, this, &QConfigWindow::hovered);

    QObject::connect(configPanel_ -> board(), &QBoard::cellLeaved, this, &QConfigWindow::leaved);
    QObject::connect(configPanel_ -> storage(), &QPieceStorage::cellLeaved, this, &QConfigWindow::leaved);

    QObject::connect(configPanel_ -> board(), &QBoard::cellClicked, this, &QConfigWindow::boardClicked);
    QObject::connect(configPanel_ -> storage(), &QPieceStorage::cellClicked, this, &QConfigWindow::storageClicked);

    QObject::connect(saveButton_, &QPushButton::clicked, this, &QConfigWindow::saved);
}


/* Related to this class only */
stratego::model::Color QConfigWindow::playerColor() const{
    return playerColor_;
}

void QConfigWindow::updateTitle(){
    std::string title {"<h3><strong>Configuration</strong> du plateau de jeu (Joueur <font color='"
        + std::string{playerColor_ == model::Color::RED ? "red" : "blue"} + "'>"
        + std::string{playerColor_ == model::Color::RED ? "rouge" : "bleu"} + "</font>)</h3>"
        "<p>Déposez un fichier de configuration "
        "sur le plateau de jeu<br /> pour le charger, entrez manuellement "
        "son nom ou disposez les pions de la manière que vous voulez</p>"};

    title_ -> setText(title.c_str());
    title_ -> repaint();
}

bool QConfigWindow::validRow(int row) const{
  return playerColor_ == model::Color::RED ?
              row < Config::BOARD_SIZE - 1 && row >= Config::BOARD_SIZE - 5 :
              row < 5 && row >= 1;
}




/* ============================ SLOTS ============================ */
void QConfigWindow::saved(){
    if(!configPanel_ -> board() -> isFull(playerColor_)){
       error("Sauvegarde impossible. Le plateau de jeu n'est pas complet");
    } else{
        bool ok;
        QString text = QInputDialog::getText(this, "Configuration", "Nom de fichier", QLineEdit::Normal, "", &ok);
        if(ok){
            if(text.isEmpty() || !std::regex_match(text.toStdString(), std::regex{"[[:alnum:]]+"})){
                error("Nom de fichier invalide. Le nom ne peut être vide et ne peut contenir que des caractères alphanumériques.");
            } else{
                std::ifstream ifs {Config::BOARD_CONFIG_PATH + text.toStdString()};
                if(ifs.good()){
                    error("Un fichier de configuration de même nom existe");
                } else{
                    configPanel_ -> board() -> saveToFile(Config::BOARD_CONFIG_PATH + text.toStdString(), playerColor_);
                    emit successOccured("Configuration du plateau de jeu sauvegardé avec succès !");
                }
            }
        } else{
            error("Annulation de la sauvegarde");
        }
    }
}

void QConfigWindow::loaded(const QString& configFilename){
    if(configFilename.isEmpty()){
        std::vector<model::Piece*> pieces {const_cast<Model*>(model_) -> piecesOf(playerColor_)};                                                         
        configPanel_ -> storage() -> clear();
        configPanel_ -> board() -> clear(playerColor_);
        configPanel_ -> storage() -> fillin(pieces);
    } else{
        configPanel_ -> board() -> clear(playerColor_);
        configPanel_ -> storage() -> clear();
        emit loadClicked(configFilename.toStdString(), playerColor_);
    }
}

void QConfigWindow::dropLoaded(const QString& filepath){
    configPanel_ -> board() -> clear(playerColor_);
    configPanel_ -> storage() -> clear();
    emit loadClicked(filepath.toStdString(), playerColor_, true);
}

void QConfigWindow::finished(const QString& pseudo){
    if(!configPanel_ -> board() -> isFull(playerColor_)){
       error("Le plateau de jeu n'est pas complet");
    } else if(!model_ -> playerCanMove_startGame(playerColor_)){
        error("Positionnement invalide. Aucune pièce ne peut se déplacer.");
    } else if(playerPointer_ < Config::PLAYER_COUNT - 1){
        playerNames_[playerPointer_] = pseudo.toStdString();
        configPanel_ -> board() -> hideColor(playerColor_);
        playerPointer_ = (playerPointer_ + 1) % Config::PLAYER_COUNT;
        playerColor_ = static_cast<model::Color>(static_cast<int>(playerColor_) ^ static_cast<int>(model::Color::BLUE));
        updateTitle();
    } else{
        if(playerNames_[playerPointer_ - 1] == pseudo.toStdString()){
            error("Le pseudo entré est déjà utilisé par l'autre joueur");
        } else{
            playerNames_[playerPointer_] = pseudo.toStdString();
            configPanel_ -> board() -> hideColor(playerColor_);
            emit finishClicked(playerNames_[0], playerNames_[1]);
        }
    }
}

void QConfigWindow::error(const QString& error){
    emit errorOccured(error);
}

void QConfigWindow::hovered(const QString& info){
    emit cellHovered(info);
}

void QConfigWindow::leaved(){
    emit cellLeaved();
}

void QConfigWindow::boardClicked(QCell* cell){
    model::Board& board {const_cast<model::Board&>(model_ -> board())};
    if(lastClickedStorageCell_){ // when previously clicked on storage
        model::Piece* prevPiece {lastClickedStorageCell_ -> qpiece().piece()};
        model::Piece* piece {cell -> qpiece().piece()};
        int cellRow {cell -> row()}, cellCol {cell -> col()};

        if(piece && piece -> color() != playerColor_){
            error("Le pion selectionné ne vous appartient pas");
        } else if(!(prevPiece && piece) && ((prevPiece && !board.walkableCell(cellCol, cellRow)) || (prevPiece && !validRow(cellRow)))){
            error("Le pion ne peut se déplacer sur cette case lors de la mise en place du plateau de jeu");
        } else if(prevPiece || piece){
            lastClickedStorageCell_ -> qpiece().setPiece(piece);
            cell -> qpiece().setPiece(prevPiece);

            board.getCell(cellCol, cellRow).piece = prevPiece;
            if(piece) lastClickedStorageCell_ -> qpiece().piece() -> setPosition({0, 0});
            if(prevPiece) prevPiece -> setPosition(model::Position{cell -> col(), cell -> row()});

            cell -> reload();
            lastClickedStorageCell_ -> reload();
        }

        lastClickedStorageCell_ = nullptr;
    } else if(lastClickedBoardCell_ && lastClickedBoardCell_ != cell){ // when previously clicked on board
        model::Piece* prevPiece {lastClickedBoardCell_ -> qpiece().piece()};
        model::Piece* piece {cell -> qpiece().piece()};
        int cellRow {cell -> row()}, cellCol {cell -> col()};
        int prevRow {lastClickedBoardCell_ -> row()}, prevCol {lastClickedBoardCell_ -> col()};

        if((prevPiece && prevPiece -> color() != playerColor_) || (piece && piece -> color() != playerColor_)){
            error("Un des pions ne vous appartient pas");
        } else if(!(prevPiece && piece) &&
                 ((prevPiece && !board.walkableCell(cellCol, cellRow)) ||
                 (piece && !board.walkableCell(prevCol, prevRow)) ||
                 (prevPiece && !validRow(cellRow)) || (piece && !validRow(prevRow)))){
            error("Le pion ne peut se déplacer sur cette case lors de la mise en place du plateau de jeu");
        } else if(piece || prevPiece){
            if(piece) cell -> qpiece().piece() -> setPosition(model::Position{prevCol, prevRow});
            if(prevPiece) lastClickedBoardCell_ -> qpiece().piece() -> setPosition(model::Position{cellCol, cellRow});

            // swap internally
            board.getCell(prevCol, prevRow).piece = piece;
            board.getCell(cellCol, cellRow).piece = prevPiece;

            // swap on the UI
            cell -> qpiece().setPiece(prevPiece);
            lastClickedBoardCell_ -> qpiece().setPiece(piece);

            // reload
            cell -> reload();
            lastClickedBoardCell_ -> reload();
        }

        lastClickedBoardCell_ = nullptr;
    } else{ // when initiating the click
        lastClickedBoardCell_ = cell;
    }
}

void QConfigWindow::storageClicked(QCell* cell){
    model::Board& board {const_cast<model::Board&>(model_ -> board())};
    if(lastClickedBoardCell_){ // when previously clicked on board
        model::Piece* prevPiece {lastClickedBoardCell_ -> qpiece().piece()};
        model::Piece* piece {cell -> qpiece().piece()};
        int prevRow {lastClickedBoardCell_ -> row()}, prevCol {lastClickedBoardCell_ -> col()};

        if(prevPiece && prevPiece -> color() != playerColor_){
            error("Le pion selectionné ne vous appartient pas");
        } else if(!(prevPiece && piece) && ((piece && !board.walkableCell(prevCol, prevRow)) || (piece && !validRow(prevRow)))){
            error("Le pion ne peut se déplacer sur cette case lors de la mise en place du plateau de jeu");
        } else if(prevPiece || piece){
            lastClickedBoardCell_ -> qpiece().setPiece(piece);
            cell -> qpiece().setPiece(prevPiece);

            board.getCell(prevCol, prevRow).piece = piece;
            if(piece) lastClickedBoardCell_ -> qpiece().piece() -> setPosition({prevCol, prevRow});
            if(prevPiece) prevPiece -> setPosition({0, 0});

            cell -> reload();
            lastClickedBoardCell_ -> reload();
        }

        lastClickedBoardCell_ = nullptr;
    } else if(lastClickedStorageCell_ && lastClickedStorageCell_ != cell){ // when previously clicked on storage
        model::Piece* prevPiece {lastClickedStorageCell_ -> qpiece().piece()};
        model::Piece* piece {cell -> qpiece().piece()};

        // swap on UI
        lastClickedStorageCell_ -> qpiece().setPiece(piece);
        cell -> qpiece().setPiece(prevPiece);

        // reload
        lastClickedStorageCell_ -> reload();
        cell -> reload();

        lastClickedStorageCell_ = nullptr;
    } else{
        lastClickedStorageCell_ = cell;
    }
}
