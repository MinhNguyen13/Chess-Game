#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Position.h"
#include <string>

class Game {
private:
    Board board;
    bool isWhiteTurn;

public:
    Game();

    void start();
    bool processMove(std::string move);
    Position parsePosition(std::string s);

    char askPromotionChoice();
};

#endif
