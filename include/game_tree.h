#ifndef GAMETREE_H
#define GAMETREE_H

	#include <stdio.h>
	#include <stdlib.h>

	#define COMP 1
	#define HUM -1
	#define NOTH 0

	int win(const int board[9]);
	void draw(int b[9]);
	int minimax(int board[9], int player);
	void computerMove(int board[9]);
	void playerMove(int board[9]);

#endif