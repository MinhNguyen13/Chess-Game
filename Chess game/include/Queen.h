#pragma once
#include "Piece.h"

class Queen : public Piece {
public:
    Queen(bool isWhite);
    bool isValidMove(Position from, Position to, Board& board) override;
    char getSymbol() override;
};
