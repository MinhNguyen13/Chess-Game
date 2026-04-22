#include "King.h"
#include "Board.h"
#include <cmath>

King::King(bool isWhite) : Piece(isWhite) {
    hasMoved = false;
}

bool King::getHasMoved() { return hasMoved; }
void King::setHasMoved(bool val) { hasMoved = val; }

bool King::isValidMove(Position from, Position to, Board& board) {
    if (!board.isInside(to)) return false;
    int dx = abs(to.row - from.row);
    int dy = abs(to.col - from.col);

    // 1. chỉ đi 1 ô
    if (dx <= 1 && dy <= 1){
        // ô đích
        Piece* target = board.getPiece(to);
        if (target == nullptr) return true;
        return target->getColor() != this->getColor();
    }
    // CÓ THỂ castling
    if (dx == 0 && dy == 2 && !hasMoved) {
        return true;
    }

    return false;


}

char King::getSymbol() {
    return isWhite ? 'K' : 'k';
}
