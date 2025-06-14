#include "../include/Bishop.h"
#include "../include/Board.h"
#include <cmath>

bool Bishop::canMove(const Board& board, Position src, Position dest) const {
    int dr = std::abs(dest.first - src.first);
    int dc = std::abs(dest.second - src.second);
    if (dr != dc || dr == 0) return false;
    if (!board.isPathClear(src, dest)) return false;
    Piece* target = board.getPieceAt(dest);
    if (target && target->getColor() == color) return false;
    return true;
}