#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "game_tree.h"
#include "tictactoe.h"
#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace cv;
using namespace xfeatures2d;
using namespace std;

int main(int argc, char** argv){
	VideoCapture cap;
	if(!cap.open(1)){
		printf("Nao e possivel abrir\n");
    	return 0;
	}

	Mat frame, reference, prevHomography = Mat::zeros(Size(3,3), CV_64F);
	Mat Homography;
	Mat temp;
	int tabuleiro[9], prevTabuleiro[9];
	fillWithZeros(tabuleiro);
	fillWithZeros(prevTabuleiro);
	int n1=0, n2=0, n3=0, n4=0, n5=0, n6=0, n7=0, n8=0, n9=0;
	int n_frames = 0, max_samples = 20;
	double Norm, norm_hold = 0.;

	//OPERAÇÕES SOBRE O TEMPLATE
	temp = imread("X.jpg", IMREAD_GRAYSCALE);
	if (temp.empty()){
		printf("Erro no  template\n");
		return -1;
	}
	resize(temp, temp, Size(), 0.4, 0.4);
	imshow("Template", temp);

	//adquirindo imagem de referência
	namedWindow("Reference", CV_WINDOW_FREERATIO);
	if(argc == 1){
		for(;;){
			cap >> reference;
			imshow("Reference", reference);
			if(waitKey(1) == 27){
				break;
			}
			imwrite("Ref.png", reference);
		}
	}
	else{
		reference = imread(argv[1], IMREAD_COLOR);
	}

	cvtColor(reference, reference, CV_RGB2GRAY);

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

	for (;;){
		cap >> frame;
		imshow("frame", frame);
		if (waitKey(1) == 27){
			break;
		}
	}

	for(;;){
		cap >> frame;
		Mat grayFrame;
		cvtColor(frame, grayFrame, CV_RGB2GRAY);
		grayFrame.convertTo(grayFrame, CV_8U);

		//pontos de interesse do frame
		fram->detect(grayFrame, framPoints);
		KeyPointsFilter::retainBest( framPoints, 200);
		//descritor do grayFrame
		extractor->compute(grayFrame, framPoints, framDescriptor);

		//Match dos decritores 
		BFMatcher matcher(NORM_L1);
		vector<DMatch> matches;
		matcher.match(refDescriptor, framDescriptor, matches);

		//pega apenas os pontos com uma distancia menor que 6
		vector<DMatch> good_matches = thresholdDist(matches, 6.0);

		//divide os pontos encontrados em o da imagem de referencia e o grayFrame
		vector<Point2f> grayFramePo;
		vector<Point2f> refPo;
		for (int i = 0; i < (int)good_matches.size(); ++i){
			refPo.push_back(refPoints[ good_matches[i].queryIdx ].pt);
			grayFramePo.push_back(framPoints[ good_matches[i].trainIdx ].pt);
		}

		//desenha os matches
		Mat img_matches;
  		drawMatches( reference, refPoints, grayFrame, framPoints,
               good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

		imshow("MATCHES", img_matches);


		//calcula a homografia
		Homography = findHomography( refPo, grayFramePo, CV_RANSAC);

		//adquiri o threshold da homografia nos primeiros 20 frames
		if(n_frames < max_samples){
			norm_hold += norm(Homography);
			prevHomography = Homography;
			if (n_frames == max_samples-1){
				norm_hold = norm_hold/((double)max_samples*15);
			}
			n_frames++;
			continue;
		}
		else{
			//calcula a norma da diferenca das homografias
			Mat sub;
			subtract(Homography, prevHomography, sub);
			Norm = norm(sub);
			cout << "Norm: " << Norm << endl;
			//atualiza a homografia caso necessario
			if(Norm < norm_hold){
				printf("Changed\n");
				prevHomography = Homography;
			}
		}

		Mat warpedgrayFrame;
		//transforma a perspectiva
		warpPerspective(grayFrame, warpedgrayFrame, prevHomography, grayFrame.size(), WARP_INVERSE_MAP);

		//adquiri a s imagens dos quadrantes do jogo
		Mat Q1(warpedgrayFrame, Rect(0, 0,
									 warpedgrayFrame.cols/3 , warpedgrayFrame.rows/3));

		Mat Q2(warpedgrayFrame, Rect(warpedgrayFrame.cols/3 , 0,
									 warpedgrayFrame.cols/3 , warpedgrayFrame.rows/3 ));

		Mat Q3(warpedgrayFrame, Rect(2*warpedgrayFrame.cols/3 , 0,
									 warpedgrayFrame.cols/3, warpedgrayFrame.rows/3 ));

		Mat Q4(warpedgrayFrame, Rect(0, warpedgrayFrame.rows/3 , 
									 warpedgrayFrame.cols/3 , warpedgrayFrame.rows/3 + 30));

		Mat Q5(warpedgrayFrame, Rect(warpedgrayFrame.cols/3 , warpedgrayFrame.rows/3 ,
									 warpedgrayFrame.cols/3 , warpedgrayFrame.rows/3 ));

		Mat Q6(warpedgrayFrame, Rect(2*warpedgrayFrame.cols/3 , warpedgrayFrame.rows/3 ,
									 warpedgrayFrame.cols/3 , warpedgrayFrame.rows/3 + 30));
		
		Mat Q7(warpedgrayFrame, Rect(0, 2*warpedgrayFrame.rows/3 ,
									 warpedgrayFrame.cols/3 , warpedgrayFrame.rows/3 ));
		
		Mat Q8(warpedgrayFrame, Rect(warpedgrayFrame.cols/3 , 2*warpedgrayFrame.rows/3 ,
									 warpedgrayFrame.cols/3 , warpedgrayFrame.rows/3 ));
		
		Mat Q9(warpedgrayFrame, Rect(2*warpedgrayFrame.cols/3 , 2*warpedgrayFrame.rows/3 ,
									 warpedgrayFrame.cols/3 , warpedgrayFrame.rows/3 ));

		imshow("Q1", Q1);
		imshow("Q2", Q2);
		imshow("Q3", Q3);
		imshow("Q4", Q4);
		imshow("Q5", Q5);
		imshow("Q6", Q6);
		imshow("Q7", Q7);
		imshow("Q8", Q8);
		imshow("Q9", Q9);

		// cout << "Q5:" << Q5.type() << endl;
		// cout << "Q5 Chan:" << Q5.channels() << endl;
		// cout << "TEMPLATE:" << temp.type() << endl;
		// cout << "TEMPLATE Chan:" << temp.channels() << endl;
		Mat map1, map2, map3, map4, map5, map6, map7, map8, map9;
		map1 = templateMatching(Q1, temp);
		map2 = templateMatching(Q2, temp);
		map3 = templateMatching(Q3, temp);
		map4 = templateMatching(Q4, temp);
		map5 = templateMatching(Q5, temp);
		map6 = templateMatching(Q6, temp);
		map7 = templateMatching(Q7, temp);
		map8 = templateMatching(Q8, temp);
		map9 = templateMatching(Q9, temp);

		if(tabuleiro[0] == NOTH){
			tabuleiro[0] = checkX(map1, &n1);
		}
		if(tabuleiro[1] == NOTH){
			tabuleiro[1] = checkX(map2, &n2);
		}
		if(tabuleiro[2] == NOTH){
			tabuleiro[2] = checkX(map3, &n3);
		}
		if(tabuleiro[3] == NOTH){
			tabuleiro[3] = checkX(map4, &n4);
		}
		if(tabuleiro[4] == NOTH){
			tabuleiro[4] = checkX(map5, &n5);
		}
		if(tabuleiro[5] == NOTH){
			tabuleiro[5] = checkX(map6, &n6);
		}
		if(tabuleiro[6] == NOTH){
			tabuleiro[6] = checkX(map7, &n7);
		}
		if(tabuleiro[7] == NOTH){
			tabuleiro[7] = checkX(map8, &n8);
		}
		if(tabuleiro[8] == NOTH){
			tabuleiro[8] = checkX(map9, &n9);
		}
		imshow("map1", map1);
		imshow("map2", map2);
		imshow("map3", map3);
		imshow("map4", map4);
		imshow("map5", map5);
		imshow("map6", map6);
		imshow("map7", map7);
		imshow("map8", map8);
		imshow("map9", map9);
		if (checkChange(tabuleiro, prevTabuleiro)){
			computerMove(tabuleiro);
			copyTabuleiro(tabuleiro, prevTabuleiro);
		}

		draw(tabuleiro);

		Mat resposta = Mat::zeros(frame.size(), CV_8UC3); 
		//Point center;
		// center.x = frame.rows/6;
		// center.y = frame.cols/6;
		// circle(resposta, center, 20, Scalar(0, 255, 128), 8);
		resposta = drawImage(frame.size(), tabuleiro);
		imshow("GAME", resposta);

		imshow("grayFrame", warpedgrayFrame);
		n_frames++;
		if(waitKey(1) == 27){
			break;
		}
	}


	return 0;
}