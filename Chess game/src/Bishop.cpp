#include "Bishop.h"
#include "Board.h"
#include <cmath>

Bishop::Bishop(bool isWhite) : Piece(isWhite) {}

bool Bishop::isValidMove(Position from, Position to, Board& board) {
    if (!board.isInside(to)) return false;
    int dx = to.row - from.row;
    int dy = to.col - from.col;

    // 1. phải đi chéo
    if (abs(dx) != abs(dy)) return false;

    int stepX = (dx > 0) ? 1 : -1;
    int stepY = (dy > 0) ? 1 : -1;

    int x = from.row + stepX;
    int y = from.col + stepY;

    // 2. check đường đi
    while (x != to.row && y != to.col) {
        if (!board.isEmpty(Position(x, y))) {
            return false;
        }
        x += stepX;
        y += stepY;
    }

    // 3. ô đích
    Piece* target = board.getPiece(to);

    if (target == nullptr) return true;
    return target->getColor() != this->getColor();
}

char Bishop::getSymbol() {
    return isWhite ? 'B' : 'b';
}
