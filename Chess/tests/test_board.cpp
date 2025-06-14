// ==============================
// File: tests/test_board.cpp
// ==============================
#include <cassert>
#include <iostream>
#include "../include/Board.h"
#include "../include/Rook.h"
#include "../include/Bishop.h"
#include "../include/Knight.h"
#include "../include/Queen.h"
#include "../include/King.h"
#include "../include/Pawn.h"

using namespace std;

int main() {
    string mini =
        "r#"
        + string(6, '#')
        + "#R"
        + string(6, '#')
        + string(8 * 6, '#');

    assert((int)mini.size() == 64);

    Board b1;
    b1.initializeFromString(mini);

    {
        Piece* p = b1.getPieceAt({0, 0});
        assert(p && p->symbol() == 'r');
        assert(p->getColor() == Color::BLACK);
    }

    assert(b1.getPieceAt({0, 1}) == nullptr);

    {
        Piece* p = b1.getPieceAt({1, 1});
        assert(p && p->symbol() == 'R');
        assert(p->getColor() == Color::WHITE);
    }

    string start =
        "RNBQKBNR"
        "PPPPPPPP"
        "########"
        "########"
        "########"
        "########"
        "pppppppp"
        "rnbqkbnr";

    Board b2;
    b2.initializeFromString(start);

    {
        Piece* p = b2.getPieceAt({0, 0});
        assert(p && tolower(p->symbol()) == 'r' && p->getColor() == Color::WHITE);
        bool canJump = p->canMove(b2, {0, 0}, {2, 0});
        assert(!canJump);
    }

    {
        Piece* p = b2.getPieceAt({0, 0});
        bool diag = p->canMove(b2, {0, 0}, {1, 1});
        assert(!diag);
    }

    {
        Piece* pa2 = b2.getPieceAt({1, 0});
        assert(pa2 && tolower(pa2->symbol()) == 'p' && pa2->getColor() == Color::WHITE);
        bool canMove2 = pa2->canMove(b2, {1, 0}, {3, 0});
        assert(canMove2);
    }

    {
        Piece* pa2 = b2.getPieceAt({1, 0});
        bool cannotDiag = !pa2->canMove(b2, {1, 0}, {2, 1});
        assert(cannotDiag);
    }


    {
        Piece* nb1 = b2.getPieceAt({0, 1});
        assert(nb1 && tolower(nb1->symbol()) == 'n' && nb1->getColor() == Color::WHITE);
        bool okKnight = nb1->canMove(b2, {0, 1}, {2, 2});
        assert(okKnight);
    }

    {
        string oneCol =
            "########"
            "########"
            "########"
            "###R####"
            "###p####"
            "###B####"
            + string(8 * 2, '#');

        Board b3;
        b3.initializeFromString(oneCol);

        assert(!b3.isPathClear({3, 3}, {6, 3}));
        assert(!b3.isPathClear({5, 3}, {3, 3}));
        assert(b3.isPathClear({3, 3}, {3, 6}));
    }

    cout << "[Board tests passed]\n";
    return 0;
}
