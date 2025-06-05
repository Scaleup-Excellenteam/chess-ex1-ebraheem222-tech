#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece {
public:
    Rook(Color c) : Piece(c) {}
    bool canMove(const Board& board, Position src, Position dest) const override;
    char symbol() const override { return (color == Color::WHITE ? 'R' : 'r'); }
};

#endif // ROOK_H