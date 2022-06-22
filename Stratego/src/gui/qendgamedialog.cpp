#include <QPushButton>
#include <config.h>

#include "qappdialog.h"
#include "util.h"

using namespace stratego::view;

QEndGameDialog::QEndGameDialog(const QString& message, QWidget* parent) :
    QMessageBox{parent},
    message_ {message}
{}


/* QComponent related (inherited from it) */
void QEndGameDialog::compose(){
    QFont newFont {};
    QPixmap img {};
    newFont.setPointSize(11);

    // font & styles
    setFont(newFont);

    // message box related
    QPushButton* yesBtn {addButton("&Oui", QMessageBox::AcceptRole)};
    addButton("&Non", QMessageBox::RejectRole);
    setDefaultButton(yesBtn);
    img.load((std::string{Config::IMAGE_PATH} + "stratego-icon.png").c_str(), "PNG");
    setIconPixmap(img.scaled(100, 100, Qt::KeepAspectRatio));
    setWindowTitle("Fin de partie");
    setTextFormat(Qt::RichText);
    setText((message_.toStdString() + std::string{"<br />Voulez-vous <strong>rejouer</strong> ?"}).c_str());

    // rendering
    util::centerWidget(this);

    QComponent::compose();
}

void QEndGameDialog::decompose(){
    QComponent::decompose();
}

void QEndGameDialog::reload(){
    QComponent::reload();
}

void QEndGameDialog::connectSlots(){}
