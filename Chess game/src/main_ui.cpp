#include "raylib.h"
#include "Game.h"
#include "Board.h"
#include "Piece.h"
#include "Position.h"

#include <string>
#include <vector>

// ===================== CẤU HÌNH =====================
const int CELL = 80;           // kích thước 1 ô cờ (px)
const int BOARD_SIZE = CELL * 8;
const int SIDEBAR = 260;
const int WIN_W = BOARD_SIZE + SIDEBAR;
const int WIN_H = BOARD_SIZE + 60; // thêm 60px cho label hàng/cột

const int BOARD_X = 0;        // bàn cờ bắt đầu từ x=0
const int BOARD_Y = 30;       // bắt đầu từ y=30 (để có label cột a-h)

// ===================== MÀU SẮC =====================
const Color CLR_LIGHT      = { 240, 217, 181, 255 }; // ô sáng
const Color CLR_DARK       = { 181, 136, 99,  255 }; // ô tối
const Color CLR_SELECTED   = { 247, 236, 81,  200 }; // ô được chọn
const Color CLR_LEGAL      = { 100, 200, 100, 160 }; // nước đi hợp lệ
const Color CLR_CHECK      = { 220, 50,  50,  180 }; // vua bị chiếu
const Color CLR_BG         = { 22,  22,  30,  255 }; // nền sidebar
const Color CLR_SIDEBAR_BG = { 30,  30,  42,  255 }; // sidebar
const Color CLR_TEXT       = { 240, 230, 210, 255 }; // chữ
const Color CLR_ACCENT     = { 200, 160, 80,  255 }; // accent vàng

// ===================== TRẠNG THÁI UI =====================
struct UIState {
    bool hasSelected = false;
    Position selected = {-1, -1};
    std::vector<Position> legalMoves;

    bool showPromotion = false;
    Position promotionPos = {-1, -1};

    std::string message = "";
    int messageTimer = 0;

    bool gameOver = false;
    std::string gameOverMsg = "";
};

// ===================== HÀM TIỆN ÍCH =====================

// Lấy màu ô (row, col)
Color getCellColor(int row, int col, UIState& ui, Board& board, bool isWhiteTurn) {
    Position pos(row, col);

    // ô được chọn
    if (ui.hasSelected && ui.selected.row == row && ui.selected.col == col)
        return CLR_SELECTED;

    // nước đi hợp lệ
    for (auto& m : ui.legalMoves) {
        if (m.row == row && m.col == col)
            return CLR_LEGAL;
    }

    // vua đang bị chiếu
    if (board.isCheck(isWhiteTurn)) {
        Position kingPos = board.findKing(isWhiteTurn);
        if (kingPos.row == row && kingPos.col == col)
            return CLR_CHECK;
    }

    return ((row + col) % 2 == 0) ? CLR_LIGHT : CLR_DARK;
}

// Chuyển pixel (x,y) → Position bàn cờ
Position pixelToPos(int px, int py) {
    int col = (px - BOARD_X) / CELL;
    int row = (py - BOARD_Y) / CELL;
    return Position(row, col);
}

// Lấy tên file ảnh theo symbol quân cờ
std::string getTextureName(char symbol) {
    switch (symbol) {
        case 'K': return "pieces_asset/wK.png";
        case 'Q': return "pieces_asset/wQ.png";
        case 'R': return "pieces_asset/wR.png";
        case 'B': return "pieces_asset/wB.png";
        case 'N': return "pieces_asset/wN.png";
        case 'P': return "pieces_asset/wP.png";
        case 'k': return "pieces_asset/bK.png";
        case 'q': return "pieces_asset/bQ.png";
        case 'r': return "pieces_asset/bR.png";
        case 'b': return "pieces_asset/bB.png";
        case 'n': return "pieces_asset/bN.png";
        case 'p': return "pieces_asset/bP.png";
        default:  return "";
    }
}

// Tính tất cả nước đi hợp lệ của quân tại 'from'
std::vector<Position> getLegalMoves(Board& board, Position from, bool isWhiteTurn) {
    std::vector<Position> moves;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Position to(r, c);
            if (board.isLegalMove(from, to, isWhiteTurn))
                moves.push_back(to);
        }
    }
    return moves;
}

// ===================== VẼ BÀN CỜ =====================
void drawBoard(Board& board, UIState& ui, Texture2D textures[12], bool isWhiteTurn) {
    // Nhãn cột (a-h)
    for (int c = 0; c < 8; c++) {
        char label[2] = { (char)('a' + c), '\0' };
        DrawText(label, BOARD_X + c * CELL + CELL / 2 - 5, 8, 18, CLR_ACCENT);
        DrawText(label, BOARD_X + c * CELL + CELL / 2 - 5, BOARD_Y + BOARD_SIZE + 6, 18, CLR_ACCENT);
    }
    // Nhãn hàng (1-8)
    for (int r = 0; r < 8; r++) {
        char label[2] = { (char)('0' + (8 - r)), '\0' };
        DrawText(label, BOARD_SIZE + 4, BOARD_Y + r * CELL + CELL / 2 - 9, 18, CLR_ACCENT);
    }

    // Vẽ các ô
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Color cellColor = getCellColor(row, col, ui, board, isWhiteTurn);
            DrawRectangle(
                BOARD_X + col * CELL,
                BOARD_Y + row * CELL,
                CELL, CELL,
                cellColor
            );

            // Chấm nhỏ cho nước đi hợp lệ (ô trống)
            for (auto& m : ui.legalMoves) {
                if (m.row == row && m.col == col && board.isEmpty(m)) {
                    DrawCircle(
                        BOARD_X + col * CELL + CELL / 2,
                        BOARD_Y + row * CELL + CELL / 2,
                        10, { 0, 0, 0, 80 }
                    );
                }
            }

            // Vẽ quân cờ
            Piece* piece = board.getPiece(Position(row, col));
            if (piece != nullptr) {
                char sym = piece->getSymbol();
                // Map symbol → index texture
                std::string symbols = "KQRBNPkqrbnp";
                int idx = (int)symbols.find(sym);
                if (idx != (int)std::string::npos && textures[idx].id != 0) {
                    // Scale ảnh vừa ô
                    Rectangle src = { 0, 0, (float)textures[idx].width, (float)textures[idx].height };
                    Rectangle dst = {
                        (float)(BOARD_X + col * CELL + 4),
                        (float)(BOARD_Y + row * CELL + 4),
                        (float)(CELL - 8),
                        (float)(CELL - 8)
                    };
                    DrawTexturePro(textures[idx], src, dst, {0,0}, 0, WHITE);
                }
            }
        }
    }

    // Viền bàn cờ
    DrawRectangleLinesEx(
        { (float)BOARD_X, (float)BOARD_Y, (float)BOARD_SIZE, (float)BOARD_SIZE },
        2, CLR_ACCENT
    );
}

// ===================== VẼ SIDEBAR =====================
void drawSidebar(bool isWhiteTurn, UIState& ui) {
    int sx = BOARD_SIZE + 2;
    DrawRectangle(sx, 0, SIDEBAR, WIN_H, CLR_SIDEBAR_BG);
    DrawLineEx({ (float)sx, 0 }, { (float)sx, (float)WIN_H }, 2, CLR_ACCENT);

    // Tiêu đề
    DrawText("CHESS", sx + 20, 20, 36, CLR_ACCENT);
    DrawLine(sx + 10, 65, sx + SIDEBAR - 10, 65, CLR_ACCENT);

    // Lượt chơi
    DrawText("TURN", sx + 20, 80, 16, { 160, 150, 130, 255 });
    const char* turnText = isWhiteTurn ? "WHITE" : "BLACK";
    Color turnColor = isWhiteTurn ? WHITE : { 150, 150, 150, 255 };
    DrawText(turnText, sx + 20, 100, 28, turnColor);

    // Chỉ báo màu lượt (hình tròn)
    Color circleColor = isWhiteTurn ? WHITE : { 40, 40, 40, 255 };
    DrawCircle(sx + SIDEBAR - 40, 114, 18, circleColor);
    DrawCircleLines(sx + SIDEBAR - 40, 114, 18, CLR_ACCENT);

    DrawLine(sx + 10, 145, sx + SIDEBAR - 10, 145, { 60, 55, 50, 255 });

    // Hướng dẫn
    DrawText("HOW TO PLAY", sx + 20, 160, 14, { 160, 150, 130, 255 });
    DrawText("Click piece to select", sx + 20, 182, 13, CLR_TEXT);
    DrawText("Click destination to move", sx + 20, 200, 13, CLR_TEXT);
    DrawText("Green dots = legal moves", sx + 20, 218, 13, CLR_TEXT);

    DrawLine(sx + 10, 245, sx + SIDEBAR - 10, 245, { 60, 55, 50, 255 });

    // Thông báo
    if (ui.messageTimer > 0) {
        float alpha = (float)ui.messageTimer / 120.0f;
        Color msgColor = { 240, 200, 80, (unsigned char)(alpha * 255) };
        DrawText(ui.message.c_str(), sx + 20, 265, 18, msgColor);
    }

    // Game over
    if (ui.gameOver) {
        DrawRectangle(sx + 10, 300, SIDEBAR - 20, 80, { 20, 15, 10, 220 });
        DrawRectangleLinesEx({ (float)(sx + 10), 300, (float)(SIDEBAR - 20), 80 }, 1, CLR_ACCENT);
        DrawText(ui.gameOverMsg.c_str(), sx + 20, 320, 16, CLR_ACCENT);
        DrawText("Press R to restart", sx + 20, 350, 13, CLR_TEXT);
    }
}

// ===================== VẼ MÀN HÌNH PROMOTION =====================
void drawPromotion(bool isWhite, Texture2D textures[12]) {
    // Overlay mờ
    DrawRectangle(0, 0, WIN_W, WIN_H, { 0, 0, 0, 160 });

    // Hộp promotion
    int bw = 4 * CELL + 20;
    int bh = CELL + 60;
    int bx = (BOARD_SIZE - bw) / 2;
    int by = WIN_H / 2 - bh / 2;

    DrawRectangleRounded({ (float)bx, (float)by, (float)bw, (float)bh }, 0.1f, 8, CLR_SIDEBAR_BG);
    DrawRectangleRoundedLines({ (float)bx, (float)by, (float)bw, (float)bh }, 0.1f, 8, CLR_ACCENT);

    DrawText("PROMOTE TO:", bx + 10, by + 8, 16, CLR_ACCENT);

    // 4 lựa chọn: Q R B N
    std::string choices = isWhite ? "QRBNqrbn" : "qrbnQRBN";
    std::string symbols = "KQRBNPkqrbnp";

    // white: Q=idx1, R=idx2, B=idx3, N=idx4
    // black: q=idx7, r=idx8, b=idx9, n=idx10
    int idxOffset = isWhite ? 0 : 6;
    int pieceIdxs[4] = { 1 + idxOffset, 2 + idxOffset, 3 + idxOffset, 4 + idxOffset };

    for (int i = 0; i < 4; i++) {
        int px = bx + 10 + i * (CELL + 5);
        int py = by + 30;

        // Highlight khi hover
        Vector2 mouse = GetMousePosition();
        bool hovered = (mouse.x >= px && mouse.x <= px + CELL &&
                        mouse.y >= py && mouse.y <= py + CELL);

        DrawRectangle(px, py, CELL, CELL, hovered ? CLR_SELECTED : CLR_LIGHT);
        DrawRectangleLines(px, py, CELL, CELL, CLR_ACCENT);

        int tidx = pieceIdxs[i];
        if (textures[tidx].id != 0) {
            Rectangle src = { 0, 0, (float)textures[tidx].width, (float)textures[tidx].height };
            Rectangle dst = { (float)(px + 4), (float)(py + 4), (float)(CELL - 8), (float)(CELL - 8) };
            DrawTexturePro(textures[tidx], src, dst, {0,0}, 0, WHITE);
        }
    }
}

// ===================== MAIN =====================
int main() {
    InitWindow(WIN_W, WIN_H, "Chess Game");
    SetTargetFPS(60);

    // Load textures (thứ tự: K Q R B N P k q r b n p)
    std::string symbols = "KQRBNPkqrbnp";
    Texture2D textures[12] = {};
    for (int i = 0; i < 12; i++) {
        std::string path = getTextureName(symbols[i]);
        textures[i] = LoadTexture(path.c_str());
    }

    Game game;
    Board& board = game.getBoard();
    bool& isWhiteTurn = game.getIsWhiteTurn();

    UIState ui;

    while (!WindowShouldClose()) {
        // ========== UPDATE ==========

        // Giảm timer thông báo
        if (ui.messageTimer > 0) ui.messageTimer--;

        // Restart
        if (ui.gameOver && IsKeyPressed(KEY_R)) {
            game.reset();
            ui = UIState();
        }

        // Xử lý click chuột
        if (!ui.gameOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            int mx = (int)mouse.x;
            int my = (int)mouse.y;

            // Click trong bàn cờ
            if (!ui.showPromotion &&
                mx >= BOARD_X && mx < BOARD_SIZE &&
                my >= BOARD_Y && my < BOARD_Y + BOARD_SIZE)
            {
                Position clicked = pixelToPos(mx, my);

                if (!ui.hasSelected) {
                    // Chọn quân
                    Piece* piece = board.getPiece(clicked);
                    if (piece != nullptr && piece->getColor() == isWhiteTurn) {
                        ui.hasSelected = true;
                        ui.selected = clicked;
                        ui.legalMoves = getLegalMoves(board, clicked, isWhiteTurn);
                    }
                } else {
                    // Đã chọn → thử di chuyển
                    bool moved = false;

                    // Click vào nước đi hợp lệ
                    for (auto& m : ui.legalMoves) {
                        if (m.row == clicked.row && m.col == clicked.col) {
                            board.movePiece(ui.selected, clicked);
                            moved = true;

                            // Kiểm tra promotion
                            if (board.isPromotionSquare(clicked)) {
                                ui.showPromotion = true;
                                ui.promotionPos = clicked;
                            } else {
                                isWhiteTurn = !isWhiteTurn;
                                // Kiểm tra check/checkmate/stalemate
                                bool opponent = isWhiteTurn;
                                if (board.isCheckmate(opponent)) {
                                    ui.gameOver = true;
                                    ui.gameOverMsg = (!opponent ? "WHITE" : "BLACK");
                                    ui.gameOverMsg += " WINS!";
                                } else if (board.isStalemate(opponent)) {
                                    ui.gameOver = true;
                                    ui.gameOverMsg = "DRAW!";
                                } else if (board.isCheck(opponent)) {
                                    ui.message = "CHECK!";
                                    ui.messageTimer = 120;
                                }
                            }
                            break;
                        }
                    }

                    if (!moved) {
                        // Click vào quân khác cùng màu → chọn lại
                        Piece* piece = board.getPiece(clicked);
                        if (piece != nullptr && piece->getColor() == isWhiteTurn) {
                            ui.selected = clicked;
                            ui.legalMoves = getLegalMoves(board, clicked, isWhiteTurn);
                        } else {
                            // Click vào chỗ khác → bỏ chọn
                            ui.hasSelected = false;
                            ui.legalMoves.clear();
                        }
                    } else if (!ui.showPromotion) {
                        ui.hasSelected = false;
                        ui.legalMoves.clear();
                    }
                }
            }

            // Xử lý click promotion
            if (ui.showPromotion) {
                int idxOffset = (!isWhiteTurn) ? 0 : 6; // lúc này isWhiteTurn đã chưa đổi
                // Thực ra màu của pawn là màu vừa đi
                bool pawnIsWhite = !isWhiteTurn; // vì chưa đổi lượt khi promotion
                // Tính vị trí các nút
                int bw = 4 * CELL + 20;
                int bx = (BOARD_SIZE - bw) / 2;
                int by = WIN_H / 2 - (CELL + 60) / 2;
                int py = by + 30;

                char promotionChoices[4] = {'Q', 'R', 'B', 'N'};
                for (int i = 0; i < 4; i++) {
                    int px = bx + 10 + i * (CELL + 5);
                    if (mx >= px && mx <= px + CELL && my >= py && my <= py + CELL) {
                        board.handlePromotion(ui.promotionPos, promotionChoices[i]);
                        ui.showPromotion = false;

                        isWhiteTurn = !isWhiteTurn;
                        bool opponent = isWhiteTurn;
                        if (board.isCheckmate(opponent)) {
                            ui.gameOver = true;
                            ui.gameOverMsg = (!opponent ? "WHITE" : "BLACK");
                            ui.gameOverMsg += " WINS!";
                        } else if (board.isStalemate(opponent)) {
                            ui.gameOver = true;
                            ui.gameOverMsg = "DRAW!";
                        } else if (board.isCheck(opponent)) {
                            ui.message = "CHECK!";
                            ui.messageTimer = 120;
                        }

                        ui.hasSelected = false;
                        ui.legalMoves.clear();
                        break;
                    }
                }
            }
        }

        // ========== DRAW ==========
        BeginDrawing();
        ClearBackground(CLR_BG);

        drawBoard(board, ui, textures, isWhiteTurn);
        drawSidebar(isWhiteTurn, ui);

        if (ui.showPromotion) {
            bool pawnIsWhite = !isWhiteTurn;
            drawPromotion(pawnIsWhite, textures);
        }

        EndDrawing();
    }

    // Cleanup
    for (int i = 0; i < 12; i++) UnloadTexture(textures[i]);
    CloseWindow();
    return 0;
}
