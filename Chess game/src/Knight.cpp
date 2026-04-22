#include "Knight.h"
#include "Board.h"
#include <cmath>

Knight::Knight(bool isWhite) : Piece(isWhite) {}

bool Knight::isValidMove(Position from, Position to, Board& board) {
    if (!board.isInside(to)) return false;
    int dx = abs(to.row - from.row);
    int dy = abs(to.col - from.col);

    // 1. đúng shape L
    if (!((dx == 2 && dy == 1) || (dx == 1 && dy == 2))) {
        return false;
    }

    // 2. ô đích
    Piece* target = board.getPiece(to);

    // trống → đi được
    if (target == nullptr) return true;

    // khác màu → ăn được
    return target->getColor() != this->getColor();
}

char Knight::getSymbol() {
    return isWhite ? 'N' : 'n';
}
