#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c) {}
    bool canMove(const Board& board, Position src, Position dest) const override;
    char symbol() const override { return (color == Color::WHITE ? 'B' : 'b'); }
};

#endif // BISHOP_H