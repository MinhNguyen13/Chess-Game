#include "Game.h"
#include "Board.h"
#include <iostream>

using namespace std;

Game::Game() {
    isWhiteTurn = true;
}

void Game::start() {
    cout<<"GameStart!"<<endl;
    while (true) {
        board.print();  // in bàn cờ

        cout << (isWhiteTurn ? "White" : "Black") << " move: ";

        string from, to;
        cin >> from >> to;

        if (processMove(from + to)) {
            isWhiteTurn = !isWhiteTurn;
        } else {
            cout << "Invalid move!\n";
        }
    }
}

bool Game::processMove(string move) {
    // ví dụ: e2\ne4

    Position from = parsePosition(move.substr(0, 2));
    Position to   = parsePosition(move.substr(2, 2));
    cout<<"ProcessMove!"<<endl;
    Piece* piece = board.getPiece(from);

    if (piece == nullptr) return false;

    // đúng lượt không
    if (piece->getColor() != isWhiteTurn) return false;

    // check hợp lệ
    if (!board.isLegalMove(from, to, isWhiteTurn)){
         cout<<"Illegal Move"<<endl;
         return false;
    }

    // di chuyển && xử lí castling(nếu có)
    board.movePiece(from, to);

    // Xu li Promotion
    if (board.isPromotionSquare(to)) {
        char choice = askPromotionChoice();
        board.handlePromotion(to, choice);
    }


    // Kiểm tra trạng thái đối thủ
    bool opponent = !isWhiteTurn;

    if (board.isCheckmate(opponent)) {
        board.print();
        cout << (isWhiteTurn ? "White" : "Black") << " wins by checkmate!\n";
        exit(0); // hoặc return đặc biệt
    }

    else if (board.isStalemate(opponent)) {
        board.print();
        cout << "Draw by stalemate!\n";
        exit(0);
    }

    // (tuỳ chọn)
    else if (board.isCheck(opponent)) {
        cout << "Check!\n";
    }

    return true;
}

Position Game::parsePosition(string s) {

    // ví dụ: "e2"

    int col = s[0] - 'a';        // a-h → 0-7
    int row = 8 - (s[1] - '0');  // 1-8 → 7-0

    return Position(row, col);
}

char Game::askPromotionChoice() {
    char choice;
    cout << "Promote to (Q/R/B/N): ";
    cin >> choice;
    return choice;
}
