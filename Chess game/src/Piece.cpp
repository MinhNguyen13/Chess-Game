#include "Piece.h"

Piece::Piece(bool isWhite) {
    this->isWhite = isWhite;
}

Piece::~Piece() {}

bool Piece::getColor() {
    return isWhite;
}
