#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace cv;
using namespace xfeatures2d;
using namespace std;

vector<DMatch> thresholdDist(vector<DMatch> matches, float min){
	vector<DMatch> output;
	for (int i = 0; i < matches.size(); i++){
		if(matches[i].distance < min){
			output.push_back(matches[i]);
		}
	}

	return output;
}

int main(int argc, char** argv){
	VideoCapture cap;
	if(!cap.open(0))
    	return 0;

	Mat frame, reference, prevHomography = Mat::zeros(Size(3,3), CV_64F);
	double Norm;
	//adquirindo imagem de referência
	namedWindow("Reference", CV_WINDOW_FREERATIO);
	for(;;){
		cap >> reference;
		imshow("Reference", reference);
		if(waitKey(1) == 27){
			break;
		}
	}

	//adquiri pontos de interesse
	Ptr<SURF> ref = SURF::create();
	vector<KeyPoint> refPoints;
	ref->detect(reference, refPoints);
	KeyPointsFilter::retainBest( refPoints, 200);
	//adquiri descritores

	Ptr<SURF> extractor = SURF::create();
	Mat refDescriptor;
	extractor->compute(reference, refPoints, refDescriptor);

	//pontos de interesse do frame atual
	Ptr<SURF> fram = SURF::create();
	vector<KeyPoint> framPoints;

	Mat framDescriptor;

	for(;;){
		cap >> frame;
		cvtColor(frame, frame, CV_RGB2GRAY);

		//pontos de interesse do frame
		fram->detect(frame, framPoints);
		KeyPointsFilter::retainBest( framPoints, 200);
		//descritor do frame
		extractor->compute(frame, framPoints, framDescriptor);

		//Match dos decritores
		FlannBasedMatcher matcher;
		vector<DMatch> matches;
		matcher.match(refDescriptor, framDescriptor, matches);

		//pega apenas os pontos com uma distancia menor que 6
		vector<DMatch> good_matches = thresholdDist(matches, 6.0);

		//divide os pontos encontrados em o da imagem de referencia e o frame
		vector<Point2f> framePo;
		vector<Point2f> refPo;
		for (int i = 0; i < good_matches.size(); ++i){
			refPo.push_back(refPoints[ good_matches[i].queryIdx ].pt);
			framePo.push_back(framPoints[ good_matches[i].trainIdx ].pt);
		}

		//calcula a homografia
		Mat Homography = findHomography( refPo, framePo, CV_RANSAC);

		//calcula a norma da diferenca das homografias
		Mat sub;
		subtract(Homography, prevHomography, sub);
		Norm = norm(sub);
		cout << "Norm: " << Norm << endl;
		if(Norm < 10){
			printf("Changed\n");
			prevHomography = Homography;
		}
		//desenja os pontos de interesse apenas para visualização
		drawKeypoints( frame, framPoints, frame, Scalar(), DrawMatchesFlags::DEFAULT);

		//transforma a perspectiva
		warpPerspective(frame, frame, prevHomography, frame.size());


		imshow("Frame", frame);
		if(waitKey(1) == 27){
			break;
		}
	}


	return 0;
}