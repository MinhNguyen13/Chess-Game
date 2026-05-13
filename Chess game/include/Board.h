#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"

/**
 * @file Board.h
 * @brief Quản lý bàn cờ, trạng thái ván cờ và toàn bộ logic di chuyển.
 *
 * Class Board chứa lưới 8×8 các con trỏ Piece, đồng thời cung cấp
 * tất cả các hàm kiểm tra tính hợp lệ của nước đi, phát hiện trạng thái
 * chiếu/chiếu hết/hòa cờ, cũng như xử lý các luật đặc biệt:
 * nhập thành (castling), phong tốt (promotion), bắt tốt qua đường (en passant).
 */
class Board
{
private:
    static const int szBoard = 8; ///< Kích thước bàn cờ (8×8).
    Piece* grid[szBoard][szBoard]; ///< Lưới các quân cờ; nullptr nếu ô trống.

    Position enPassantTarget; ///< Ô mục tiêu hợp lệ cho en passant ở lượt tiếp theo.
    bool hasEnPassant = false; ///< Có en passant khả dụng cho lượt kế không.

public:
    /**
     * @brief Khởi tạo Board và sắp xếp quân cờ về vị trí ban đầu.
     */
    Board();

    /**
     * @brief Đặt lại toàn bộ bàn cờ về vị trí ban đầu của ván mới.
     *
     * Giải phóng toàn bộ quân cờ hiện có, sau đó tạo và đặt lại
     * từng quân theo đúng vị trí chuẩn của cờ vua.
     */
    void initialize();

    // ─── Truy cập ô ────────────────────────────────────────────────────────────

    /**
     * @brief Lấy con trỏ đến quân cờ tại vị trí cho trước.
     * @param pos Vị trí cần truy vấn.
     * @return Con trỏ tới Piece, hoặc nullptr nếu ô trống.
     */
    Piece* getPiece(Position pos);

    /**
     * @brief Kiểm tra ô có đang trống không.
     * @param pos Vị trí cần kiểm tra.
     * @return true nếu không có quân nào tại pos.
     */
    bool isEmpty(Position pos);

    /**
     * @brief Kiểm tra vị trí có nằm trong bàn cờ không.
     * @param pos Vị trí cần kiểm tra.
     * @return true nếu row và col đều trong khoảng [0, 7].
     */
    bool isInside(Position pos);

    // ─── Phát hiện chiếu / chiếu hết / hòa ────────────────────────────────────

    /**
     * @brief Tìm vị trí vua của bên cho trước.
     * @param isWhite true để tìm vua trắng, false để tìm vua đen.
     * @return Position của vua; Position(-1, -1) nếu không tìm thấy.
     */
    Position findKing(bool isWhite);

    /**
     * @brief Kiểm tra vua của bên cho trước có đang bị chiếu không.
     * @param isWhite Màu của bên cần kiểm tra.
     * @return true nếu vua đang bị chiếu bởi ít nhất một quân đối phương.
     */
    bool isCheck(bool isWhite);

    /**
     * @brief Kiểm tra xem nước đi giả định có khiến vua bị chiếu không.
     *
     * Thực hiện di chuyển tạm thời trên lưới, kiểm tra isCheck(), rồi khôi phục
     * bàn cờ về trạng thái ban đầu. Không thay đổi trạng thái bàn cờ thực sự.
     *
     * @param from Vị trí xuất phát của quân di chuyển.
     * @param to   Vị trí đích đến.
     * @param color Màu của bên đang di chuyển (bên cần bảo vệ vua).
     * @return true nếu sau nước đi giả định, vua của `color` bị chiếu.
     */
    bool wouldBeInCheck(Position from, Position to, bool color);

    /**
     * @brief Xác định nước đi có hợp lệ theo toàn bộ luật cờ vua không.
     *
     * Kiểm tra theo thứ tự: đúng lượt, không đứng yên, luật di chuyển của quân,
     * luật nhập thành (nếu có), và không tự đưa vua vào thế chiếu.
     *
     * @param from        Vị trí quân cần di chuyển.
     * @param to          Vị trí đích.
     * @param isWhiteTurn Lượt của bên nào đang chơi.
     * @return true nếu nước đi hoàn toàn hợp lệ.
     */
    bool isLegalMove(Position from, Position to, bool isWhiteTurn);

    /**
     * @brief Kiểm tra bên cho trước có đang bị chiếu hết không.
     *
     * Điều kiện: đang bị chiếu VÀ không còn nước đi hợp lệ nào.
     *
     * @param isWhite Màu bên cần kiểm tra.
     * @return true nếu bên đó đang bị chiếu hết (thua ván).
     */
    bool isCheckmate(bool isWhite);

    /**
     * @brief Kiểm tra bên cho trước có đang bị hòa cờ (stalemate) không.
     *
     * Điều kiện: không bị chiếu VÀ không còn nước đi hợp lệ nào.
     *
     * @param isWhite Màu bên cần kiểm tra.
     * @return true nếu ván cờ kết thúc hòa do stalemate.
     */
    bool isStalemate(bool isWhite);

    // ─── Di chuyển thông thường ────────────────────────────────────────────────

    /**
     * @brief Thực hiện một nước đi thông thường (không phải nhập thành / en passant).
     *
     * Dịch chuyển quân từ `from` sang `to` và đặt ô `from` thành nullptr.
     * Quân đang đứng tại `to` (nếu có) sẽ bị ghi đè mà không giải phóng bộ nhớ —
     * hãy gọi hàm này sau khi đã xử lý capture nếu cần.
     *
     * @param from Vị trí nguồn.
     * @param to   Vị trí đích.
     */
    void performNormalMove(Position from, Position to);

    /**
     * @brief Cập nhật cờ `hasMoved` cho Vua hoặc Xe sau khi di chuyển.
     *
     * Dùng dynamic_cast để xác định loại quân; chỉ tác động đến King và Rook.
     *
     * @param piece Con trỏ quân vừa được di chuyển.
     */
    void updateMovedFlag(Piece* piece);

    // ─── Phong tốt (Promotion) ─────────────────────────────────────────────────

    /**
     * @brief Kiểm tra ô có phải là ô phong cấp cho tốt đang đứng ở đó không.
     *
     * Tốt trắng phong cấp khi lên hàng 0; tốt đen phong cấp khi xuống hàng 7.
     *
     * @param pos Vị trí cần kiểm tra (phải có Pawn đứng ở đây).
     * @return true nếu quân tại `pos` là Pawn và đang ở hàng phong cấp.
     */
    bool isPromotionSquare(Position pos);

    /**
     * @brief Thực hiện phong tốt: thay Pawn bằng quân mới do người chơi chọn.
     *
     * Giải phóng Pawn hiện tại và thay bằng Queen, Rook, Bishop hoặc Knight
     * tùy theo `choice`. Mặc định là Queen nếu ký tự không hợp lệ.
     *
     * @param pos    Vị trí ô phong cấp.
     * @param choice Ký tự đại diện quân mới: 'Q', 'R', 'B', hoặc 'N' (không phân biệt hoa/thường).
     */
    void handlePromotion(Position pos, char choice);

    // ─── Nhập thành (Castling) ─────────────────────────────────────────────────

    /**
     * @brief Kiểm tra nước đi có phải là ý định nhập thành không.
     *
     * Phát hiện bằng cách kiểm tra quân từ `from` là King và di chuyển
     * đúng 2 cột sang ngang.
     *
     * @param from Vị trí của quân (dự kiến là King).
     * @param to   Vị trí đích.
     * @return true nếu đây là ý định nhập thành.
     */
    bool isCastlingMove(Position from, Position to);

    /**
     * @brief Kiểm tra nhập thành có hợp lệ theo đầy đủ các quy tắc không.
     *
     * Kiểm tra: vua không đang bị chiếu, xe chưa di chuyển, đường giữa trống,
     * và vua không đi qua hoặc đứng vào ô bị chiếu.
     *
     * @param from    Vị trí hiện tại của Vua.
     * @param to      Vị trí đích (cột 6 = kingside, cột 2 = queenside).
     * @param isWhite Màu bên đang nhập thành.
     * @return true nếu nhập thành hợp lệ.
     */
    bool canCastle(Position from, Position to, bool isWhite);

    /**
     * @brief Thực hiện nhập thành: di chuyển cả Vua lẫn Xe cùng lúc.
     *
     * Với kingside: Vua → cột 6, Xe → cột 5.
     * Với queenside: Vua → cột 2, Xe → cột 3.
     *
     * @param from Vị trí Vua trước khi nhập thành.
     * @param to   Vị trí Vua sau khi nhập thành.
     */
    void performCastling(Position from, Position to);

    // ─── Bắt tốt qua đường (En Passant) ───────────────────────────────────────

    /**
     * @brief Kiểm tra nước đi có phải là en passant không.
     *
     * Chỉ hợp lệ khi quân là Pawn, `hasEnPassant` đang bật, và
     * ô đích trùng với `enPassantTarget`.
     *
     * @param from Vị trí Pawn đang di chuyển.
     * @param to   Vị trí đích.
     * @return true nếu đây là nước bắt tốt qua đường hợp lệ.
     */
    bool isEnPassantMove(Position from, Position to);

    /**
     * @brief Thực hiện bắt tốt qua đường.
     *
     * Di chuyển Pawn tới `to` và xóa (delete) Pawn đối phương
     * đang đứng ở hàng ngang với Pawn vừa di chuyển.
     *
     * @param from Vị trí Pawn đang bắt.
     * @param to   Ô đích (vị trí "qua đường").
     */
    void performEnPassant(Position from, Position to);

    /**
     * @brief Kiểm tra ô đích có phải là ô en passant khả dụng không (const).
     * @param to Vị trí cần kiểm tra.
     * @return true nếu `hasEnPassant` đang bật và `to` trùng `enPassantTarget`.
     */
    bool canEnPassant(Position to) const;

    // ─── Giao diện chung ───────────────────────────────────────────────────────

    /**
     * @brief In bàn cờ ra console theo định dạng ASCII.
     *
     * Hiển thị ký hiệu từng quân (hoa = trắng, thường = đen), dấu '.' cho ô trống,
     * cùng nhãn hàng (1–8) và cột (a–h).
     */
    void print();

    /**
     * @brief Di chuyển quân từ `from` sang `to`, tự động phân loại kiểu di chuyển.
     *
     * Xử lý theo ưu tiên: nhập thành → en passant → di chuyển thông thường.
     * Sau đó cập nhật cờ `hasMoved` và trạng thái en passant cho lượt kế tiếp.
     *
     * @param from Vị trí nguồn.
     * @param to   Vị trí đích.
     */
    void movePiece(Position from, Position to);

    /**
     * @brief Giải phóng toàn bộ quân cờ còn lại trên bàn.
     */
    ~Board();
};

#endif // BOARD_H