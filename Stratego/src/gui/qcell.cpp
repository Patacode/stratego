#include <../core/util.h>
#include <QBitmap>
#include <QMimeData>
#include <QApplication>

#include "qcell.h"

using namespace stratego::view;

QCell::QCell(model::Cell::Type type, int row, int col, model::Piece* piece, QWidget* parent) :
    QPushButton{parent},
    type_ {type},
    piece_ {piece},
    row_ {row},
    col_ {col}
{}


/* QComponent related (inherited from it) */
void QCell::compose(){
    setMouseTracking(true);
    setMinimumSize(QSize{50, 50});
    reload();

    QComponent::compose();
}

void QCell::decompose(){
    if(piece_.piece()){
        delete piece_.piece();
        piece_.setPiece(nullptr);
    }

    QComponent::decompose();
}

void QCell::reload(){
    if(type_ != model::Cell::NORMAL){
        std::string typeName {model::Cell::typeNames[type_]};
        util::strtolower(typeName);
        std::string iconPath {std::string{Config::IMAGE_PATH} + "icons-" + typeName};
        QPixmap pixmap {iconPath.c_str()};

        if(type_ == model::Cell::WALL){
            QBitmap mask {pixmap.createMaskFromColor(QColor{176, 193, 212}, Qt::MaskOutColor)};
            pixmap.fill(QColor{230, 81, 0});
            pixmap.setMask(mask);
        }

        setIcon(QIcon{pixmap});
        setIconSize(QSize{50, 50});
        setStyleSheet("background-color: transparent");
    } else if(piece_.piece() && piece_.piece() -> revealed()){
        model::Color color {piece_.piece() -> color()};
        setIcon(piece_.icon());
        setIconSize(QSize{30, 30});
        setToolTip((std::string(piece_.piece() -> info().symbol) + " " + std::string{piece_.piece() -> name()}).c_str());
        setStyleSheet((std::string{"QPushButton{background-color: "} + (color == model::Color::RED ? "black" : "white") + "}").c_str());
    } else if(piece_.piece() && !piece_.piece() -> revealed()){
        model::Color color {piece_.piece() -> color()};
        setIcon(QIcon{});
        setStyleSheet((std::string{"QPushButton{background-color: "} + (color == model::Color::RED ? "black" : "white") + "}").c_str());
        setToolTip("");
    } else{
        setIcon(QIcon{});
        setStyleSheet("");
        setToolTip("");
    }

    repaint();
    QComponent::reload();
}

void QCell::connectSlots(){
    QObject::connect(this, &QPushButton::clicked, this, &QCell::buttonClicked);
}


/* Related to this class only */
stratego::model::Cell::Type QCell::type() const{
    return type_;
}

QPiece& QCell::qpiece(){
    return piece_;
}

const QPiece& QCell::qpiece() const{
    return piece_;
}

int QCell::row() const{
    return row_;
}

int QCell::col() const{
    return col_;
}

void QCell::setRow(int row){
    row_ = row;
}

void QCell::setCol(int col){
    col_ = col;
}

void QCell::highlight(bool state){
    if(state){
        setStyleSheet("background-color: green");
    } else{
        setStyleSheet("");
    }
}


/* Overriden events */
void QCell::mouseMoveEvent(QMouseEvent* event){
    emit cellHovered(this);
}

void QCell::leaveEvent(QEvent *event){
    emit cellLeaved(this);
}

/* Slots */
void QCell::buttonClicked(bool checked){
    emit cellClicked(this);
}
