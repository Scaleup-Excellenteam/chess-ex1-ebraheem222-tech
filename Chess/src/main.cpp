
#include <iostream>
#include <string>
#include "../include/Chess.h"
#include "../include/Board.h"
#include "../include/MoveRecommendation.h"

using namespace std;

int main() {
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

    bool myTurn = true; // true = White to move, false = Black to move

    {
        auto suggestions = recommendMoves(board, /*isWhiteTurn=*/ myTurn, /*depth=*/ 2, /*topN=*/ 1);
        if (!suggestions.empty()) {
            string text = "Recommended move: ";
            text += char('a' + suggestions[0].src.first);
            text += char('1' + suggestions[0].src.second);
            text += " ";
            text += char('a' + suggestions[0].dest.first);
            text += char('1' + suggestions[0].dest.second);
            Chess::setRecommendation(text);
        } else {
            Chess::setRecommendation("Recommended move: (none)");
        }
    }

    while (true) {

        string move = gui.getInput();
        if (move == "exit") {
            break;
        }
        int sr = move[1] - '1';
        int sc = move[0] - 'a';
        int dr = move[3] - '1';
        int dc = move[2] - 'a';
        Position src{ sc, sr };
        Position dest{ dc, dr };

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
                    }
                    else {
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
            auto suggestions = recommendMoves(board, myTurn, /*depth=*/ 2, /*topN=*/ 1);
            if (!suggestions.empty()) {
                string text = "Recommended move: ";
                text += char('a' + suggestions[0].src.first);
                text += char('1' + suggestions[0].src.second);
                text += " ";
                text += char('a' + suggestions[0].dest.first);
                text += char('1' + suggestions[0].dest.second);
                Chess::setRecommendation(text);
            } else {
                Chess::setRecommendation("Recommended move: (none)");
            }
        }
    }

    return 0;
}
