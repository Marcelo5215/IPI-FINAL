#include "tictactoe.h"

vector<DMatch> thresholdDist(vector<DMatch> matches, float min){
	vector<DMatch> output;
	for (int i = 0; i < matches.size(); i++){
		if(matches[i].distance < min){
			output.push_back(matches[i]);
		}
	}

	return output;
}

Mat templateMatching(Mat image, Mat temp){
	Mat output;
	matchTemplate(image, temp, output, CV_TM_CCOEFF_NORMED);
	normalize(temp, temp, 0, 1, NORM_MINMAX);
	output.convertTo(output, CV_32FC1);
	threshold(output, output, 0.45, 1, THRESH_BINARY);
	return output;
}

int checkX(Mat image, int *n){
	printf("N = %d\n", *n);
	for (int i = 0; i < image.rows; ++i){
		for (int j = 0; j < image.cols; ++j){
			if(image.at<float>(i, j) != 0){
				if(*n == 10){
					return -1;
				}
				*n = *n + 1;
				return 0;
			}
		}
	}
	// image.convertTo(image, CV_8U);

	// SimpleBlobDetector::Params params;
	// params.minDistBetweenBlobs = 0.0f;
	// params.filterByInertia = false;
	// params.filterByConvexity = false;
	// params.filterByColor = false;
	// params.filterByCircularity = false;
	// params.filterByArea = true;
	// params.minArea = 40.0f;
	// params.maxArea = 500.0f;	


	// Ptr<SimpleBlobDetector> blob = SimpleBlobDetector::create(params);
	// vector<KeyPoint> keypoints;
	// blob->detect(image, keypoints);

	// if(keypoints.size() == 0){
	// 	return 0;
	// }
	// else{
	// 	drawKeypoints( image, keypoints, image, Scalar(255,0,0), DrawMatchesFlags::DEFAULT);
	// 	if(*n == 5){
	// 		printf("KJASDHKAWJDHAKJDHAWKJDAWHKDJAWHDKAJWDHKAWJDHAWKJDAHW\n");
	// 		printf("KJASDHKAWJDHAKJDHAWKJDAWHKDJAWHDKAJWDHKAWJDHAWKJDAHW\n");
	// 		printf("KJASDHKAWJDHAKJDHAWKJDAWHKDJAWHDKAJWDHKAWJDHAWKJDAHW\n");
	// 		printf("KJASDHKAWJDHAKJDHAWKJDAWHKDJAWHDKAJWDHKAWJDHAWKJDAHW\n");
	// 		return -1;
	// 	}
	// 	*n = *n + 1;

	// }

	return 0;
}

void fillWithZeros(int board[9]){
	for (int i = 0; i < 9; ++i){
		board[i] = 0;
	}
	
	return;
}

void copyTabuleiro(int board1[9], int board2[9]){
	for (int i = 0; i < 9; ++i){
		board2[i] = board1[i];
	}

	return;
}

bool checkChange(int board1[9], int board2[9]){
	for (int i = 0; i < 9; ++i){
		if(board1[i] != board2[i]){
			return true;
		}	
	}

	return false;
}

Mat drawImage(Size image, int board[9]){
	Mat output = Mat::zeros(image, CV_8UC3);
	Point center;
	int thick = 9;

	for (int i = 0; i < 9; ++i){
		if(board[i] == 0 || board[i] == -1){
			continue;
		}
		switch(i){
			case 0:
				center.y = image.height/6;
				center.x = image.width/6;
				circle(output, center, 20, Scalar(0, 255, 128), thick);
				break;
			case 1:
				center.y = image.height/6;
				center.x = 3*image.width/6;
				circle(output, center, 20, Scalar(0, 255, 128), thick);

				break;
			case 2:
				center.y = image.height/6;
				center.x = 5*image.width/6;
				circle(output, center, 20, Scalar(0, 255, 128), thick);

				break;
			case 3:
				center.y = 3*image.height/6;
				center.x = image.width/6;
				circle(output, center, 20, Scalar(0, 255, 128), thick);

				break;
			case 4:
				center.y = 3*image.height/6;
				center.x = 3*image.width/6;
				circle(output, center, 20, Scalar(0, 255, 128), thick);

				break;
			case 5:
				center.y = 3*image.height/6;
				center.x = 5*image.width/6;
				circle(output, center, 20, Scalar(0, 255, 128), thick);

				break;
			case 6:
				center.y = 5*image.height/6;
				center.x = image.width/6;
				circle(output, center, 20, Scalar(0, 255, 128), thick);

				break;
			case 7:
				center.y = 5*image.height/6;
				center.x = 3*image.width/6;
				circle(output, center, 20, Scalar(0, 255, 128), thick);

				break;
			case 8:
				center.y = 5*image.height/6;
				center.x = 5*image.width/6;
				circle(output, center, 20, Scalar(0, 255, 128), thick);

				break;

		}
	}

	return output;
}