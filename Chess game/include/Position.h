#pragma once

/**
 * @brief Biểu diễn một ô trên bàn cờ bằng chỉ số hàng và cột.
 *
 * Hệ tọa độ 0-based: row 0 = hàng trên cùng (hàng 8), col 0 = cột 'a'.
 */
class Position {
public:
    int row; ///< Chỉ số hàng (0–7), 0 là hàng trên cùng.
    int col; ///< Chỉ số cột (0–7), 0 là cột 'a'.

    /**
     * @brief Khởi tạo Position với hàng và cột cho trước.
     * @param r Chỉ số hàng (mặc định 0).
     * @param c Chỉ số cột (mặc định 0).
     */
    Position(int r = 0, int c = 0) {
        row = r;
        col = c;
    }

    /**
     * @brief So sánh bằng hai Position.
     * @return true nếu cùng hàng và cùng cột.
     */
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};