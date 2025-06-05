#include "../include/King.h"
#include "../include/Board.h"
#include <cmath>

bool King::canMove(const Board& board, Position src, Position dest) const {
    int dr = std::abs(dest.first - src.first);
    int dc = std::abs(dest.second - src.second);
    if ((dr <= 1 && dc <= 1) && !(dr == 0 && dc == 0)) {
        Piece* target = board.getPieceAt(dest);
        if (target && target->getColor() == color) return false;
        return true;
    }
    return false;
}