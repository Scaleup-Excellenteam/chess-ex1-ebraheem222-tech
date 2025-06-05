#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(Color c) : Piece(c) {}
    bool canMove(const Board& board, Position src, Position dest) const override;
    char symbol() const override { return (color == Color::WHITE ? 'N' : 'n'); }
};

#endif // KNIGHT_H