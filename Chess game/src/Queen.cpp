#include "Queen.h"
#include "Board.h"
#include <cmath>

Queen::Queen(bool isWhite) : Piece(isWhite) {}

bool Queen::isValidMove(Position from, Position to, Board& board) {
    if (!board.isInside(to)) return false;
    int dx = to.row - from.row;
    int dy = to.col - from.col;

    // ===== CASE 1: Rook move =====
    if (dx == 0 || dy == 0) {
        int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
        int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

        int x = from.row + stepX;
        int y = from.col + stepY;

        while (x != to.row || y != to.col) {
            if (!board.isEmpty(Position(x, y))) return false;
            x += stepX;
            y += stepY;
        }
    }
    // ===== CASE 2: Bishop move =====
    else if (abs(dx) == abs(dy)) {
        int stepX = (dx > 0) ? 1 : -1;
        int stepY = (dy > 0) ? 1 : -1;

        int x = from.row + stepX;
        int y = from.col + stepY;

        while (x != to.row && y != to.col) {
            if (!board.isEmpty(Position(x, y))) return false;
            x += stepX;
            y += stepY;
        }
    }
    else {
        return false;
    }

    // ô đích
    Piece* target = board.getPiece(to);

    if (target == nullptr) return true;
    return target->getColor() != this->getColor();
}

char Queen::getSymbol() {
    return isWhite ? 'Q' : 'q';
}
