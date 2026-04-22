#pragma once
#include "Piece.h"

class King : public Piece {
private:
    bool hasMoved;
public:
    King(bool isWhite);
    bool isValidMove(Position from, Position to, Board& board) override;
    char getSymbol() override;

    bool getHasMoved();
    void setHasMoved(bool val);
};
