#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c) {}
    bool canMove(const Board& board, Position src, Position dest) const override;
    char symbol() const override { return (color == Color::WHITE ? 'P' : 'p'); }
};

#endif // PAWN_H