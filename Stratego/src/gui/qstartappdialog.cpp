#include <QPushButton>
#include <config.h>

#include "qappdialog.h"
#include "util.h"

using namespace stratego::view;

QStartAppDialog::QStartAppDialog(QWidget* parent) :
    QMessageBox{parent}
{}


/* QComponent related (inherited from it) */
void QStartAppDialog::compose(){
    QFont newFont {};
    QPixmap img {};
    newFont.setPointSize(11);

    // font & styles
    setFont(newFont);

    // message box related
    QPushButton* startBtn {addButton("&Start", QMessageBox::AcceptRole)};
    setDefaultButton(startBtn);
    img.load((std::string{Config::IMAGE_PATH} + "stratego-icon.png").c_str(), "PNG");
    setIconPixmap(img.scaled(100, 100, Qt::KeepAspectRatio));
    setWindowTitle(" ");
    setTextFormat(Qt::RichText);
    setText("<h1 align='left'>Bienvenue !</h1><p align='left'>Avant de commencer une partie, posons le contexte "
            "du jeu.<br /><br /><strong>Stratego</strong> est un jeu de plateau se jouant à 2 joueurs où le but "
            "est de capturer le drapeau du camp ennemi ou d'empêcher le joueur adverse de se déplacer."
            "</p><p align='right'>Appuyez sur <strong>Start</strong> pour commencer.</p>");

    // rendering
    util::centerWidget(this);

    QComponent::compose();
}

void QStartAppDialog::decompose(){
    QComponent::decompose();
}

void QStartAppDialog::reload(){
    QComponent::reload();
}

void QStartAppDialog::connectSlots(){}
