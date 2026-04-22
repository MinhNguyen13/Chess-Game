#pragma once

class Position {
public:
    int row, col;

    Position(int r = 0, int c = 0) {
        row = r;
        col = c;
    }
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};
