#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece {
public:
    King(Color c) : Piece(c) {}
    bool canMove(const Board& board, Position src, Position dest) const override;
    char symbol() const override { return (color == Color::WHITE ? 'K' : 'k'); }
};

#endif // KING_H