#pragma once
#include "Position.h"

class Board; // forward declaration

/**
 * @file Piece.h
 * @brief Lớp cơ sở trừu tượng cho tất cả các quân cờ.
 *
 * Piece định nghĩa giao diện chung mà mọi quân cờ cụ thể (Pawn, Rook,
 * Knight, Bishop, Queen, King) đều phải triển khai. Mỗi lớp con override
 * isValidMove() để thể hiện luật di chuyển riêng của mình.
 */
class Piece {
protected:
    bool isWhite; // true nếu quân thuộc bên trắng; false nếu thuộc bên đen.

public:
    /**
     * @brief Khởi tạo quân cờ với màu cho trước.
     * @param isWhite true để tạo quân trắng, false để tạo quân đen.
     */
    Piece(bool isWhite);

    /**
     * @brief Destructor ảo, đảm bảo giải phóng đúng khi dùng con trỏ đa hình.
     */
    virtual ~Piece();

    /**
     * @brief Trả về màu của quân cờ này.
     * @return true nếu quân trắng, false nếu quân đen.
     */
    bool getColor();

    /**
     * @brief Kiểm tra nước đi từ `from` đến `to` có hợp lệ theo luật của quân này không.
     *
     * Hàm thuần ảo — mỗi lớp con triển khai logic di chuyển riêng.
     * Hàm chỉ kiểm tra luật hình học của quân (đường đi, chặn đường, ăn quân...),
     * chưa tính đến việc có tự đưa vua vào thế chiếu hay không (việc đó do Board xử lý).
     *
     * @param from  Vị trí hiện tại của quân.
     * @param to    Vị trí đích muốn di chuyển đến.
     * @param board Tham chiếu đến bàn cờ hiện tại (để kiểm tra chặn đường, quân đối...).
     * @return true nếu nước đi tuân theo luật di chuyển của quân này.
     */
    virtual bool isValidMove(Position from, Position to, Board& board) = 0;

    /**
     * @brief Trả về ký tự đại diện cho quân cờ dùng khi vẽ bàn cờ.
     *
     * Quy ước: chữ hoa = quân trắng, chữ thường = quân đen.
     * Ví dụ: 'K'/'k' = Vua, 'Q'/'q' = Hậu, 'R'/'r' = Xe,
     *         'B'/'b' = Tượng, 'N'/'n' = Mã, 'P'/'p' = Tốt.
     *
     * @return Ký tự char đại diện cho loại và màu quân.
     */
    virtual char getSymbol() = 0;
};