#pragma once
#include "Piece.h"

class Knight : public Piece {
public:
    Knight(bool isWhite);

    bool isValidMove(Position from, Position to, Board& board) override;
    char getSymbol() override;
};
