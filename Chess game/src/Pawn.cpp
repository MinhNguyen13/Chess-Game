#include "Pawn.h"
#include "Board.h"

#include<iostream>

Pawn::Pawn(bool isWhite) : Piece(isWhite) {}

bool Pawn::isValidMove(Position from, Position to, Board& board) {
    if (!board.isInside(to)) return false;

    int direction = isWhite ? -1 : 1; // trắng đi lên, đen đi xuống

    int dx = to.row - from.row;
    int dy = to.col - from.col;

    // 1. Đi thẳng 1 ô
    if (dy == 0 && dx == direction) {
        if (board.isEmpty(to)) {
            return true;
        }
    }

    // 2. Đi 2 ô từ vị trí ban đầu
    if (dy == 0 && dx == 2 * direction) {
        int startRow = isWhite ? 6 : 1;

        if (from.row == startRow) {
            Position mid(from.row + direction, from.col);

            if (board.isEmpty(mid) && board.isEmpty(to)) {
                return true;
            }
        }
    }

    // 3. Ăn chéo
    if (abs(dy) == 1 && dx == direction) {
        Piece* target = board.getPiece(to);
        // Normal
        if (target != nullptr && target->getColor() != this->getColor()) {
            return true;
        }
        //En Passant
        if(board.canEnPassant(to)){
            return true;
        }

    }

    return false;
}

char Pawn::getSymbol() {
    return isWhite ? 'P' : 'p';
}
