#include <QPushButton>
#include <config.h>

#include "qappdialog.h"
#include "util.h"

using namespace stratego::view;

QStartGameDialog::QStartGameDialog(QWidget* parent) :
    QMessageBox{parent}
{}


/* QComponent related (inherited from it) */
void QStartGameDialog::compose(){
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
    setText("<h1 align='left'>À vos marques !</h1><p align='left'>Vous vous apprêter à commencer la partie. Préparez-vous et"
            " appuyez sur <strong>Start</strong> pour commencer.</p><p>À noter que c'est le joueur rouge qui commence.</p>");

    // rendering
    util::centerWidget(this);

    QComponent::compose();
}

void QStartGameDialog::decompose(){
    QComponent::decompose();
}

void QStartGameDialog::reload(){
    QComponent::reload();
}

void QStartGameDialog::connectSlots(){}
