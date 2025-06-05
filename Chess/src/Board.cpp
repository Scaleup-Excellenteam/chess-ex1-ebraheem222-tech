
#include "../include/Board.h"
#include "../include/Rook.h"
#include "../include/Bishop.h"
#include "../include/Knight.h"
#include "../include/Queen.h"
#include "../include/King.h"
#include "../include/Pawn.h"
#include <stdexcept>
#include <cctype>
#include <cmath>

Board::Board() {}

void Board::initializeFromString(const std::string& str) {
    if (str.size() != 64)
        throw std::invalid_argument("Invalid start string length");//this if length string not eqaul 64 mean not suited to Chess 8x8

    for (int idx = 0; idx < 64; ++idx) {
        int row = idx / 8;
        int col = idx % 8;
        char c = str[idx];

        if (c == '#') {
            grid[row][col] = nullptr;
        } else {
            Color clr = (std::isupper(c) ? Color::WHITE : Color::BLACK);
            char lower = std::tolower(c);
            switch (lower) {
                case 'r': grid[row][col] = std::make_unique<Rook>(clr);   break;
                case 'n': grid[row][col] = std::make_unique<Knight>(clr); break;
                case 'b': grid[row][col] = std::make_unique<Bishop>(clr); break;
                case 'q': grid[row][col] = std::make_unique<Queen>(clr);  break;
                case 'k': grid[row][col] = std::make_unique<King>(clr);   break;
                case 'p': grid[row][col] = std::make_unique<Pawn>(clr);   break;
                default:  throw std::invalid_argument("Unknown piece char");
            }
        }
    }
}

Piece* Board::getPieceAt(Position pos) const {
    if (!inBounds(pos)) return nullptr;
    return grid[pos.first][pos.second].get();
}

void Board::movePiece(Position src, Position dest) {
    grid[dest.first][dest.second] = std::move(grid[src.first][src.second]);
}

bool Board::inBounds(Position pos) {
    return pos.first >= 0 && pos.first < 8 && pos.second >= 0 && pos.second < 8;
}

bool Board::isPathClear(Position src, Position dest) const {
    int dr = dest.first - src.first;
    int dc = dest.second - src.second;
    int stepR = (dr == 0 ? 0 : (dr > 0 ? 1 : -1));
    int stepC = (dc == 0 ? 0 : (dc > 0 ? 1 : -1));
    int r = src.first + stepR;
    int c = src.second + stepC;

    while (r != dest.first || c != dest.second) {
        if (grid[r][c]) return false;
        r += stepR;
        c += stepC;
    }
    return true;
}

Position Board::findKing(Color c) const {
    for (int r = 0; r < 8; ++r) {
        for (int col = 0; col < 8; ++col) {
            Piece* p = grid[r][col].get();
            if (p && p->symbol() == (c == Color::WHITE ? 'K' : 'k'))
                return {r, col};
        }
    }
    return {-1, -1};
}

bool Board::isInCheck(Color c) const {
    Position kingPos = findKing(c);
    if (kingPos.first < 0) return false;

    Color opp = (c == Color::WHITE ? Color::BLACK : Color::WHITE);
    for (int r = 0; r < 8; ++r) {
        for (int col = 0; col < 8; ++col) {
            Piece* p = grid[r][col].get();
            if (p && p->getColor() == opp) {
                Position src{r, col};
                if (p->canMove(*this, src, kingPos))
                    return true;
            }
        }
    }
    return false;
}

std::unique_ptr<Board> Board::clone() const {
    auto newBoard = std::make_unique<Board>();
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (grid[r][c]) {
                char sym = grid[r][c]->symbol();
                Color clr = grid[r][c]->getColor();
                switch (std::tolower(sym)) {
                    case 'r': newBoard->grid[r][c] = std::make_unique<Rook>(clr);   break;
                    case 'n': newBoard->grid[r][c] = std::make_unique<Knight>(clr); break;
                    case 'b': newBoard->grid[r][c] = std::make_unique<Bishop>(clr); break;
                    case 'q': newBoard->grid[r][c] = std::make_unique<Queen>(clr);  break;
                    case 'k': newBoard->grid[r][c] = std::make_unique<King>(clr);   break;
                    case 'p': newBoard->grid[r][c] = std::make_unique<Pawn>(clr);   break;
                }
            }
        }
    }
    return newBoard;
}

// Generate all target positions for piece at src
std::vector<Position> Board::generateMoves(Position src) const {
    std::vector<Position> targets;
    Piece* p = getPieceAt(src);
    if (!p) return targets;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Position dest{r, c};
            Piece* destPiece = getPieceAt(dest);
            if (destPiece && destPiece->getColor() == p->getColor()) continue;
            if (p->canMove(*this, src, dest)) {
                targets.push_back(dest);
            }
        }
    }
    return targets;
}
