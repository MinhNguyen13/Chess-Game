#pragma once
#include "Position.h"

class Board; // forward declaration

class Piece {
protected:
    bool isWhite;

public:
    Piece(bool isWhite);
    virtual ~Piece();

    bool getColor();

    // hàm ảo - mỗi quân override
    virtual bool isValidMove(Position from, Position to, Board& board) = 0;

    // để in ra bàn cờ
    virtual char getSymbol() = 0;
};
