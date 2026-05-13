#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Position.h"
#include <string>

/**
 * @file Game.h
 * @brief Điều phối vòng lặp trò chơi và xử lý input từ người chơi.
 *
 * Class Game đóng vai trò controller: nó sở hữu một Board, theo dõi
 * lượt chơi, phân tích chuỗi nước đi nhập từ bàn phím, và kiểm tra
 * kết quả sau mỗi nước (chiếu hết, hòa cờ, phong tốt).
 */
class Game {
private:
    Board board;        ///< Bàn cờ hiện tại của ván đấu.
    bool isWhiteTurn;   ///< true nếu đang là lượt của bên trắng.

public:
    /**
     * @brief Khởi tạo Game với bàn cờ ở vị trí ban đầu và lượt trắng đi trước.
     */
    Game();

    /**
     * @brief Bắt đầu vòng lặp chơi trên console (chế độ text).
     *
     * In bàn cờ, nhận input "from to" từ stdin (ví dụ "e2 e4"),
     * gọi processMove() và chuyển lượt cho đến khi ván kết thúc.
     */
    void start();

    /**
     * @brief Xử lý một nước đi được biểu diễn dưới dạng chuỗi 4 ký tự.
     *
     * Chuỗi `move` có dạng "e2e4": hai ký tự đầu là ô xuất phát,
     * hai ký tự cuối là ô đích. Hàm kiểm tra tính hợp lệ, thực hiện
     * di chuyển, xử lý phong tốt (nếu có), rồi kiểm tra chiếu hết / hòa.
     *
     * @param move Chuỗi nước đi 4 ký tự theo chuẩn algebraic rút gọn (ví dụ "e2e4").
     * @return true nếu nước đi hợp lệ và đã được thực hiện thành công.
     */
    bool processMove(std::string move);

    /**
     * @brief Chuyển đổi chuỗi ký hiệu ô cờ thành đối tượng Position.
     *
     * Ký hiệu đầu vào theo chuẩn algebraic: cột a–h và hàng 1–8.
     * Ví dụ: "e2" → Position(6, 4) trong hệ tọa độ lưới 0-based (hàng 0 = hàng 8).
     *
     * @param s Chuỗi 2 ký tự biểu diễn ô cờ (ví dụ "e2", "h8").
     * @return Position tương ứng trong lưới bàn cờ nội bộ.
     */
    Position parsePosition(std::string s);

    // ─── Getters / Setters ────────────────────────────────────────────────────

    /**
     * @brief Trả về tham chiếu đến Board nội bộ.
     * @return Tham chiếu tới Board hiện tại.
     */
    Board& getBoard() { return board; }

    /**
     * @brief Trả về tham chiếu đến cờ lượt chơi.
     * @return Tham chiếu tới isWhiteTurn (có thể đọc và ghi trực tiếp).
     */
    bool& getIsWhiteTurn() { return isWhiteTurn; }

    /**
     * @brief Đặt lại ván cờ về trạng thái ban đầu.
     *
     * Khởi tạo lại bàn cờ và đặt lại lượt chơi về trắng đi trước.
     */
    void reset() { board.initialize(); isWhiteTurn = true; }

    // ─── Promotion ────────────────────────────────────────────────────────────

    /**
     * @brief Hỏi người chơi chọn quân phong tốt trên console.
     *
     * Hiển thị prompt "Promote to (Q/R/B/N): " và đọc một ký tự từ stdin.
     *
     * @return Ký tự người chơi nhập ('Q', 'R', 'B', hoặc 'N').
     */
    char askPromotionChoice();
};

#endif