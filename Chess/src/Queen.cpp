#include "../include/Queen.h"
#include "../include/Board.h"
#include <cmath>

bool Queen::canMove(const Board& board, Position src, Position dest) const {
    int dr = std::abs(dest.first - src.first);
    int dc = std::abs(dest.second - src.second);
    // Queen moves like Rook (straight) or Bishop (diagonal)
    bool straight = (src.first == dest.first || src.second == dest.second);
    bool diagonal = (dr == dc && dr != 0);
    if (!straight && !diagonal) return false;
    // Check for any pieces blocking the path
    if (!board.isPathClear(src, dest)) return false;
    // Cannot capture your own piece
    Piece* target = board.getPieceAt(dest);
    if (target && target->getColor() == color) return false;
    return true;
}