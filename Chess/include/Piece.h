
#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <utility>

// Forward declaration of Board
class Board;

enum class Color { WHITE, BLACK };

typedef std::pair<int,int> Position; // row, col (0-based)

class Piece {
protected:
    Color color;
public:
    Piece(Color c) : color(c) {}
    virtual ~Piece() = default;
    Color getColor() const { return color; }

    virtual bool canMove(const Board& board, Position src, Position dest) const = 0;
    virtual char symbol() const = 0;
};

#endif // PIECE_H