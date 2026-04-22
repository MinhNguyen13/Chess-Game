#pragma once
#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(bool isWhite);

    bool isValidMove(Position from, Position to, Board& board) override;
    char getSymbol() override;
};
