#include "Rook.h"
#include "Board.h"
Rook::Rook(bool isWhite) : Piece(isWhite) {
    hasMoved = false;
}

bool Rook::getHasMoved() { return hasMoved; }
void Rook::setHasMoved(bool val) { hasMoved = val; }

bool Rook::isValidMove(Position from, Position to, Board& board) {
    if (!board.isInside(to)) return false;

    int dx = to.row - from.row;
    int dy = to.col - from.col;

    // 1. Phải đi thẳng hàng hoặc cột
    if (dx != 0 && dy != 0) {
        return false;
    }

    int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

    int x = from.row + stepX;
    int y = from.col + stepY;

    // 2. Kiểm tra đường đi có bị chặn không
    while (x != to.row || y != to.col) {
        if (!board.isEmpty(Position(x, y))) {
            return false;
        }
        x += stepX;
        y += stepY;
    }

    // 3. Ô đích
    Piece* target = board.getPiece(to);

    // nếu trống → đi được
    if (target == nullptr) return true;

    // nếu khác màu → ăn được
    if (target->getColor() != this->getColor()) return true;

    // cùng màu → không được
    return false;
}

char Rook::getSymbol() {
    return isWhite ? 'R' : 'r';
}
