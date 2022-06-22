#include <regex>
#include <fstream>
#include <config.h>
#include <model.h>
#include <QMimeData>

#include "lineedit.h"

using namespace stratego::view;



/* =============== QPredicateLineEdit =============== */
QPredicateLineEdit::QPredicateLineEdit(std::function<bool(const QString&)> predicate, QWidget* parent) :
    QLineEdit{parent},
    predicate_ {predicate}
{}

bool QPredicateLineEdit::isValid() const{
    return predicate_(text());
}



/* =============== QPseudoLineEdit =============== */
QPseudoLineEdit::QPseudoLineEdit(QWidget* parent) :
    QPredicateLineEdit{[](const QString& text){
        return std::regex_match(text.toStdString(), std::regex{"[[:alpha:]]{3,}"});
    }, parent}
{}


/* QComponent related (inherited from it) */
void QPseudoLineEdit::compose(){
    setPlaceholderText("Votre pseudo...");
        QComponent::compose();
}

void QPseudoLineEdit::decompose(){
    QComponent::decompose();
}

void QPseudoLineEdit::reload(){
    QComponent::reload();
}

void QPseudoLineEdit::connectSlots(){}



/* =============== QFilenameLineEdit =============== */
QFilenameLineEdit::QFilenameLineEdit(QWidget* parent) :
    QPredicateLineEdit{[](const QString& text){
        if(text.isEmpty())
            return true;

        std::ifstream fstream {std::string{Config::BOARD_CONFIG_PATH} + text.toStdString()};
        return fstream.good();
    }, parent}
{
    setAcceptDrops(true);
}


/* QComponent related (inherited from it) */
void QFilenameLineEdit::compose(){
    setPlaceholderText("Nom du fichier de configuration...");
    QComponent::compose();
}

void QFilenameLineEdit::decompose(){
    QComponent::decompose();
}

void QFilenameLineEdit::reload(){
    QComponent::reload();
}

void QFilenameLineEdit::connectSlots(){}
