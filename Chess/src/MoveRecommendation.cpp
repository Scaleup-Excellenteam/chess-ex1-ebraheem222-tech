#include "../include/MoveRecommendation.h"
#include <algorithm>
#include <cctype>
#include <limits>

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

static std::unique_ptr<Board> cloneBoard(const Board& board) {
    return board.clone();
}

std::vector<std::pair<Position, Position>> allPossibleMoves(const Board& board, bool whiteTurn) {
    std::vector<std::pair<Position, Position>> moves;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Position src{r, c};
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

int calculateCoverage(const Board& board, bool whiteTurn) {
    std::vector<std::vector<bool>> covered(8, std::vector<bool>(8, false));
    int count = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Position src{r, c};
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

static bool controlsCenter(Position finalDest) {
    for (auto& c : CENTER_SQUARES) {
        if (c == finalDest) return true;
    }
    return false;
}

int evaluateMove(Board& board, Position src, Position dest, bool isWhiteTurn, int depth) {
    auto testBoard = cloneBoard(board);
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

    int myCoverage   = calculateCoverage(*testBoard, isWhiteTurn);
    int oppCoverage  = calculateCoverage(*testBoard, !isWhiteTurn);
    score += (myCoverage - oppCoverage); 

    if (depth <= 0) {
        return score;
    }

    int oppBest = std::numeric_limits<int>::max();
    auto oppMoves = allPossibleMoves(*testBoard, !isWhiteTurn);
    if (oppMoves.empty()) {
        return score;
    }
    for (auto& oppMv : oppMoves) {
        int val = evaluateMove(*testBoard,
                               oppMv.first,
                               oppMv.second,
                               !isWhiteTurn,
                               depth - 1);
        oppBest = std::min(oppBest, val);
    }
    score -= oppBest;
    return score;
}

std::vector<MoveCandidate> recommendMoves(Board& board,
                                          bool isWhiteTurn,
                                          int searchDepth,
                                          int topN) {
    PriorityQueue<MoveCandidate, MoveComparator> pq(topN);
    auto moves = allPossibleMoves(board, isWhiteTurn);
    for (auto& mv : moves) {
        int sc = evaluateMove(board, mv.first, mv.second, isWhiteTurn, searchDepth);
        MoveCandidate candidate{ mv.first, mv.second, sc };
        pq.push(candidate);
    }
    std::vector<MoveCandidate> result;
    while (!pq.empty()) {
        result.push_back(pq.poll());
    }
    std::reverse(result.begin(), result.end());
    if ((int)result.size() > topN) result.resize(topN);
    return result;
}