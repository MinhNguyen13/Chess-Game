#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"

class Board
{
private:
    static const int szBoard = 8;
    Piece* grid[szBoard][szBoard];
    Position enPassantTarget;
    bool hasEnPassant = false;


public:
    Board();
    void initialize();

    Piece* getPiece(Position pos);
    bool isEmpty(Position pos);
    bool isInside(Position pos);

    Position findKing(bool isWhite);
    bool isCheck(bool isWhite);
    bool wouldBeInCheck(Position from, Position to, bool color);
    bool isLegalMove(Position from, Position to, bool isWhiteTurn);
    //
    bool isCheckmate(bool isWhite);
    //Stalemate
    bool isStalemate(bool isWhite);


    //Normal move funcs
    void performNormalMove(Position from, Position to);
    void updateMovedFlag(Piece* piece);

    //Promotion funcs
    bool isPromotionSquare(Position pos);
    void handlePromotion(Position pos, char choice);

    //Castling funcs
    bool isCastlingMove(Position from, Position to);
    bool canCastle(Position from, Position to, bool isWhite);
    void performCastling(Position from, Position to);

    // En Passant funcs
    bool isEnPassantMove(Position from, Position to);
    void performEnPassant(Position from, Position to);
    bool canEnPassant(Position to) const;

    //
    void print();
    void movePiece(Position from, Position to);

    ~Board();

};

#endif // BOARD_H
