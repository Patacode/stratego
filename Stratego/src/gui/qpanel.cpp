#include "qpanel.h"

using namespace stratego::view;

/* ========== QConfigPanel ========== */
QConfigPanel::QConfigPanel(const Model* model, QWidget* parent) :
    QFrame{parent},
    container_ {new QHBoxLayout},
    board_ {new QBoard{model}},
    storage_ {new QPieceStorage}
{
    addChildren({board_, storage_});
    setLayout(container_);
}

/* QComponent related (inherited from it) */
void QConfigPanel::compose(){
    for(QComponent* component : children_){
        QWidget* widget {dynamic_cast<QWidget*>(component)};
        container_ -> addWidget(widget);
    }

    QComponent::compose();
}

void QConfigPanel::decompose(){
    QComponent::decompose();
}

void QConfigPanel::reload(){
    QComponent::reload();
}

void QConfigPanel::connectSlots(){}


/* Related to this class only */
QBoard* QConfigPanel::board(){
    return board_;
}

QPieceStorage* QConfigPanel::storage(){
    return storage_;
}


/* ========== QGamePanel ========== */
QGamePanel::QGamePanel(const Model* model, QWidget* parent) :
    QFrame{parent},
    container_ {new QHBoxLayout},
    graveyardRed_ {new QGraveyard{model, model::Color::RED}},
    graveyardBlue_ {new QGraveyard{model, model::Color::BLUE}},
    board_ {new QBoard{model}},
    stats_ {new QPieceStats{model}}
{
    addChildren({graveyardRed_, graveyardBlue_, board_, stats_});
    setLayout(container_);
}

/* QComponent related (inherited from it) */
void QGamePanel::compose(){
    container_ -> setSpacing(10);
    for(QComponent* component : children_){
        QWidget* widget {dynamic_cast<QWidget*>(component)};
        container_ -> addWidget(widget);
    }

    QComponent::compose();
}

void QGamePanel::decompose(){
    QComponent::decompose();
}

void QGamePanel::reload(){
    QComponent::reload();
}

void QGamePanel::connectSlots(){}


/* Related to this class only */
QGraveyard* QGamePanel::graveyardRed(){
    return graveyardRed_;
}

QGraveyard* QGamePanel::graveyardBlue(){
    return graveyardBlue_;
}

QBoard* QGamePanel::board(){
    return board_;
}

QPieceStats* QGamePanel::stats(){
    return stats_;
}
