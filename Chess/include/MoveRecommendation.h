#ifndef MOVERECOMMENDATION_H
#define MOVERECOMMENDATION_H
#include "Board.h"
#include "PriorityQueue.h"
#include <vector>
#include <atomic>
#include <mutex>

struct MoveCandidate {
    Position src;
    Position dest;
    int score;
};

struct MoveComparator {
    int operator()(const MoveCandidate& a, const MoveCandidate& b) const {
        return a.score - b.score;
    }
};

std::vector<MoveCandidate> recommendMovesParallel(
    Board& board,
    bool isWhiteTurn,
    int searchDepth,
    int topN,
    int threadCount,
    int earlyThreshold = -1
);


#endif // MOVERECOMMENDATION_H