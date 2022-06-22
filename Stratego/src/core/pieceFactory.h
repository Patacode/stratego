#ifndef PIECEFACTORY_H
#define PIECEFACTORY_H

#include "gamestuff.h"

namespace stratego::model {

    /**
     * Factory de pions.
     */
    class PieceFactory{

    public:

        /**
         * Construit une PieceFactory chargé de créer des pions.
         */
        PieceFactory() noexcept;

        /**
         * Créé un pion de rang donné, à la position de départ et de couleur donné et utilise d'autres paramètres pour assurer
         * sa bonne instantiation et son bon fonctionnement.
         *
         * @param rank le rang du pion à créer
         * @param initPos la position de départ du pion
         * @param color la couleur du pion
         * @param board le board utilisé par le pion
         * @param graph le graphe d'état utilisé par le pion
         * @param hist l'historique utilisé par le pion
         *
         * @return un pion de rang donné correctement initialisé.
         */
        Piece* createPiece(int rank, const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist) noexcept;
    };
}

#endif // PIECEFACTORY_H
