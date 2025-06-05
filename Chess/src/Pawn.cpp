#include "../include/Pawn.h"
#include "../include/Board.h"
#include <cmath>

bool Pawn::canMove(const Board& board, Position src, Position dest) const {
    int direction = (color == Color::WHITE ? 1 : -1);
    int startRow = (color == Color::WHITE ? 1 : 6);
    int dr = dest.first - src.first;
    int dc = std::abs(dest.second - src.second);
    Piece* target = board.getPieceAt(dest);

    // Forward move (one step)
    if (dc == 0) {
        if (dr == direction && !target) {
            return true;
        }
        // Two-step opening move (only from starting row)
        if (src.first == startRow && dr == 2 * direction) {
            Position mid{ src.first + direction, src.second };
            if (!board.getPieceAt(mid) && !target) {
                return true;
            }
        }
    }
    // Diagonal capture move
    if (dc == 1 && dr == direction && target && target->getColor() != color) {
        return true;
    }
    return false;
}