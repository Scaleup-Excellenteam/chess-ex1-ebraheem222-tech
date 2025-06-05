#include "../include/MoveRecommendation.h"
#include "../include/ThreadPool.h"
#include <algorithm>
#include <limits>
#include <iostream>

static int pieceValue(char sym) {
    switch (std::tolower(sym)) {
        case 'p': return 1;
        case 'n': return 3;
        case 'b': return 3;
        case 'r': return 5;
        case 'q': return 9;
        case 'k': return 100;
    }
    return 0;
}

static int evaluateMove(Board& board, Position src, Position dest, bool isWhiteTurn, int depth);

static std::vector<std::pair<Position, Position>> allPossibleMoves(const Board& board, bool whiteTurn) {
    std::vector<std::pair<Position, Position>> moves;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Position src{r,c};
            Piece* p = board.getPieceAt(src);
            if (p && ((p->getColor() == Color::WHITE) == whiteTurn)) {
                auto targets = board.generateMoves(src);
                for (auto& dest : targets) {
                    moves.emplace_back(src, dest);
                }
            }
        }
    }
    return moves;
}

std::vector<MoveCandidate> recommendMovesParallel(
    Board& board,
    bool isWhiteTurn,
    int searchDepth,
    int topN,
    int threadCount,
    int earlyThreshold
) {
    if (threadCount <= 1) {
        PriorityQueue<MoveCandidate, MoveComparator> pq(topN);
        auto moves = allPossibleMoves(board, isWhiteTurn);
        for (auto& mv : moves) {
            int sc = evaluateMove(board, mv.first, mv.second, isWhiteTurn, searchDepth);
            MoveCandidate cand{ mv.first, mv.second, sc };
            pq.push(cand);
        }
        std::vector<MoveCandidate> out;
        while (!pq.empty()) {
            out.push_back(pq.poll());
        }
        std::reverse(out.begin(), out.end());
        if ((int)out.size() > topN) out.resize(topN);
        return out;
    }

    auto allMoves = allPossibleMoves(board, isWhiteTurn);
    int totalMoves = (int)allMoves.size();
    if (totalMoves == 0) {
        return {};
    }

    int chunkSize = (totalMoves + threadCount - 1) / threadCount; // ceil division

    std::mutex globalPqMutex;
    PriorityQueue<MoveCandidate, MoveComparator> globalPq(topN);
    std::atomic<bool> stopEarly(false);

    ThreadPool pool(threadCount);

    for (int t = 0; t < threadCount; ++t) {
        int startIdx = t * chunkSize;
        int endIdx = std::min(startIdx + chunkSize, totalMoves);

        if (startIdx >= endIdx) break;

        pool.enqueue([=, &board, &globalPq, &globalPqMutex, &stopEarly]() mutable {
            PriorityQueue<MoveCandidate, MoveComparator> localPq(topN);

            for (int idx = startIdx; idx < endIdx; ++idx) {
                if (stopEarly.load()) {
                    break;
                }

                Position src = allMoves[idx].first;
                Position dest = allMoves[idx].second;

                int sc = evaluateMove(board, src, dest, isWhiteTurn, searchDepth);
                MoveCandidate cand{ src, dest, sc };

                if (earlyThreshold >= 0 && sc >= earlyThreshold) {
                    stopEarly.store(true);
                }

                localPq.push(cand);
            }

            std::vector<MoveCandidate> buffer;
            while (!localPq.empty()) {
                buffer.push_back(localPq.poll());
            }
            std::reverse(buffer.begin(), buffer.end());

            std::lock_guard<std::mutex> lock(globalPqMutex);
            for (auto& c : buffer) {
                globalPq.push(c);
            }
        });
    }

    pool.shutdown();

    std::vector<MoveCandidate> result;
    while (!globalPq.empty()) {
        result.push_back(globalPq.poll());
    }
    std::reverse(result.begin(), result.end());
    if ((int)result.size() > topN) result.resize(topN);
    return result;
}


static bool controlsCenter(Position finalDest) {
    static const Position CENTER_SQUARES[] = {
        {3,4}, {3,3}, {4,4}, {4,3}
    };
    for (auto& c : CENTER_SQUARES) {
        if (c == finalDest) return true;
    }
    return false;
}
static int calculateCoverage(const Board& board, bool whiteTurn) {
    std::vector<std::vector<bool>> covered(8, std::vector<bool>(8, false));
    int count = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Position src{r,c};
            Piece* p = board.getPieceAt(src);
            if (p && ((p->getColor() == Color::WHITE) == whiteTurn)) {
                auto targets = board.generateMoves(src);
                for (auto& dest : targets) {
                    if (!covered[dest.first][dest.second]) {
                        covered[dest.first][dest.second] = true;
                        ++count;
                    }
                }
            }
        }
    }
    return count;
}

static int evaluateMove(Board& board, Position src, Position dest, bool isWhiteTurn, int depth) {
    auto testBoard = board.clone();
    Piece* moving = testBoard->getPieceAt(src);
    Piece* capture = testBoard->getPieceAt(dest);
    int score = 0;

    if (capture) {
        score += pieceValue(capture->symbol());
    }
    testBoard->movePiece(src, dest);

    if (controlsCenter(dest)) {
        score += 2;
    }

    int myCov  = calculateCoverage(*testBoard, isWhiteTurn);
    int oppCov = calculateCoverage(*testBoard, !isWhiteTurn);
    score += (myCov - oppCov);

    if (depth <= 0) {
        return score;
    }

    // find best reply (minimize our score)
    int oppBest = std::numeric_limits<int>::max();
    auto oppMoves = allPossibleMoves(*testBoard, !isWhiteTurn);
    if (oppMoves.empty()) {
        return score;
    }
    for (auto& omv : oppMoves) {
        int val = evaluateMove(*testBoard, omv.first, omv.second, !isWhiteTurn, depth - 1);
        oppBest = std::min(oppBest, val);
    }
    score -= oppBest;
    return score;
}