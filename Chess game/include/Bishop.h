#pragma once
#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(bool isWhite);
    bool isValidMove(Position from, Position to, Board& board) override;
    char getSymbol() override;
};
