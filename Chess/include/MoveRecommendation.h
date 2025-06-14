#ifndef MOVERECOMMENDATION_H
#define MOVERECOMMENDATION_H

#include "Board.h"
#include "PriorityQueue.h"
#include <vector>
#include <string>

class PromotionException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid promotion piece chosen";
    }
};

struct MoveCandidate {
    Position src;
    Position dest;
    int score;

    friend std::ostream& operator<<(std::ostream& os, const MoveCandidate& m) {
        char file1 = 'a' + m.src.first;
        char rank1 = '1' + m.src.second;
        char file2 = 'a' + m.dest.first;
        char rank2 = '1' + m.dest.second;
        os << file1 << rank1 << "->" << file2 << rank2 << " (" << m.score << ")";
        return os;
    }
};

struct MoveComparator {
    int operator()(const MoveCandidate& a, const MoveCandidate& b) const {
        return a.score - b.score;
    }
};

std::vector<std::pair<Position, Position>> allPossibleMoves(const Board& board, bool whiteTurn);


int calculateCoverage(const Board& board, bool whiteTurn);

static const Position CENTER_SQUARES[] = {
    {3, 4},
    {3, 3},
    {4, 4},
    {4, 3}
};

int evaluateMove(Board& board,
                 Position src,
                 Position dest,
                 bool isWhiteTurn,
                 int depth);

std::vector<MoveCandidate> recommendMoves(Board& board,
                                          bool isWhiteTurn,
                                          int searchDepth,
                                          int topN);

#endif // MOVERECOMMENDATION_H