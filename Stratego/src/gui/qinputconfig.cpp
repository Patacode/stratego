#include "qinputconfig.h"

using namespace stratego::view;

QInputConfig::QInputConfig(QWidget* parent) :
    QFrame{parent},
    container_ {new QHBoxLayout},
    pseudoLineEdit_ {new QPseudoLineEdit},
    configLineEdit_ {new QFilenameLineEdit},
    loadButton_ {new QPushButton{"&Load"}},
    finishButton_ {new QPushButton{"&Finish"}}
{
    addChildren({pseudoLineEdit_, configLineEdit_});
    setLayout(container_);
}


/* QComponent related (inherited from it) */
void QInputConfig::compose(){
    container_ -> addWidget(pseudoLineEdit_, 4);
    container_ -> addWidget(configLineEdit_, 10);
    container_ -> addWidget(loadButton_);
    container_ -> addWidget(finishButton_);

    loadButton_ -> setToolTip("Charge le fichier de configuration");
    finishButton_ -> setToolTip("Termine la configuration du plateau de jeu");

    QComponent::compose();
}

void QInputConfig::decompose(){
    QComponent::decompose();
}

void QInputConfig::reload(){
    QComponent::reload();
}

void QInputConfig::connectSlots(){
    QObject::connect(loadButton_, &QPushButton::clicked, this, &QInputConfig::loaded);
    QObject::connect(finishButton_, &QPushButton::clicked, this, &QInputConfig::finished);
}


/* Slots */
void QInputConfig::loaded(){
    QString configFilename {configLineEdit_ -> text()};
    bool configFilenameValidity {configLineEdit_ -> isValid()};

    if(configFilenameValidity) emit loadClicked(configFilename);
    else emit errorOccured("Fichier invalide.");
}

void QInputConfig::finished(){
    QString pseudo {pseudoLineEdit_ -> text()};
    bool pseudoValidity {pseudoLineEdit_ -> isValid()};

    if(pseudoValidity) emit finishClicked(pseudo);
    else emit errorOccured("Pseudo invalide.");
}
