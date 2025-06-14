#include "../include/Rook.h"
#include "../include/Board.h"

bool Rook::canMove(const Board& board, Position src, Position dest) const {
    if (src == dest) return false;
    // Must move in a straight line (same row or same column)
    if (src.first != dest.first && src.second != dest.second) return false;
    // Check for any pieces blocking the path
    if (!board.isPathClear(src, dest)) return false;
    // Cannot capture your own piece
    Piece* target = board.getPieceAt(dest);
    if (target && target->getColor() == color) return false;
    return true;
}