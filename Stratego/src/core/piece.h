#ifndef PIECE_H
#define PIECE_H

/* =========================================================
 * Regroupe l'ensemble des pions différents intervenant dans
 * une partie de Stratego
 * =========================================================
 */

#include "gamestuff.h"

namespace stratego::model{

    /**
     * Le Maréchal de rang 10.
     *
     * => Quantité: 1
     */
    class Marshal : public Piece{

        public:

            /**
             * Construit un Maréchal de rang 10 à la position initial et de couleur donnée.
             *
             * @param initPos la position initiale du pion
             * @param color la couleur du pion
             * @param board la référence vers le plateau de jeu sur lequelle le pion se trouve
             * @param graph la référence vers le graphe d'état utilisé par le modèle
             * @param hist la référence vers l'historique utilisé par le modèle
             */
            Marshal(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist);


            // --- Déjà documenté ---
            bool canMove(const Position& pos) const noexcept;
            bool canAttack(const Position& pos) const noexcept;
    };

    /**
     * Le Général de rang 9.
     *
     * => Quantité: 1
     */
    class General : public Piece{

        public:

            /**
             * Construit un Général de rang 9 à la position initial et de couleur donnée.
             *
             * @param initPos la position initiale du pion
             * @param color la couleur du pion
             * @param board la référence vers le plateau de jeu sur lequelle le pion se trouve
             * @param graph la référence vers le graphe d'état utilisé par le modèle
             * @param hist la référence vers l'historique utilisé par le modèle
             */
            General(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist);


            // --- Déjà documenté ---
            bool canMove(const Position& pos) const noexcept;
            bool canAttack(const Position& pos) const noexcept;
    };

    /**
     * Le Colonel de rang 8.
     *
     * => Quantité: 2
     */
    class Colonel : public Piece{

        public:

            /**
             * Construit un Colonel de rang 8 à la position initial et de couleur donnée.
             *
             * @param initPos la position initiale du pion
             * @param color la couleur du pion
             * @param board la référence vers le plateau de jeu sur lequelle le pion se trouve
             * @param graph la référence vers le graphe d'état utilisé par le modèle
             * @param hist la référence vers l'historique utilisé par le modèle
             */
            Colonel(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist);


            // --- Déjà documenté ---
            bool canMove(const Position& pos) const noexcept;
            bool canAttack(const Position& pos) const noexcept;
    };

    /**
     * Le Major de rang 7.
     *
     * => Quantité: 3
     */
    class Major : public Piece{

        public:

            /**
             * Construit un Major de rang 7 à la position initial et de couleur donnée.
             *
             * @param initPos la position initiale du pion
             * @param color la couleur du pion
             * @param board la référence vers le plateau de jeu sur lequelle le pion se trouve
             * @param graph la référence vers le graphe d'état utilisé par le modèle
             * @param hist la référence vers l'historique utilisé par le modèle
             */
            Major(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist);


            // --- Déjà documenté ---
            bool canMove(const Position& pos) const noexcept;
            bool canAttack(const Position& pos) const noexcept;
    };

    /**
     * Le Commandant de rang 6.
     *
     * => Quantité: 4
     */
    class Captain : public Piece{

        public:

            /**
             * Construit un Commandant de rang 6 à la position initial et de couleur donnée.
             *
             * @param initPos la position initiale du pion
             * @param color la couleur du pion
             * @param board la référence vers le plateau de jeu sur lequelle le pion se trouve
             * @param graph la référence vers le graphe d'état utilisé par le modèle
             * @param hist la référence vers l'historique utilisé par le modèle
             */
            Captain(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist);


            // --- Déjà documenté ---
            bool canMove(const Position& pos) const noexcept;
            bool canAttack(const Position& pos) const noexcept;
    };

    /**
     * Le Lieutenant de rang 5.
     *
     * => Quantité: 4
     */
    class Lieutenant : public Piece{

        public:

            /**
             * Construit un Lieutenant de rang 5 à la position initial et de couleur donnée.
             *
             * @param initPos la position initiale du pion
             * @param color la couleur du pion
             * @param board la référence vers le plateau de jeu sur lequelle le pion se trouve
             * @param graph la référence vers le graphe d'état utilisé par le modèle
             * @param hist la référence vers l'historique utilisé par le modèle
             */
            Lieutenant(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist);


            // --- Déjà documenté ---
            bool canMove(const Position& pos) const noexcept;
            bool canAttack(const Position& pos) const noexcept;
    };

    /**
     * Le Sergent de rang 4.
     *
     * => Quantité: 4
     */
    class Sergent : public Piece{

        public:

            /**
             * Construit un Sergent de rang 4 à la position initial et de couleur donnée.
             *
             * @param initPos la position initiale du pion
             * @param color la couleur du pion
             * @param board la référence vers le plateau de jeu sur lequelle le pion se trouve
             * @param graph la référence vers le graphe d'état utilisé par le modèle
             * @param hist la référence vers l'historique utilisé par le modèle
             */
            Sergent(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist);


            // --- Déjà documenté ---
            bool canMove(const Position& pos) const noexcept;
            bool canAttack(const Position& pos) const noexcept;
    };

    /**
     * Le Démineur de rang 3. Ce pion possède la particularité à pouvoir déblayer une bombe.
     *
     * => Quantité: 5
     */
    class Miner : public Piece{

        public:

            /**
             * Construit un Démineur de rang 3 à la position initial et de couleur donnée.
             *
             * @param initPos la position initiale du pion
             * @param color la couleur du pion
             * @param board la référence vers le plateau de jeu sur lequelle le pion se trouve
             * @param graph la référence vers le graphe d'état utilisé par le modèle
             * @param hist la référence vers l'historique utilisé par le modèle
             */
            Miner(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist);


            // --- Déjà documenté ---
            bool canMove(const Position& pos) const noexcept;
            bool canAttack(const Position& pos) const noexcept;
    };

    /**
     * L'éclaireur de rang 2. Ce pion possède la particularité à pouvoir se déplacer et attaquer d'un
     * nombre de cases indéterminés.
     *
     * => Quantité: 8
     */
    class Scout : public Piece{

        public:

            /**
             * Construit un Éclaireur de rang 2 à la position initial et de couleur donnée.
             *
             * @param initPos la position initiale du pion
             * @param color la couleur du pion
             * @param board la référence vers le plateau de jeu sur lequelle le pion se trouve
             * @param graph la référence vers le graphe d'état utilisé par le modèle
             * @param hist la référence vers l'historique utilisé par le modèle
             */
            Scout(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist);


            // --- Déjà documenté ---
            bool canMove(const Position& pos) const noexcept;
            bool canAttack(const Position& pos) const noexcept;
    };

    /**
     * L'espionne de rang 1. Ce pion possède la particularité à pouvoir battre le Maréchal même si elle se fait battre
     * par n'importe quel autre pion
     *
     * => Quantité: 1
     */
    class Spy : public Piece{

        public:

            /**
             * Construit une Espionne de rang 1 à la position initial et de couleur donnée.
             *
             * @param initPos la position initiale du pion
             * @param color la couleur du pion
             * @param board la référence vers le plateau de jeu sur lequelle le pion se trouve
             * @param graph la référence vers le graphe d'état utilisé par le modèle
             * @param hist la référence vers l'historique utilisé par le modèle
             */
            Spy(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist);


            // --- Déjà documenté ---
            bool canMove(const Position& pos) const noexcept;
            bool canAttack(const Position& pos) const noexcept;
    };

    /**
     * La Bombe de rang 11. Ce pion ne peut se déplacer ou attaquer mais peut battre tout le monde sauf le démineur.
     *
     * => Quantité: 6
     */
    class Bomb : public Piece{

        public:

            /**
             * Construit une Bombe de rang 11 à la position initial et de couleur donnée.
             *
             * @param initPos la position initiale du pion
             * @param color la couleur du pion
             * @param board la référence vers le plateau de jeu sur lequelle le pion se trouve
             * @param graph la référence vers le graphe d'état utilisé par le modèle
             * @param hist la référence vers l'historique utilisé par le modèle
             */
            Bomb(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist);


            // --- Déjà documenté ---
            bool canMove(const Position& pos) const noexcept;
            bool canAttack(const Position& pos) const noexcept;
    };

    /**
     * Le Drapeau de rang 0. Ce pion ne fait rien, comme la bombe, et ne bat personne, mais la victoire revient au joueur
     * arrivant à s'emparer de ce pion en premier.
     *
     * => Quantité: 1
     */
    class Flag : public Piece{

        public:

            /**
             * Construit un Drapeau de rang 0 à la position initial et de couleur donnée.
             *
             * @param initPos la position initiale du pion
             * @param color la couleur du pion
             * @param board la référence vers le plateau de jeu sur lequelle le pion se trouve
             * @param graph la référence vers le graphe d'état utilisé par le modèle
             * @param hist la référence vers l'historique utilisé par le modèle
             */
            Flag(const Position& initPos, Color color, Board& board, StateGraph& graph, History& hist);


            // --- Déjà documenté ---
            bool canMove(const Position& pos) const noexcept;
            bool canAttack(const Position& pos) const noexcept;
    };
};


#endif

