#include <iostream>
#include <string>
#include <chrono>
#include "../include/Chess.h"
#include "../include/Board.h"
#include "../include/MoveRecommendation.h"

using namespace std;
using Clock = std::chrono::high_resolution_clock;

int main() {
    cout << "Enter search depth (plies): ";
    int depth;
    cin >> depth;
    if (depth < 0) depth = 0;

    cout << "Enter number of threads to use (0 = single-thread): ";
    int numThreads;
    cin >> numThreads;
    if (numThreads < 0) numThreads = 0;

    cout << "Choose mode: 1 = interactive vs human, 2 = auto (engine vs itself): ";
    int mode;
    cin >> mode;

    Chess gui;
    const string startingBoard =
        "RNBQKBNR"
        "PPPPPPPP"
        "########"
        "########"
        "########"
        "########"
        "pppppppp"
        "rnbqkbnr";

    Board board;
    board.initializeFromString(startingBoard);

    bool myTurn = true;

    {
        auto recs = recommendMovesParallel(board, myTurn, depth, /*topN=*/1, numThreads, /*earlyThreshold=*/INT_MAX);
        if (!recs.empty()) {
            string text = "Recommended move: ";
            text += char('a' + recs[0].src.first);
            text += char('1' + recs[0].src.second);
            text += " ";
            text += char('a' + recs[0].dest.first);
            text += char('1' + recs[0].dest.second);
            Chess::setRecommendation(text);
        } else {
            Chess::setRecommendation("Recommended move: (none)");
        }
    }

    if (mode == 1) {
        while (true) {
            string move = gui.getInput();
            if (move == "exit") break;

            int sr = move[1] - '1';
            int sc = move[0] - 'a';
            int dr = move[3] - '1';
            int dc = move[2] - 'a';
            Position src{ sc, sr }, dest{ dc, dr };

            Piece* piece = board.getPieceAt(src);
            int response;

            if (!piece) {
                response = 11;
            }
            else if ((piece->getColor() == Color::WHITE) != myTurn) {
                response = 12;
            }
            else {
                Piece* destPiece = board.getPieceAt(dest);
                if (destPiece && destPiece->getColor() == piece->getColor()) {
                    response = 13;
                }
                else if (!piece->canMove(board, src, dest)) {
                    response = 21;
                }
                else {
                    auto testBoard = board.clone();
                    testBoard->movePiece(src, dest);
                    if (testBoard->isInCheck(piece->getColor())) {
                        response = 31;
                    }
                    else {
                        board.movePiece(src, dest);
                        Color opp = (piece->getColor() == Color::WHITE ? Color::BLACK : Color::WHITE);
                        if (board.isInCheck(opp)) {
                            response = 41;
                        } else {
                            response = 42;
                        }
                    }
                }
            }

            gui.setCodeResponse(response);
            if (response == 41 || response == 42) {
                myTurn = !myTurn;
            }

            {
                auto recs = recommendMovesParallel(board, myTurn, depth, /*topN=*/1, numThreads, /*earlyThreshold=*/INT_MAX);
                if (!recs.empty()) {
                    string text = "Recommended move: ";
                    text += char('a' + recs[0].src.first);
                    text += char('1' + recs[0].src.second);
                    text += " ";
                    text += char('a' + recs[0].dest.first);
                    text += char('1' + recs[0].dest.second);
                    Chess::setRecommendation(text);
                } else {
                    Chess::setRecommendation("Recommended move: (none)");
                }
            }
        }

        return 0;
    }


    cout << "\n** Auto‐play for 8 plies using “top‐1” recommendation **\n";
    int totalPlies = 8;
    int pliesDone = 0;

    auto t0 = Clock::now();

    while (pliesDone < totalPlies) {
        auto plyStart = Clock::now();

        auto recs = recommendMovesParallel(board, myTurn, depth, /*topN=*/1, numThreads, /*earlyThreshold=*/INT_MAX);

        if (recs.empty()) {
            cout << "\nNo legal moves available for " << (myTurn ? "White" : "Black") << ". Stopping.\n";
            break;
        }

        Position src = recs[0].src;
        Position dest = recs[0].dest;
        Piece* piece = board.getPieceAt(src);

        board.movePiece(src, dest);
        Color opp = (piece->getColor() == Color::WHITE ? Color::BLACK : Color::WHITE);
        bool givesCheck = board.isInCheck(opp);

        int code = givesCheck ? 41 : 42;
        gui.setCodeResponse(code);
        if (code == 41 || code == 42) {
            myTurn = !myTurn;
        }

        auto plyEnd = Clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(plyEnd - plyStart).count();

        char f1 = 'a' + recs[0].src.first;
        char r1 = '1' + recs[0].src.second;
        char f2 = 'a' + recs[0].dest.first;
        char r2 = '1' + recs[0].dest.second;

        cout << (myTurn ? "Black" : "White")
             << " plays: " << f1 << r1 << "-" << f2 << r2
             << "  (gen time: " << delta << " ms)\n";

        ++pliesDone;
    }

    auto t1 = Clock::now();
    auto totalDelta = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    cout << "\n** Auto‐play complete. Total time for " << totalPlies
         << " plies: " << totalDelta << " ms**\n";

    return 0;
}