
#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include <string>
#include <vector>
#include "Piece.h"

class Board {
private:
    // 8×8 grid storing nullptr or unique_ptr to Piece
    std::unique_ptr<Piece> grid[8][8];
public:
    Board();
    ~Board() = default;

    // Initialize from a 64-character string (rows 0–7, cols 0–7)
    void initializeFromString(const std::string& str);

    // Return pointer to piece at pos, or nullptr if empty
    Piece* getPieceAt(Position pos) const;

    // Move piece (assumed legal); source cell becomes empty
    void movePiece(Position src, Position dest);

    // Check if straight-line path from src to dest is clear (excluding endpoints)
    bool isPathClear(Position src, Position dest) const;

    // Is pos within [0..7] × [0..7]?
    static bool inBounds(Position pos);

    // Locate the king of color c; returns {-1,-1} if not found
    Position findKing(Color c) const;

    // Is color c currently in check?
    bool isInCheck(Color c) const;

    // Deep copy board (all pieces) for simulation
    std::unique_ptr<Board> clone() const;

    // Generate all possible target positions for piece at src
    std::vector<Position> generateMoves(Position src) const;
};

#endif // BOARD_H
