#include <../core/util.h>
#include <QBitmap>
#include "qpiece.h"

using namespace stratego::view;

QPiece::QPiece(model::Piece* piece) :
    icon_ {},
    piece_ {piece}
{}

QIcon& QPiece::icon(){
    return icon_;
}

const QIcon& QPiece::icon() const{
    return icon_;
}

stratego::model::Piece* QPiece::piece(){
    return piece_;
}

const stratego::model::Piece* QPiece::piece() const{
    return piece_;
}

void QPiece::setPiece(model::Piece* piece){
    piece_ = piece;
    if(piece_){
        std::string pieceName {std::string{piece_ -> name()}};
        util::strtolower(pieceName);
        std::string piecePath {std::string{Config::IMAGE_PATH} + "piece-" + pieceName};
        std::string color {piece_ -> color() == model::Color::RED ? "red" : "blue"};

        QPixmap pixmap {piecePath.c_str()};
        QBitmap mask {pixmap.createMaskFromColor(QColor{"white"}, Qt::MaskOutColor)};
        pixmap.fill(QColor{color.c_str()});
        pixmap.setMask(mask);

        icon_ = QIcon{pixmap};
    } else{
        icon_ = QIcon{};
    }
}
