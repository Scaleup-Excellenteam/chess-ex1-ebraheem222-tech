#include "../include/Knight.h"
#include "../include/Board.h"
#include <cmath>

bool Knight::canMove(const Board& board, Position src, Position dest) const {
    int dr = std::abs(dest.first - src.first);
    int dc = std::abs(dest.second - src.second);
    if (!((dr == 2 && dc == 1) || (dr == 1 && dc == 2))) return false;
    Piece* target = board.getPieceAt(dest);
    if (target && target->getColor() == color) return false;
    return true;
}