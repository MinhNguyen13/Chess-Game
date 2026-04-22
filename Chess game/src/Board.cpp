#include "Board.h"
#include "Pawn.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
#include "Knight.h"
#include "Bishop.h"

#include <iostream>
using namespace std;

Board::Board()
{
    initialize();
}

void Board::initialize() {
    // 1. set whole = nullptr
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            grid[i][j] = nullptr;
        }
    }

    // 2. Pawn
    for (int j = 0; j < 8; j++) {
        grid[1][j] = new Pawn(false); // black
        grid[6][j] = new Pawn(true);  // white
    }

    // 3. Rook
    grid[0][0] = grid[0][7] = new Rook(false);
    grid[7][0] = grid[7][7] = new Rook(true);

    // 4. Knight
    grid[0][1] = grid[0][6] = new Knight(false);
    grid[7][1] = grid[7][6] = new Knight(true);

    // 5. Bishop
    grid[0][2] = grid[0][5] = new Bishop(false);
    grid[7][2] = grid[7][5] = new Bishop(true);

    // 6. Queen
    grid[0][3] = new Queen(false);
    grid[7][3] = new Queen(true);

    // 7. King
    grid[0][4] = new King(false);
    grid[7][4] = new King(true);

}

Piece* Board::getPiece(Position pos){
    return grid[pos.row][pos.col];
}

bool Board::isEmpty(Position pos){
    return grid[pos.row][pos.col] == nullptr;
}

bool Board::isInside(Position pos) {
    return pos.row >= 0 && pos.row < 8 &&
           pos.col >= 0 && pos.col < 8;
}


Position Board::findKing(bool isWhite) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* p = grid[i][j];
            if (p != nullptr && p->getColor() == isWhite) {
                if (p->getSymbol() == (isWhite ? 'K' : 'k')) {
                    return Position(i, j);
                }
            }
        }
    }
    return Position(-1,-1);
}

bool Board::isCheck(bool isWhite) {
    Position kingPos = findKing(isWhite);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* p = grid[i][j];

            if (p != nullptr && p->getColor() != isWhite) {
                if (p->isValidMove(Position(i,j), kingPos, *this)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::wouldBeInCheck(Position from, Position to, bool color) {
    Piece* temp = grid[to.row][to.col];

    grid[to.row][to.col] = grid[from.row][from.col];
    grid[from.row][from.col] = nullptr;

    bool inCheck = isCheck(color);

    // rollback
    grid[from.row][from.col] = grid[to.row][to.col];
    grid[to.row][to.col] = temp;

    return inCheck;
}

// Tinh hop le cua nuoc di isWhiteTurn
bool Board::isLegalMove(Position from, Position to, bool isWhiteTurn) {
    Piece* piece = getPiece(from);

    if (piece == nullptr) return false;

    // 1. đúng màu
    if (piece->getColor() != isWhiteTurn) return false;

    // 2.1. không đứng yên
    if (from.row == to.row && from.col == to.col) return false;

    // 2.2. đúng luật quân
    if (!piece->isValidMove(from, to, *this)) return false;

    // di chuyển castling
    if (isCastlingMove(from, to)) {
        return canCastle(from, to, isWhiteTurn);
    }

    // giả lập move/ 0 bi chieu -> hop le
    if(wouldBeInCheck(from, to, isWhiteTurn)) return false;

    return true;

}

// Ham kiem tra vua isWhite co bi chieu
bool Board::isCheckmate(bool isWhite) {
    // 1. phải đang bị chiếu
    if (!isCheck(isWhite)) return false;

    // 2. thử tất cả nước đi
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = grid[i][j];

            if (piece != nullptr && piece->getColor() == isWhite) {
                Position from(i, j);

                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        Position to(x, y);

                        if (isLegalMove(from, to, isWhite)) {
                            return false; // có cách cứu
                        }
                    }
                }
            }
        }
    }

    return true; // hết cứu
}

bool Board::isStalemate(bool isWhite) {
    // 1. nếu đang bị chiếu → không phải stalemate
    if (isCheck(isWhite)) return false;

    // 2. thử tất cả nước đi
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = grid[i][j];

            if (piece != nullptr && piece->getColor() == isWhite) {
                Position from(i, j);

                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        Position to(x, y);

                        if (isLegalMove(from, to, isWhite)) {
                            return false; // còn nước đi → chưa hòa
                        }
                    }
                }
            }
        }
    }

    return true; // hết nước đi mà không bị chiếu → hòa
}

void Board::performNormalMove(Position from, Position to) {
    grid[to.row][to.col] = grid[from.row][from.col];
    grid[from.row][from.col] = nullptr;
}


bool Board::isPromotionSquare(Position pos) {
    Piece* piece = grid[pos.row][pos.col];

    Pawn* pawn = dynamic_cast<Pawn*>(piece);
    if (!pawn) return false;

    bool isWhite = pawn->getColor();

    return (isWhite && pos.row == 0) ||
           (!isWhite && pos.row == 7);
}

void Board::handlePromotion(Position pos, char choice) {
    Piece* piece = grid[pos.row][pos.col];
    Pawn* pawn = dynamic_cast<Pawn*>(piece);

    if (!pawn) return;

    bool isWhite = pawn->getColor();
    delete pawn;

    switch (toupper(choice)) {
        case 'R':
            grid[pos.row][pos.col] = new Rook(isWhite);
            break;
        case 'B':
            grid[pos.row][pos.col] = new Bishop(isWhite);
            break;
        case 'N':
            grid[pos.row][pos.col] = new Knight(isWhite);
            break;
        case 'Q':
        default:
            grid[pos.row][pos.col] = new Queen(isWhite);
            break;
    }
}


bool Board::isCastlingMove(Position from, Position to) {
    Piece* piece = getPiece(from);
    King* king = dynamic_cast<King*>(piece);

    return king && abs(to.col - from.col) == 2;
}

bool Board::canCastle(Position from, Position to, bool isWhite) {
    // đang bị chiếu → không được castle
    if (isCheck(isWhite)) return false;

    int row = from.row;

    // 👉 kingside (phải)
    if (to.col == 6) {
        Piece* rookPiece = grid[row][7];
        Rook* rook = dynamic_cast<Rook*>(rookPiece);

        if (!rook || rook->getHasMoved()) return false;

        // đường trống
        if (!isEmpty(Position(row,5)) || !isEmpty(Position(row,6)))
            return false;

        // không đi qua check
        if (wouldBeInCheck(from, Position(row,5), isWhite)) return false;
        if (wouldBeInCheck(from, Position(row,6), isWhite)) return false;

        return true;
    }

    // 👉 queenside (trái)
    if (to.col == 2) {
        Piece* rookPiece = grid[row][0];
        Rook* rook = dynamic_cast<Rook*>(rookPiece);

        if (!rook || rook->getHasMoved()) return false;

        if (!isEmpty(Position(row,1)) ||
            !isEmpty(Position(row,2)) ||
            !isEmpty(Position(row,3)))
            return false;

        if (wouldBeInCheck(from, Position(row,3), isWhite)) return false;
        if (wouldBeInCheck(from, Position(row,2), isWhite)) return false;

        return true;
    }

    return false;
}

void Board::performCastling(Position from, Position to) {
    int row = from.row;

    // move king
    grid[to.row][to.col] = grid[from.row][from.col];
    grid[from.row][from.col] = nullptr;

    // move rook
    if (to.col == 6) { // kingside
        grid[row][5] = grid[row][7];
        grid[row][7] = nullptr;
    }
    else if (to.col == 2) { // queenside
        grid[row][3] = grid[row][0];
        grid[row][0] = nullptr;
    }
}

void Board::updateMovedFlag(Piece* piece) {
    King* king = dynamic_cast<King*>(piece);
    if (king) king->setHasMoved(true);

    Rook* rook = dynamic_cast<Rook*>(piece);
    if (rook) rook->setHasMoved(true);
}




// En Passant func

bool Board::isEnPassantMove(Position from, Position to) {
    Piece* piece = getPiece(from);
    Pawn* pawn = dynamic_cast<Pawn*>(piece);

    if (!pawn) return false;

    return (hasEnPassant && (to == enPassantTarget));
}

void Board::performEnPassant(Position from, Position to) {
    Piece* moving = grid[from.row][from.col];

    // move pawn
    grid[to.row][to.col] = moving;
    grid[from.row][from.col] = nullptr;

    // xoá pawn bị ăn
    int capturedRow = (moving->getColor()) ? to.row + 1 : to.row - 1;

    delete grid[capturedRow][to.col];
    grid[capturedRow][to.col] = nullptr;
}

bool Board::canEnPassant(Position to) const {
    return hasEnPassant && (to == enPassantTarget);
}


void Board::movePiece(Position from, Position to) {
    Piece* moving = grid[from.row][from.col];

    // nếu là castling
    if (isCastlingMove(from, to)) {
        performCastling(from, to);
    }
    // nếu là en passant
    if (isEnPassantMove(from, to)) {
        performEnPassant(from, to);
    }

    else {
        performNormalMove(from, to);
    }
    // update flag hasMoved của King và Rook
    updateMovedFlag(moving);

    // update hasEnPassant và ô target En Passatn cho lượt kế tiếp
    Pawn* pawn = dynamic_cast<Pawn*>(moving);
    if (pawn && abs(to.row - from.row) == 2) {
        hasEnPassant = true;

        // ô mà đối phương có thể ăn
        enPassantTarget = Position((from.row + to.row) / 2, from.col);
    }
    else {
        hasEnPassant = false;
    }

}


void Board::print() {
    cout << "\n";

    for (int i = 0; i < 8; i++) {
        cout << 8 - i << " ";

        for (int j = 0; j < 8; j++) {
            if (grid[i][j] == nullptr)
                cout << ". ";
            else
                cout << grid[i][j]->getSymbol() << " ";
        }
        cout << endl;
    }

    cout << "  a b c d e f g h\n\n";
}


Board::~Board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            delete grid[i][j];
        }
    }
}

