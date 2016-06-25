#ifndef H_TICTACTOE
#define H_TICTACTOE

	#include <iostream>
	#include <stdio.h>
	#include <math.h>
	#include <stdlib.h>
	#include "game_tree.h"
	#include "opencv2/opencv.hpp"
	#include "opencv2/xfeatures2d.hpp"

	using namespace cv;
	using namespace xfeatures2d;
	using namespace std;


	vector<DMatch> thresholdDist(vector<DMatch> matches, float min);
	Mat templateMatching(Mat image, Mat temp);
	int checkX(Mat image, int *n);
	void fillWithZeros(int board[9]);
	void copyTabuleiro(int board1[9], int board2[9]);
	bool checkChange(int board1[9], int board2[9]);
	Mat drawImage(Size image, int board[9]);

#endif