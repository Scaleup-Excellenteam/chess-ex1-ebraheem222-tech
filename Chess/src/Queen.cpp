#include "../include/Queen.h"
#include "../include/Board.h"
#include <cmath>

bool Queen::canMove(const Board& board, Position src, Position dest) const {
    int dr = std::abs(dest.first - src.first);
    int dc = std::abs(dest.second - src.second);
    bool straight = (src.first == dest.first || src.second == dest.second);
    bool diagonal = (dr == dc && dr != 0);
    if (!straight && !diagonal) return false;
    if (!board.isPathClear(src, dest)) return false;
    Piece* target = board.getPieceAt(dest);
    if (target && target->getColor() == color) return false;
    return true;
}