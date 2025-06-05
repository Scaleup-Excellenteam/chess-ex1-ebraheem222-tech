
#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include <string>
#ifdef _WIN32
#include <Windows.h>
#endif

#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;

const int _SIZE = 21;

class Chess {
	unsigned char m_board[_SIZE][_SIZE] = { 0 };
	bool m_turn = true;
	string m_boardString;
	string m_input;
	string m_msg = "\n";
	string m_errorMsg = "\n";
	int m_codeResponse;

	// Holds the single‐line recommendation text
	static string s_recommendation;

	void clear() const;
	void setFrames();
	void setPieces();
	void show() const;
	void displayBoard() const;
	void showAskInput() const;
	bool isSame() const;
	bool isValid() const;
	bool isExit() const;

	// <== Restore this declaration so Chess.cpp’s “void Chess::excute()” has a matching prototype:
	void excute();

	void doTurn();

public:
	Chess(const string& start = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr");
	Chess(const Chess&) = delete;
	Chess& operator=(const Chess&) = delete;

	// Called by main.cpp to redraw the board + prompt the user
	string getInput();
	// Called by main.cpp to inform GUI whether the last move was legal, etc.
	void setCodeResponse(int codeResponse);

	// <== Make this public so main.cpp can set it before calling getInput()
	static void setRecommendation(const string& rec) {
		s_recommendation = rec;
	}
};

#endif // CHESS_H
