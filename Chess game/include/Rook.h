#pragma once
#include "Piece.h"

class Rook : public Piece {
private:
    bool hasMoved;
public:
    Rook(bool isWhite);

    bool isValidMove(Position from, Position to, Board& board) override;
    char getSymbol() override;

    bool getHasMoved();
    void setHasMoved(bool val);
};
