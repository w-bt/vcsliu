#include "stdafx.h"
#include "Core.h"
#include "Share.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <direct.h>
#include <time.h>
#include <set>
#include <algorithm>    // std::shuffle, sort
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <thread>
#include <fstream>
#include <cstdlib>
#include <ctime>

#if defined(_MSC_VER) && _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4146 4244 4800)
#endif
#include <mpirxx.h>
#if defined(_MSC_VER) && _MSC_VER
#pragma warning(pop)
#endif

using namespace cv;
using namespace std;

int M0S2[2][4] = {
		{0, 0, 255, 255},
		{0, 0, 255, 255}
	};

int M1S2[2][4] = {
		{0, 0, 255, 255},
		{255, 255, 0, 0}
	};

int M0S3[2][9] = {
		{0, 0, 0, 255, 255, 255, 255, 255, 0},
		{0, 0, 0, 255, 255, 255, 255, 255, 0}
	};

int M1S3[2][9] = {
		{0, 0, 0, 255, 255, 255, 255, 255, 0},
		{255, 255, 255, 0, 0, 0, 255, 255, 0}
	};

int M0S8[2][16] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255},
		{0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255}
	};

int M1S8[2][16] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255},
		{255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0}
	};

array<int,4> posS2 = {0,1,2,3};

array<int,9> posS3 = {0,1,2,3,4,5,6,7,8};

array<int,16> posS8 = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

int Core::optPause;

int Core::optPreview;

int Core::optOthers;

int Core::optScheme;

int Core::optShift;

int Core::block;

Core::Core()
{
	reset();

	_mkdir("result/");
	_mkdir("result/encoding/");
	_mkdir("result/encoding/others/");
	_mkdir("result/decoding/");
}

void Core::reset(){
	widthSI		= 0;
	heightSI	= 0;
	widthCanvas = 0;
	heightCanvas= 0;
	optPause	= 1;
	optPreview	= 1;
	optOthers	= 1;
	optScheme	= 1;
	optShift	= 1;
	optBlock	= 1;
	optMinWidth = 1;
	optMinHeight= 1;
	SI.release();
	filteredSI.release();
	canvas.release();
}

int Core::viewMainMenu(){
	system("cls");
	int ans = 0;

	cout << "================================================================================" << endl;
	cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
	cout << "================================================================================" << endl;
	cout << "\n";
	centerString("Option :");
	cout << "\n\n" << endl;
	centerString("(1) Encoding");
	cout << "\n" << endl;
	centerString("(2) Decoding");
	cout << "\n" << endl;
	centerString("(3) Option");
	cout << "\n" << endl;
	centerString("(4) Exit");
	cout << "\n\n" << endl;
	cout << "================================================================================" << endl;
	cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
	cout << "================================================================================" << endl;
	while((ans != 1)&&(ans != 2)&&(ans != 3)&&(ans != 4)){
		cout << "\n> Your choice (1/2/3/4) ? ";
		cin >> ans;
		cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
	}

	return ans;
}

void Core::viewOption(){
	system("cls");
	// *************************************************************************************
	// ************************************* OPTION ****************************************
	// *************************************************************************************
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "*******************************************************************************" << endl;
	cout << "********************************** OPTION *************************************" << endl;
	cout << "*******************************************************************************" << endl;
	setOptionPause();
	setOptionPreview();
	setOptionOthers();
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if(optPause == 1){
		cout << "\n" ;
		system("pause");
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	int opt = viewMainMenu();
	if(opt == 1){
		viewEncoding();
	} else {
		if(opt == 2){
			viewDecoding();
		} else {
			if(opt == 3){
				viewOption();
			} else {
				exit(0);
			}
		}
	}
}

void Core::setOptionPause(){
	int flag	= 0;
	int ans;
	while(flag == 0){
		cout << "\n> Pause for each step (1) yes (2) no : ";
		cin >> ans;
		if(ans == 1){
			optPause	= 1;
			flag		= 1;
		} else {
			if(ans == 2){
				optPause	= 0;
				flag		= 1;
			} else {
				cout << "\nPlease input (1) or (2)";
			}
		}
	}
}

void Core::setOptionPreview(){
	int flag	= 0;
	int ans;
	while(flag == 0){
		cout << "\n> Turn on preview (1) yes (2) no : ";
		cin >> ans;
		if(ans == 1){
			optPreview	= 1;
			flag		= 1;
		} else {
			if(ans == 2){
				optPreview	= 0;
				flag		= 1;
			} else {
				cout << "\nPlease input (1) or (2)";
			}
		}
	}
}

void Core::setOptionOthers(){
	int flag	= 0;
	int ans;
	while(flag == 0){
		cout << "\n> Save assets to folder others (1) yes (2) no : ";
		cin >> ans;
		if(ans == 1){
			optOthers	= 1;
			flag		= 1;
		} else {
			if(ans == 2){
				optOthers	= 0;
				flag		= 1;
			} else {
				cout << "\nPlease input (1) or (2)";
			}
		}
	}
}

void Core::centerString(char* s)
{
	int l 	= strlen(s);
   	int pos = (int)((80-l)/2);
   	for(int i = 0; i < pos; i++)
    	cout << " ";

   	cout << s;
}

void Core::viewEncoding(){
	system("cls");
	// *************************************************************************************
	// ******************************** PRE-DECOMPOSING ************************************
	// *************************************************************************************
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "*******************************************************************************" << endl;
	cout << "***************************** PRE-DECOMPOSING *********************************" << endl;
	cout << "*******************************************************************************" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================
	// ============================== DETERMINING SECRET IMAGE =============================
	// =====================================================================================
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "========================== DETERMINING SECRET IMAGE ===========================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	setSecretImage();
	cout << "\nSI's size = " << widthSI << "x" << heightSI << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if(optPause == 1){
		cout << "\n" ;
		system("pause");
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================

	// =====================================================================================
	// ================================= DETERMINING SCHEME ================================
	// =====================================================================================
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "============================= DETERMINING SCHEME ==============================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	chooseScheme();
	Share S1(1);
	Share S2(2);
	if(optScheme == 1){
		viewEncodingIMSVCS(S1,S2);
	} else {
		if(optScheme == 2){
			viewEncodingNIMSVCS(S1,S2);
		}
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n> Finished" << endl;
	cout << "\n" ;
	system("pause");
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================
	int opt = viewMainMenu();
	S1.~Share();
	S2.~Share();
	reset();
	if(opt == 1){
		viewEncoding();
	} else {
		if(opt == 2){
			viewDecoding();
		} else {
			if(opt == 3){
				viewOption();
			} else {
				exit(0);
			}
		}
	}
}

void Core::setSecretImage()
{
	string pathSI;
	int flagSI	= 0;
	Mat tempSI;
	while(flagSI == 0){
		cout << "\n> Input SI's path : ";
		getline (cin, pathSI);
		ext 		= pathSI.substr(pathSI.size() - 3);
		tempSI		= imread(pathSI,0);
		if(tempSI.data > 0){
			SI		= tempSI;
			widthSI	= tempSI.cols;
			heightSI= tempSI.rows;
			flagSI	= 1;
		} else {
			cout << "\nNo image found" << endl;
		}
	}
}

void Core::chooseScheme(){
	int flag	= 0;
	int ans;
	while(flag == 0){
		cout << "\n> Choose scheme (1) IMSVCS (2) NIMSVCS : ";
		cin >> ans;
		if(ans == 1){
			flag		= 1;
			optScheme	= 1;
		} else {
			if(ans == 2){
				flag		= 1;
				optScheme	= 2;
			} else {
				cout << "\nPlease input (1) or (2)";
			}
		}
	}
}

void Core::viewEncodingIMSVCS(Share& S1, Share& S2){
	// *************************************************************************************
	// *********************************** DECOMPOSING *************************************
	// *************************************************************************************
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n*******************************************************************************" << endl;
	cout << "******************************** DECOMPOSING **********************************" << endl;
	cout << "*******************************************************************************" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	
	// =====================================================================================
	// ============================ DETERMINING SHIFT TOLERANCE ============================
	// =====================================================================================
	// determining shift tolerance
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "======================= DETERMINING SHIFT TOLERANCE ===========================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	setTolerance();
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if(optPause == 1){
		cout << "\n" ;
		system("pause");
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================

	// =====================================================================================
	// =============================== FILTERING SECRET IMAGE ==============================
	// =====================================================================================
	// filtering secret image
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "========================== FILTERING SECRET IMAGE =============================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n> Filtering secret image" << endl;
	filter();
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if(optPause == 1){
		cout << "\n" ;
		system("pause");
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================

	// =====================================================================================
	// ================================ GENERATING SHARE =================================
	// =====================================================================================
	// generating share
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "============================== GENERATING SHARE ===============================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n> Generating shares" << endl;
	generateShareIMSVCS(S1, S2);
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================
}

void Core::setTolerance(){
	int flag	= 0;
	int ans;
	while(flag == 0){
		cout << "\n> Choose shift tolerance (2/3/8) : ";
		cin >> ans;
		if((ans == 2)||(ans == 3)||(ans == 8)){
			optShift	= ans;
			flag		= 1;
			if(ans == 8){
				block	= 4;
			} else {
				block	= ans;
			}
		} else {
			cout << "\nPlease input (2) or (3) or (8)";
		}
	}
}

void Core::filter(){
	Mat imgTemp = SI.clone();
	if(ext != "bmp"){
		// histogram equalization
		equalizeHist(SI, imgTemp);
	
		// image sharpening (laplacian)
		Mat sharp;
		Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
		filter2D(imgTemp, sharp, imgTemp.depth(), kernel);

		// dithering floyd
		for (int r = 0; r < sharp.rows; r++) {
			for (int c = 0; c < sharp.cols; c++) {
				uchar oldPixel	= sharp.at<uchar>(r,c);
				uchar newPixel	= oldPixel > 127 ? 255 : 0;
				sharp.at<uchar>(r,c)	= newPixel;
				int quantError			= oldPixel - newPixel;
				if(c+1 < sharp.cols) sharp.at<uchar>(r,c+1)					+=  (7 * quantError) >> 4;
				if((c > 0)&&(r+1 < sharp.rows)) sharp.at<uchar>(r+1,c-1)	+=  (3 * quantError) >> 4;
				if(r+1 < sharp.rows) sharp.at<uchar>(r+1,c)					+=  (5 * quantError) >> 4;
				if((r+1 < sharp.rows)&&(c+1 < sharp.cols)) sharp.at<uchar>(r+1,c+1)	+=  (1 * quantError) >> 4;
			}
		}
	}

	filteredSI = imgTemp;
}

void Core::generateShareIMSVCS(Share& S1, Share& S2){
	Mat C1			= generateNewImg(heightSI*block, widthSI*block, 1);
	Mat A1			= Mat::zeros(heightSI*block, widthSI*block, CV_8UC1);
	Mat C2			= generateNewImg(heightSI*block, widthSI*block, 1);
	Mat A2			= Mat::zeros(heightSI*block, widthSI*block, CV_8UC1);

	vector<Mat> channels1, channels2;
	unsigned seed;

	for(int i = 0; i < heightSI; i++){
		for(int j = 0; j < widthSI; j++){
			if(optShift == 2){
				// obtain a time-based seed:
				// random permutation on 'position' to get matrix c
				seed = (chrono::system_clock::now().time_since_epoch().count())+j;
				shuffle (posS2.begin(), posS2.end(), default_random_engine(seed));
				int C[2][4];
				if(filteredSI.at<uchar>(i,j) < 128){
					for(int u = 0; u < 2; u++){
						for(int v = 0; v < 4; v++){
							C[u][v] = M1S2[u][v];
						}
					}
				} else {
					for(int u = 0; u < 2; u++){
						for(int v = 0; v < 4; v++){
							C[u][v] = M0S2[u][v];
						}
					}
				}
				// transform 1D to 2D matrix m
				int m_1[2][2],m_2[2][2];
				for(int l = 0; l < 4; l++){
					int rowm		= l/2;
					m_1[rowm][l % 2]	= C[0][posS2[l]];
					m_2[rowm][l % 2]	= C[1][posS2[l]];
				}

				// asign matrix m to the share
				int marky = 0;
				for(int newy = i*block; newy < (i*block)+block; newy++){
					int markx = 0;
					for(int newx = j*block; newx < (j*block)+block; newx++){
						C1.at<uchar>(newy,newx) = m_1[marky][markx];
						if(m_1[marky][markx] == 255){
							A1.at<uchar>(newy,newx) = 0;
						} else {
							A1.at<uchar>(newy,newx) = 255;
						}
					
						C2.at<uchar>(newy,newx) = m_2[marky][markx];
						if(m_2[marky][markx] == 255){
							A2.at<uchar>(newy,newx) = 0;
						} else {
							A2.at<uchar>(newy,newx) = 255;
						}
						markx = markx + 1;
					}
					marky = marky + 1;
				}
			} else {
				if(optShift == 3){
					// obtain a time-based seed:
					// random permutation on 'position' to get matrix c
					seed = (chrono::system_clock::now().time_since_epoch().count())+j;
					shuffle (posS3.begin(), posS3.end(), default_random_engine(seed));
					int C[2][9];
					if(filteredSI.at<uchar>(i,j) < 128){
						for(int u = 0; u < 2; u++){
							for(int v = 0; v < 9; v++){
								C[u][v] = M1S3[u][v];
							}
						}
					} else {
						for(int u = 0; u < 2; u++){
							for(int v = 0; v < 9; v++){
								C[u][v] = M0S3[u][v];
							}
						}
					}
					// transform 1D to 2D matrix m
					int m_1[3][3],m_2[3][3];
					for(int l = 0; l < 9; l++){
						int rowm		= l/3;
						m_1[rowm][l % 3]	= C[0][posS3[l]];
						m_2[rowm][l % 3]	= C[1][posS3[l]];
					}

					// asign matrix m to the share
					int marky = 0;
					for(int newy = i*block; newy < (i*block)+block; newy++){
						int markx = 0;
						for(int newx = j*block; newx < (j*block)+block; newx++){
							C1.at<uchar>(newy,newx) = m_1[marky][markx];
							if(m_1[marky][markx] == 255){
								A1.at<uchar>(newy,newx) = 0;
							} else {
								A1.at<uchar>(newy,newx) = 255;
							}
					
							C2.at<uchar>(newy,newx) = m_2[marky][markx];
							if(m_2[marky][markx] == 255){
								A2.at<uchar>(newy,newx) = 0;
							} else {
								A2.at<uchar>(newy,newx) = 255;
							}
							markx = markx + 1;
						}
						marky = marky + 1;
					}
				} else {
					if(optShift == 8){
						// obtain a time-based seed:
						// random permutation on 'position' to get matrix c
						seed = (chrono::system_clock::now().time_since_epoch().count())+j;
						shuffle (posS8.begin(), posS8.end(), default_random_engine(seed));
						int C[2][16];
						if(filteredSI.at<uchar>(i,j) < 128){
							for(int u = 0; u < 2; u++){
								for(int v = 0; v < 16; v++){
									C[u][v] = M1S8[u][v];
								}
							}
						} else {
							for(int u = 0; u < 2; u++){
								for(int v = 0; v < 16; v++){
									C[u][v] = M0S8[u][v];
								}
							}
						}
						// transform 1D to 2D matrix m
						int m_1[4][4],m_2[4][4];
						for(int l = 0; l < 16; l++){
							int rowm		= l/4;
							m_1[rowm][l % 4]	= C[0][posS8[l]];
							m_2[rowm][l % 4]	= C[1][posS8[l]];
						}

						// asign matrix m to the share
						int marky = 0;
						for(int newy = i*block; newy < (i*block)+block; newy++){
							int markx = 0;
							for(int newx = j*block; newx < (j*block)+block; newx++){
								C1.at<uchar>(newy,newx) = m_1[marky][markx];
								if(m_1[marky][markx] == 255){
									A1.at<uchar>(newy,newx) = 0;
								} else {
									A1.at<uchar>(newy,newx) = 255;
								}
					
								C2.at<uchar>(newy,newx) = m_2[marky][markx];
								if(m_2[marky][markx] == 255){
									A2.at<uchar>(newy,newx) = 0;
								} else {
									A2.at<uchar>(newy,newx) = 255;
								}
								markx = markx + 1;
							}
							marky = marky + 1;
						}
					}
				}
			}
		}
	}

	if(optOthers == 1){
		imwrite("result/encoding/others/3_S1.png",C1);
		imwrite("result/encoding/others/3_S2.png",C2);
	}

	if(optPreview == 1){
		namedWindow("preview generating S1", 0);
		if(C1.cols > C1.rows){
			resizeWindow("preview generating S1", 500, ceil(((float)C1.rows/(float)((float)C1.cols/(float)500))));
		} else {
			resizeWindow("preview generating S1", ceil(((float)C1.cols/(float)((float)C1.rows/(float)500))), 500);
		}
		moveWindow("preview generating S1", 0, 0);
		imshow("preview generating S1",C1);

		namedWindow("preview generating S2", 0);
		if(C2.cols > C2.rows){
			resizeWindow("preview generating S2", 500, ceil(((float)C2.rows/(float)((float)C2.cols/(float)500))));
		} else {
			resizeWindow("preview generating S2", ceil(((float)C2.cols/(float)((float)C2.rows/(float)500))), 500);
		}
		moveWindow("preview generating S2", 500, 0);
		imshow("preview generating S2",C2);
		waitKey(300);
		if(optPause == 1){
			cout << "\n" ;
			system("pause");
		}
		destroyWindow("preview generating S1");
		destroyWindow("preview generating S2");
	}

	channels1.push_back(C1);
	channels1.push_back(C1);
	channels1.push_back(C1);
	channels1.push_back(A1);

	channels2.push_back(C2);
	channels2.push_back(C2);
	channels2.push_back(C2);
	channels2.push_back(A2);

	merge(channels1, S1.share);
	merge(channels2, S2.share);

	if(optOthers == 1){
		imwrite("result/encoding/others/eS1.png", S1.share);
		imwrite("result/encoding/others/eS2.png", S2.share);
	}

	imwrite("result/encoding/share1.png",S1.share);
	imwrite("result/encoding/share2.png",S2.share);
}

Mat Core::generateNewImg(int h, int w, int channel){
	Mat newImage;
	if(channel == 1){
		newImage.create(h, w, CV_8UC1);
		newImage = Scalar(255);
	} else {
		if(channel == 3){
			newImage.create(h, w, CV_8UC3);
			newImage = Scalar(255,255,255);
		} else {
			if(channel == 4){
				newImage.create(h, w, CV_8UC4);
				newImage = Scalar(255,255,255,0);
			}
		}
	}
	return newImage;
}

void Core::viewDecoding(){
	system("cls");
	// *************************************************************************************
	// ********************************* PRE-DECODING **************************************
	// *************************************************************************************
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "*******************************************************************************" << endl;
	cout << "****************************** PRE-DECODING ***********************************" << endl;
	cout << "*******************************************************************************" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	Share S1(1);
	S1.setShare();
	cout << "\nS1's size = " << S1.width << "x" << S1.height << endl;

	Share S2(2);
	S2.setShare();
	cout << "\nS2's size = " << S2.width << "x" << S2.height << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	chooseScheme();
	if(optScheme == 1){
		viewDecodingIMSVCS(S1,S2);
	} else {
		if(optScheme == 2){
			viewDecodingNIMSVCS(S1,S2);
		}
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n> Finished" << endl;
	cout << "\n" ;
	system("pause");
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================
	int opt = viewMainMenu();
	S1.~Share();
	S2.~Share();
	reset();
	if(opt == 1){
		viewEncoding();
	} else {
		if(opt == 2){
			viewDecoding();
		} else {
			if(opt == 3){
				viewOption();
			} else {
				exit(0);
			}
		}
	}
}

void Core::viewDecodingIMSVCS(Share& S1, Share& S2){
	// *************************************************************************************
	// ************************************ STACKING ***************************************
	// *************************************************************************************
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n*******************************************************************************" << endl;
	cout << "********************************* STACKING ************************************" << endl;
	cout << "*******************************************************************************" << endl;
	int aut = optAutostackIMSVCS();
	if(aut == 1){
		stackingIMSVCS(S1, S2);
	} else {
		if(aut == 2){
			S1.setStartPosition();
			S2.setStartPosition();
			setCanvas(S1,S2);
			estimateStack(S1,S2,0);
		}
	}
	imwrite("result/decoding/ri.png",canvas);

	if(optPreview == 1){
		namedWindow("preview reconstructed image", 0);
		if(canvas.cols > canvas.rows){
			resizeWindow("preview reconstructed image", 500, ceil(((float)canvas.rows/(float)((float)canvas.cols/(float)500))));
		} else {
			resizeWindow("preview reconstructed image", ceil(((float)canvas.cols/(float)((float)canvas.rows/(float)500))), 500);
		}
		imshow("preview reconstructed image",canvas);
		waitKey(300);
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if(optPause == 1){
		cout << "\n" ;
		system("pause");
	}
					
	if(optPreview == 1){
		destroyWindow("preview reconstructed image");
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================
	cout << "\n> Finished" << endl;
	cout << "\n" ;
	system("pause");
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================
	int opt = viewMainMenu();
	S1.~Share();
	S2.~Share();
	reset();
	if(opt == 1){
		viewEncoding();
	} else {
		if(opt == 2){
			viewDecoding();
		} else {
			if(opt == 3){
				viewOption();
			} else {
				exit(0);
			}
		}
	}
}

int	Core::optAutostackIMSVCS(){
	int result;
	int flag	= 0;
	int ans;
	while(flag == 0){
		cout << "\n> Stack automaticaly (1) yes (2) no ? ";
		cin >> ans;
		if((ans == 1)||(ans == 2)){
			result	= ans;
			flag	= 1;
		} else {
			cout << "\nPlease input (1) or (2)";
		}
	}
	return result;
}

void Core::setCanvas(Share& S1, Share& S2){
	if(S1.pointStart.x < S2.pointStart.x){
		widthCanvas	= S1.width + S1.pointStart.x + (S2.pointStart.x - S1.pointStart.x);
	} else {
		widthCanvas	= S1.width + S2.pointStart.x + (S1.pointStart.x - S2.pointStart.x);
	}

	if(S1.pointStart.y < S2.pointStart.y){
		heightCanvas	= S1.height + S1.pointStart.y + (S2.pointStart.y - S1.pointStart.y);
	} else {
		heightCanvas	= S1.height + S2.pointStart.y + (S1.pointStart.y - S2.pointStart.y);
	}
}

void Core::stackingIMSVCS(Share& S1, Share& S2){
	vector<Mat> channelsBB;
	
	for(int i = 0; i < S1.height; i++){
		for(int j = 0; j < S1.width; j++){
			int currentP	= S2.channels[0].at<uchar>(i,j);
			if(currentP < 128){
				S1.channels[0].at<uchar>(i,j)	= S2.channels[0].at<uchar>(i,j);
				S1.channels[3].at<uchar>(i,j)	= 255;
			}
		}
	}
	
	channelsBB.push_back(S1.channels[0]);
    channelsBB.push_back(S1.channels[0]);
    channelsBB.push_back(S1.channels[0]);
	channelsBB.push_back(S1.channels[3]);

	merge(channelsBB, canvas);
}

void Core::viewEncodingNIMSVCS(Share& S1, Share& S2){
	// =====================================================================================
	// =============================== DETERMINING SS SIZE =================================
	// =====================================================================================
	// determining the size of shadow share
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "========================== DETERMINING SS SIZE ================================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	S1.setSSSize(widthSI, heightSI);
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	S2.setSSSize(widthSI, heightSI);
	S2.setAvailableOrientation(widthSI, heightSI);
	
	if(S2.availableOrientation.size() == 4){
		cout << "\nSS2's available orientation = " << S2.availableOrientation[0] << ", " << S2.availableOrientation[1] << ", " << S2.availableOrientation[2] << ", and " << S2.availableOrientation[3] << endl;
	} else {
		cout << "\nSS2's available orientation = " << S2.availableOrientation[0] << " and " << S2.availableOrientation[1] << endl;
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if(optPause == 1){
		cout << "\n" ;
		system("pause");
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================

	// =====================================================================================
	// ========================== CALCULATING INTERSECTION AREA ============================
	// =====================================================================================
	// calculating intersection area
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "====================== CALCULATING INTERSECTION AREA ==========================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n> Choosing random angle rotation" << endl;
	S2.genRandomOrientation();
	
	cout << "\nSS2's angle orientation = " << S2.angle << endl;

	S2.getRotationSize();
	cout << "\nSS2's rotated width = " << S2.rotWidthSS << endl;
	cout << "\nSS2's rotated height = " << S2.rotHeightSS << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n> Calculating intersection area" << endl;

	genIntersection(S1,S2);

	cout << "\nIntersection size " << (S1.intersection[2].y - S1.intersection[0].y) << " x " << (S1.intersection[1].x - S1.intersection[0].x) << endl;
	cout << "\nIntersection area for SS1 : " << endl;
	cout << "\t[1] (" << S1.intersection[0].x << "," << S1.intersection[0].y << ")" << endl;
	cout << "\t[2] (" << S1.intersection[1].x << "," << S1.intersection[1].y << ")" << endl;
	cout << "\t[3] (" << S1.intersection[2].x << "," << S1.intersection[2].y << ")" << endl;
	cout << "\t[4] (" << S1.intersection[3].x << "," << S1.intersection[3].y << ")" << endl;
	cout << "\nIntersection area for rotated SS2 : " << endl;
	cout << "\t[1] (" << S2.intersection[0].x << "," << S2.intersection[0].y << ")" << endl;
	cout << "\t[2] (" << S2.intersection[1].x << "," << S2.intersection[1].y << ")" << endl;
	cout << "\t[3] (" << S2.intersection[2].x << "," << S2.intersection[2].y << ")" << endl;
	cout << "\t[4] (" << S2.intersection[3].x << "," << S2.intersection[3].y << ")" << endl;

	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
	if(optPause == 1){
		cout << "\n" ;
		system("pause");
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================

	// =====================================================================================
	// ============================ GENERATE RANDOM SI POSITION ============================
	// =====================================================================================
	// generating random SI position
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "======================== GENERATE RANDOM SI POSITION ==========================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n> Generate random SI position" << endl;
	
	randomSIPosition(S1, S2);
	
	cout << "\nSI position in SS1:" << endl;
	cout << "\t[A] (" << S1.position[0].x << "," << S1.position[0].y << ")" << endl;
	cout << "\t[B] (" << S1.position[1].x << "," << S1.position[1].y << ")" << endl;
	cout << "\t[C] (" << S1.position[2].x << "," << S1.position[2].y << ")" << endl;
	cout << "\t[D] (" << S1.position[3].x << "," << S1.position[3].y << ")" << endl;

	cout << "\nSI position in rotated SS2:" << endl;
	cout << "\t[A] (" << S2.position[0].x << "," << S2.position[0].y << ")" << endl;
	cout << "\t[B] (" << S2.position[1].x << "," << S2.position[1].y << ")" << endl;
	cout << "\t[C] (" << S2.position[2].x << "," << S2.position[2].y << ")" << endl;
	cout << "\t[D] (" << S2.position[3].x << "," << S2.position[3].y << ")" << endl;
	
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
	if(optPreview == 1){
		cout << "\n> Preview of intersection area " << endl;
	
		previewIA(S1, S2);
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::	
	if(optPause == 1){
		cout << "\n" ;
		system("pause");
	}

	if(optPreview == 1){
		destroyWindow("preview IA");
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================

	// *************************************************************************************
	// *********************************** DECOMPOSING *************************************
	// *************************************************************************************
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n*******************************************************************************" << endl;
	cout << "******************************** DECOMPOSING **********************************" << endl;
	cout << "*******************************************************************************" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	
	// =====================================================================================
	// ============================ DETERMINING SHIFT TOLERANCE ============================
	// =====================================================================================
	// determining shift tolerance
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "======================= DETERMINING SHIFT TOLERANCE ===========================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	setTolerance();
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if(optPause == 1){
		cout << "\n" ;
		system("pause");
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================

	// =====================================================================================
	// =============================== FILTERING SECRET IMAGE ==============================
	// =====================================================================================
	// filtering secret image
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "========================== FILTERING SECRET IMAGE =============================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n> Filtering secret image" << endl;
	filter();
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if(optPause == 1){
		cout << "\n" ;
		system("pause");
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================

	// =====================================================================================
	// ================================ GENERATING SHARE =================================
	// =====================================================================================
	// generating share
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "============================== GENERATING SHARE ===============================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n> Generating shares" << endl;

	generateShareNIMSVCS(S1, S2);
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n> Rotating share 2 to normal orientation" << endl;
	getNormalRotationImage(S2);
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	imwrite("result/encoding/share1.png",S1.share);
	imwrite("result/encoding/share2.png",S2.share);
	fstream file;
    file.open("result/encoding/others/info.txt",fstream::out);
    file << "S2 rotation : " << S2.angle << endl;
	file << "S1 position : (" << (S1.pointStart.x * block) << "," << (S1.pointStart.y * block) << ")" << endl;
	file << "S2 position : (" << (S2.pointStart.x * block) << "," << (S2.pointStart.y * block) << ")" << endl;
    file.close();
	
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================
}

int Core::getRandomNumber(){
	int result = rand();

	return result;
}

void Core::genIntersection(Share& S1, Share& S2){
	int widthArea, heightArea, startx, endx, starty, endy, A2X, B2X, A2Y, C2Y, minX, minY, maxX, maxY, avWidthArea, avHeightArea, widthIntersection, heightIntersection;
	vector<Point2i> pos1, pos2;

	widthArea	= (2*S2.rotWidthSS) + (S1.widthSS - (2*widthSI));
	heightArea	= (2*S2.rotHeightSS) + (S1.heightSS - (2*heightSI));

	avWidthArea		= widthArea - S2.rotWidthSS + widthSI;
	avHeightArea	= heightArea - S2.rotHeightSS + heightSI;

	pos1.push_back(Point(S2.rotWidthSS - widthSI, S2.rotHeightSS - heightSI));
	pos1.push_back(Point((S2.rotWidthSS - widthSI) + S1.widthSS, S2.rotHeightSS - heightSI));
	pos1.push_back(Point(S2.rotWidthSS - widthSI, (S2.rotHeightSS - heightSI) + S1.heightSS));
	pos1.push_back(Point((S2.rotWidthSS - widthSI) + S1.widthSS, (S2.rotHeightSS - heightSI) + S1.heightSS));

	int flagW;
	flagW	= 0;
	while(flagW == 0){
		int rx;

		rx	= getRandomNumber();
		A2X	= rx % avWidthArea;
		B2X	= A2X + S2.rotWidthSS;

		if((pos1[0].x >= A2X)&&(pos1[0].x < B2X)&&(pos1[1].x >= B2X)){
			startx	= pos1[0].x;
			endx	= B2X;
			minX	= A2X;
			maxX	= pos1[1].x;
		} else {
			if((pos1[0].x >= A2X)&&(pos1[0].x < B2X)&&(pos1[1].x < B2X)){
				startx	= pos1[0].x;
				endx	= pos1[1].x;
				minX	= A2X;
				maxX	= B2X;
			} else {
				if((pos1[0].x <= A2X)&&(pos1[1].x >= B2X)){
					startx	= A2X;
					endx	= B2X;
					minX	= pos1[0].x;
					maxX	= pos1[1].x;
				} else {
					if((pos1[0].x <= A2X)&&(pos1[1].x < B2X)&&(pos1[1].x > A2X)){
						startx	= A2X;
						endx	= pos1[1].x;
						minX	= pos1[0].x;
						maxX	= B2X;
					}
				}
			}
		}

		widthIntersection	= endx - startx;

		if(widthIntersection >= widthSI){
			flagW = 1;
		}
	}

	int flagH;
	flagH	= 0;
	while(flagH == 0){
		int ry;

		ry	= getRandomNumber();
		A2Y	= ry % avHeightArea;
		C2Y	= A2Y + S2.rotHeightSS;

		if((pos1[0].y >= A2Y)&&(pos1[0].y < C2Y)&&(pos1[2].y >= C2Y)){
			starty	= pos1[0].y;
			endy	= C2Y;
			minY	= A2Y;
			maxY	= pos1[2].y;
		} else {
			if((pos1[0].y >= A2Y)&&(pos1[0].y < C2Y)&&(pos1[2].y < C2Y)){
				starty	= pos1[0].y;
				endy	= pos1[2].y;
				minY	= A2Y;
				maxY	= C2Y;
			} else {
				if((pos1[0].y <= A2Y)&&(pos1[2].y >= C2Y)){
					starty	= A2Y;
					endy	= C2Y;
					minY	= pos1[0].y;
					maxY	= pos1[2].y;
				} else {
					if((pos1[0].y <= A2Y)&&(pos1[2].y < C2Y)&&(pos1[2].y > A2Y)){
						starty	= A2Y;
						endy	= pos1[2].y;
						minY	= pos1[0].y;
						maxY	= C2Y;
					}
				}
			}
		}

		heightIntersection	= endy - starty;

		if(heightIntersection >= heightSI){
			flagH = 1;
		}
	}

	pos2.push_back(Point(A2X, A2Y));
	pos2.push_back(Point(A2X + S1.widthSS, A2Y));
	pos2.push_back(Point(A2X, A2Y + S1.heightSS));
	pos2.push_back(Point(A2X + S1.widthSS, A2Y + S1.heightSS));

	S1.setIntersection(startx - pos1[0].x, endx - pos1[0].x, starty - pos1[0].y, endy - pos1[0].y);
	S2.setIntersection(startx - pos2[0].x, endx - pos2[0].x, starty - pos2[0].y, endy - pos2[0].y);

	widthCanvas		= maxX - minX;
	heightCanvas	= maxY - minY;
	S1.pointStart	= Point(pos1[0].x - minX, pos1[0].y - minY);
	S2.pointStart	= Point(pos2[0].x - minX, pos2[0].y - minY);
}

void Core::randomSIPosition(Share& S1, Share& S2){
	int x, y, spaceX, spaceY;

	spaceX	= (S1.intersection[1].x - S1.intersection[0].x) - widthSI;
	spaceY	= (S1.intersection[2].y - S1.intersection[0].y) - heightSI;

	srand((unsigned)time(0));
	x	= (rand() % (spaceX+1));
	y	= (rand() % (spaceY+1));

	S1.position.push_back(Point(S1.intersection[0].x + x, S1.intersection[0].y + y));
	S1.position.push_back(Point(S1.intersection[0].x + x + widthSI, S1.intersection[0].y + y));
	S1.position.push_back(Point(S1.intersection[0].x + x, S1.intersection[0].y + y + heightSI));
	S1.position.push_back(Point(S1.intersection[0].x + x + widthSI, S1.intersection[0].y + y + heightSI));

	S2.position.push_back(Point(S2.intersection[0].x + x, S2.intersection[0].y + y));
	S2.position.push_back(Point(S2.intersection[0].x + x + widthSI, S2.intersection[0].y + y));
	S2.position.push_back(Point(S2.intersection[0].x + x, S2.intersection[0].y + y + heightSI));
	S2.position.push_back(Point(S2.intersection[0].x + x + widthSI, S2.intersection[0].y + y + heightSI));
}

void Core::previewIA(Share S1, Share S2){
	Mat prevIA = generateNewImg(heightCanvas, widthCanvas, 3);

	//draw Area1 SS1
	for(int i = 0; i < S1.heightSS; i++){
		for(int j = 0; j < S1.widthSS; j++){
			int doit = 0;
			// assign matrix m except intersection area
			if((i >= S1.intersection[0].y) && (i < S1.intersection[2].y)){
				if((j < S1.intersection[0].x) || (j >= S1.intersection[1].x)){
					doit = 1;
				}
			} else {
				doit = 1;
			}
			if(doit){
				prevIA.at<Vec3b>(i + S1.pointStart.y,j + S1.pointStart.x)[0] = 65;
				prevIA.at<Vec3b>(i + S1.pointStart.y,j + S1.pointStart.x)[1] = 69;
				prevIA.at<Vec3b>(i + S1.pointStart.y,j + S1.pointStart.x)[2] = 214;
			}
		}
	}

	//draw Area1 SS2
	for(int i = 0; i < S2.rotHeightSS; i++){
		for(int j = 0; j < S2.rotWidthSS; j++){
			int doit = 0;
			// assign matrix m except intersection area
			if((i >= S2.intersection[0].y) && (i < S2.intersection[2].y)){
				if((j < S2.intersection[0].x) || (j >= S2.intersection[1].x)){
					doit = 1;
				}
			} else {
				doit = 1;
			}
			if(doit){
				prevIA.at<Vec3b>(i + S2.pointStart.y,j + S2.pointStart.x)[0] = 155;
				prevIA.at<Vec3b>(i + S2.pointStart.y,j + S2.pointStart.x)[1] = 188;
				prevIA.at<Vec3b>(i + S2.pointStart.y,j + S2.pointStart.x)[2] = 27;
			}
		}
	}

	int siCoordinateY	= S1.position[0].y - S1.intersection[0].y;
	int siCoordinateX	= S1.position[0].x - S1.intersection[0].x;
	// inside intersection area but outside SI
	for(int i = 0; i < (S1.intersection[2].y - S1.intersection[0].y); i++){
		for(int j = 0; j < (S1.intersection[1].x - S1.intersection[0].x); j++){
			int doit			= 0;
			// assign matrix m except intersection area
			if((i >= siCoordinateY) && (i < (siCoordinateY + heightSI))){
				if((j<siCoordinateX) || (j>=siCoordinateX + widthSI)){
					doit = 1;
				}
			} else {
				doit = 1;
			}
			if(doit){
				prevIA.at<Vec3b>(i + S1.pointStart.y + S1.intersection[0].y,j + S1.pointStart.x  + S1.intersection[0].x)[0] = 63;
				prevIA.at<Vec3b>(i + S1.pointStart.y + S1.intersection[0].y,j + S1.pointStart.x  + S1.intersection[0].x)[1] = 208;
				prevIA.at<Vec3b>(i + S1.pointStart.y + S1.intersection[0].y,j + S1.pointStart.x  + S1.intersection[0].x)[2] = 244;
			}
		}
	}

	for(int i = 0; i < heightSI; i++){
		for(int j = 0; j < widthSI; j++){
			prevIA.at<Vec3b>(i + S1.position[0].y + S1.pointStart.y,j + S1.position[0].x + S1.pointStart.x)[0] = 137;
			prevIA.at<Vec3b>(i + S1.position[0].y + S1.pointStart.y,j + S1.position[0].x + S1.pointStart.x)[1] = 122;
			prevIA.at<Vec3b>(i + S1.position[0].y + S1.pointStart.y,j + S1.position[0].x + S1.pointStart.x)[2] = 108;
		}
	}

	if(optOthers == 1){
		imwrite("result/encoding/others/previewIA.png",prevIA);
	}

	if(optPreview == 1){
		namedWindow("preview IA", 0);
		if(widthCanvas > heightCanvas){
			resizeWindow("preview IA", 600, ceil(((float)heightCanvas/(float)((float)widthCanvas/(float)600))));
		} else {
			resizeWindow("preview IA", ceil(((float)widthCanvas/(float)((float)heightCanvas/(float)600))), 600);
		}
		imshow("preview IA",prevIA);
		waitKey(300);
	}
}

void Core::generateShareNIMSVCS(Share& S1, Share& S2){
	Mat C1			= generateNewImg(S1.heightSS*block, S1.widthSS*block, 1);
	Mat A1			= Mat::zeros(S1.heightSS*block, S1.widthSS*block, CV_8UC1);
	Mat C2			= generateNewImg(S2.rotHeightSS*block, S2.rotWidthSS*block, 1);
	Mat A2			= Mat::zeros(S2.rotHeightSS*block, S2.rotWidthSS*block, CV_8UC1);

	vector<Mat> channels1, channels2;
	unsigned seed;

	// outside intersection area for SS1
	// :::::::::::::::::::::::::::::::::::::::::
	cout << "\nGenerating area 1 of S1" << endl;
	// :::::::::::::::::::::::::::::::::::::::::
	for(int i = 0; i < S1.heightSS; i++){
		for(int j = 0; j < S1.widthSS; j++){
			int doit = 0;
			// assign matrix m except intersection area
			if((i >= S1.intersection[0].y) && (i < S1.intersection[2].y)){
				if((j < S1.intersection[0].x) || (j >= S1.intersection[1].x)){
					doit = 1;
				}
			} else {
				doit = 1;
			}
			if(doit){
				if(optShift == 2){
					// obtain a time-based seed:
					// random permutation on 'position' to get matrix c
					seed = (chrono::system_clock::now().time_since_epoch().count())+j;
					shuffle (posS2.begin(), posS2.end(), default_random_engine(seed));

					int C[2][4];
					for(int u = 0; u < 2; u++){
						for(int v = 0; v < 4; v++){
							C[u][v] = M0S2[u][v];
						}
					}

					// transform 1D to 2D matrix m
					int m[2][2];
					for(int l = 0; l < 4; l++){
						int rowm		= l/2;
						m[rowm][l % 2]	= C[0][posS2[l]];
					}

					// asign matrix m to the share
					int marky = 0;
					for(int newy = i*block; newy < (i*block)+block; newy++){
						int markx = 0;
						for(int newx = j*block; newx < (j*block)+block; newx++){
							C1.at<uchar>(newy,newx) = m[marky][markx];
							if(m[marky][markx] == 255){
								A1.at<uchar>(newy,newx) = 0;
							} else {
								A1.at<uchar>(newy,newx) = 255;
							}
							markx = markx + 1;
						}
						marky = marky + 1;
					}
				} else {
					if(optShift == 3){
						// obtain a time-based seed:
						// random permutation on 'position' to get matrix c
						seed = (chrono::system_clock::now().time_since_epoch().count())+j;
						shuffle (posS3.begin(), posS3.end(), default_random_engine(seed));

						int C[2][9];
						for(int u = 0; u < 2; u++){
							for(int v = 0; v < 9; v++){
								C[u][v] = M0S3[u][v];
							}
						}

						// transform 1D to 2D matrix m
						int m[3][3];
						for(int l = 0; l < 9; l++){
							int rowm		= l/3;
							m[rowm][l % 3]	= C[0][posS3[l]];
						}

						// asign matrix m to the share
						int marky = 0;
						for(int newy = i*block; newy < (i*block)+block; newy++){
							int markx = 0;
							for(int newx = j*block; newx < (j*block)+block; newx++){
								C1.at<uchar>(newy,newx) = m[marky][markx];
								if(m[marky][markx] == 255){
									A1.at<uchar>(newy,newx) = 0;
								} else {
									A1.at<uchar>(newy,newx) = 255;
								}
								markx = markx + 1;
							}
							marky = marky + 1;
						}
					} else {
						if(optShift == 8){
							// obtain a time-based seed:
							// random permutation on 'position' to get matrix c
							seed = (chrono::system_clock::now().time_since_epoch().count())+j;
							shuffle (posS8.begin(), posS8.end(), default_random_engine(seed));

							int C[2][16];
							for(int u = 0; u < 2; u++){
								for(int v = 0; v < 16; v++){
									C[u][v] = M0S8[u][v];
								}
							}

							// transform 1D to 2D matrix m
							int m[4][4];
							for(int l = 0; l < 16; l++){
								int rowm		= l/4;
								m[rowm][l % 4]	= C[0][posS8[l]];
							}

							// asign matrix m to the share
							int marky = 0;
							for(int newy = i*block; newy < (i*block)+block; newy++){
								int markx = 0;
								for(int newx = j*block; newx < (j*block)+block; newx++){
									C1.at<uchar>(newy,newx) = m[marky][markx];
									if(m[marky][markx] == 255){
										A1.at<uchar>(newy,newx) = 0;
									} else {
										A1.at<uchar>(newy,newx) = 255;
									}
									markx = markx + 1;
								}
								marky = marky + 1;
							}
						}
					}
				}
			}
		}
	}

	// outside intersection area for SS2
	// :::::::::::::::::::::::::::::::::::::::::
	cout << "\nGenerating area 1 of S2" << endl;
	// :::::::::::::::::::::::::::::::::::::::::
	for(int i = 0; i < S2.rotHeightSS; i++){
		for(int j = 0; j < S2.rotWidthSS; j++){
			int doit = 0;
			// assign matrix m except intersection area
			if((i >= S2.intersection[0].y) && (i < S2.intersection[2].y)){
				if((j < S2.intersection[0].x) || (j >= S2.intersection[1].x)){
					doit = 1;
				}
			} else {
				doit = 1;
			}
			if(doit){
				if(optShift == 2){
					// obtain a time-based seed:
					// random permutation on 'position' to get matrix c
					seed = (chrono::system_clock::now().time_since_epoch().count())+j;
					shuffle (posS2.begin(), posS2.end(), default_random_engine(seed));

					int C[2][4];
					for(int u = 0; u < 2; u++){
						for(int v = 0; v < 4; v++){
							C[u][v] = M0S2[u][v];
						}
					}

					// transform 1D to 2D matrix m
					int m[2][2];
					for(int l = 0; l < 4; l++){
						int rowm		= l/2;
						m[rowm][l % 2]	= C[1][posS2[l]];
					}

					// asign matrix m to the share
					int marky = 0;
					for(int newy = i*block; newy < (i*block)+block; newy++){
						int markx = 0;
						for(int newx = j*block; newx < (j*block)+block; newx++){
							C2.at<uchar>(newy,newx) = m[marky][markx];
							if(m[marky][markx] == 255){
								A2.at<uchar>(newy,newx) = 0;
							} else {
								A2.at<uchar>(newy,newx) = 255;
							}
							markx = markx + 1;
						}
						marky = marky + 1;
					}
				} else {
					if(optShift == 3){
						// obtain a time-based seed:
						// random permutation on 'position' to get matrix c
						seed = (chrono::system_clock::now().time_since_epoch().count())+j;
						shuffle (posS3.begin(), posS3.end(), default_random_engine(seed));

						int C[2][9];
						for(int u = 0; u < 2; u++){
							for(int v = 0; v < 9; v++){
								C[u][v] = M0S3[u][v];
							}
						}

						// transform 1D to 2D matrix m
						int m[3][3];
						for(int l = 0; l < 9; l++){
							int rowm		= l/3;
							m[rowm][l % 3]	= C[1][posS3[l]];
						}

						// asign matrix m to the share
						int marky = 0;
						for(int newy = i*block; newy < (i*block)+block; newy++){
							int markx = 0;
							for(int newx = j*block; newx < (j*block)+block; newx++){
								C2.at<uchar>(newy,newx) = m[marky][markx];
								if(m[marky][markx] == 255){
									A2.at<uchar>(newy,newx) = 0;
								} else {
									A2.at<uchar>(newy,newx) = 255;
								}
								markx = markx + 1;
							}
							marky = marky + 1;
						}
					} else {
						if(optShift == 8){
							// obtain a time-based seed:
							// random permutation on 'position' to get matrix c
							seed = (chrono::system_clock::now().time_since_epoch().count())+j;
							shuffle (posS8.begin(), posS8.end(), default_random_engine(seed));

							int C[2][16];
							for(int u = 0; u < 2; u++){
								for(int v = 0; v < 16; v++){
									C[u][v] = M0S8[u][v];
								}
							}

							// transform 1D to 2D matrix m
							int m[4][4];
							for(int l = 0; l < 16; l++){
								int rowm		= l/4;
								m[rowm][l % 4]	= C[1][posS8[l]];
							}

							// asign matrix m to the share
							int marky = 0;
							for(int newy = i*block; newy < (i*block)+block; newy++){
								int markx = 0;
								for(int newx = j*block; newx < (j*block)+block; newx++){
									C2.at<uchar>(newy,newx) = m[marky][markx];
									if(m[marky][markx] == 255){
										A2.at<uchar>(newy,newx) = 0;
									} else {
										A2.at<uchar>(newy,newx) = 255;
									}
									markx = markx + 1;
								}
								marky = marky + 1;
							}
						}
					}
				}
			}
		}
	}

	if(optPreview == 1){
		namedWindow("preview generating S1", 0);
		if(C1.cols > C1.rows){
			resizeWindow("preview generating S1", 500, ceil(((float)C1.rows/(float)((float)C1.cols/(float)500))));
		} else {
			resizeWindow("preview generating S1", ceil(((float)C1.cols/(float)((float)C1.rows/(float)500))), 500);
		}
		moveWindow("preview generating S1", 0, 0);
		imshow("preview generating S1",C1);

		namedWindow("preview generating rot S2", 0);
		if(C2.cols > C2.rows){
			resizeWindow("preview generating rot S2", 500, ceil(((float)C2.rows/(float)((float)C2.cols/(float)500))));
		} else {
			resizeWindow("preview generating rot S2", ceil(((float)C2.cols/(float)((float)C2.rows/(float)500))), 500);
		}
		moveWindow("preview generating rot S2", 500, 0);
		imshow("preview generating rot S2",C2);
		waitKey(300);
	}

	if(optOthers == 1){
		imwrite("result/encoding/others/1_S1.png",C1);
		imwrite("result/encoding/others/1_S2.png",C2);
	}

	int siCoordinateY	= S1.position[0].y - S1.intersection[0].y;
	int siCoordinateX	= S1.position[0].x - S1.intersection[0].x;
	// inside intersection area but outside SI
	// :::::::::::::::::::::::::::::::::::::::::
	cout << "\nGenerating area 2" << endl;
	// :::::::::::::::::::::::::::::::::::::::::
	for(int i = 0; i < (S1.intersection[2].y - S1.intersection[0].y); i++){
		for(int j = 0; j < (S1.intersection[1].x - S1.intersection[0].x); j++){
			int doit			= 0;
			// assign matrix m except intersection area
			if((i >= siCoordinateY) && (i < (siCoordinateY + heightSI))){
				if((j < siCoordinateX) || (j >= siCoordinateX + widthSI)){
					doit = 1;
				}
			} else {
				doit = 1;
			}
			if(doit){
				if(optShift == 2){
					// obtain a time-based seed:
					// random permutation on 'position' to get matrix c
					seed = (chrono::system_clock::now().time_since_epoch().count())+j;
					shuffle (posS2.begin(), posS2.end(), default_random_engine(seed));

					int C[2][4];
					for(int u = 0; u < 2; u++){
						for(int v = 0; v < 4; v++){
							C[u][v] = M0S2[u][v];
						}
					}

					// transform 1D to 2D matrix m
					int m_1[2][2],m_2[2][2];
					for(int l = 0; l < 4; l++){
						int rowm		= l/2;
						m_1[rowm][l % 2]	= C[0][posS2[l]];
						m_2[rowm][l % 2]	= C[1][posS2[l]];
					}

					// asign matrix m to the share
					int marky = 0;
					for(int newy = i*block; newy < (i*block)+block; newy++){
						int markx = 0;
						for(int newx = j*block; newx < (j*block)+block; newx++){
							C1.at<uchar>(newy + (S1.intersection[0].y*block),newx + (S1.intersection[0].x*block)) = m_1[marky][markx];
							if(m_1[marky][markx] == 255){
								A1.at<uchar>(newy + (S1.intersection[0].y*block),newx + (S1.intersection[0].x*block)) = 0;
							} else {
								A1.at<uchar>(newy + (S1.intersection[0].y*block),newx + (S1.intersection[0].x*block)) = 255;
							}
						
							C2.at<uchar>(newy + (S2.intersection[0].y*block),newx + (S2.intersection[0].x*block)) = m_2[marky][markx];
							if(m_2[marky][markx] == 255){
								A2.at<uchar>(newy + (S2.intersection[0].y*block),newx + (S2.intersection[0].x*block)) = 0;
							} else {
								A2.at<uchar>(newy + (S2.intersection[0].y*block),newx + (S2.intersection[0].x*block)) = 255;
							}
							markx = markx + 1;
						}
						marky = marky + 1;
					}
				} else {
					if(optShift == 3){
						// obtain a time-based seed:
						// random permutation on 'position' to get matrix c
						seed = (chrono::system_clock::now().time_since_epoch().count())+j;
						shuffle (posS3.begin(), posS3.end(), default_random_engine(seed));

						int C[2][9];
						for(int u = 0; u < 2; u++){
							for(int v = 0; v < 9; v++){
								C[u][v] = M0S3[u][v];
							}
						}

						// transform 1D to 2D matrix m
						int m_1[3][3],m_2[3][3];
						for(int l = 0; l < 9; l++){
							int rowm		= l/3;
							m_1[rowm][l % 3]	= C[0][posS3[l]];
							m_2[rowm][l % 3]	= C[1][posS3[l]];
						}

						// asign matrix m to the share
						int marky = 0;
						for(int newy = i*block; newy < (i*block)+block; newy++){
							int markx = 0;
							for(int newx = j*block; newx < (j*block)+block; newx++){
								C1.at<uchar>(newy + (S1.intersection[0].y*block),newx + (S1.intersection[0].x*block)) = m_1[marky][markx];
								if(m_1[marky][markx] == 255){
									A1.at<uchar>(newy + (S1.intersection[0].y*block),newx + (S1.intersection[0].x*block)) = 0;
								} else {
									A1.at<uchar>(newy + (S1.intersection[0].y*block),newx + (S1.intersection[0].x*block)) = 255;
								}
						
								C2.at<uchar>(newy + (S2.intersection[0].y*block),newx + (S2.intersection[0].x*block)) = m_2[marky][markx];
								if(m_2[marky][markx] == 255){
									A2.at<uchar>(newy + (S2.intersection[0].y*block),newx + (S2.intersection[0].x*block)) = 0;
								} else {
									A2.at<uchar>(newy + (S2.intersection[0].y*block),newx + (S2.intersection[0].x*block)) = 255;
								}
								markx = markx + 1;
							}
							marky = marky + 1;
						}
					} else {
						if(optShift == 8){
							// obtain a time-based seed:
							// random permutation on 'position' to get matrix c
							seed = (chrono::system_clock::now().time_since_epoch().count())+j;
							shuffle (posS8.begin(), posS8.end(), default_random_engine(seed));

							int C[2][16];
							for(int u = 0; u < 2; u++){
								for(int v = 0; v < 16; v++){
									C[u][v] = M0S8[u][v];
								}
							}

							// transform 1D to 2D matrix m
							int m_1[4][4],m_2[4][4];
							for(int l = 0; l < 16; l++){
								int rowm		= l/4;
								m_1[rowm][l % 4]	= C[0][posS8[l]];
								m_2[rowm][l % 4]	= C[1][posS8[l]];
							}

							// asign matrix m to the share
							int marky = 0;
							for(int newy = i*block; newy < (i*block)+block; newy++){
								int markx = 0;
								for(int newx = j*block; newx < (j*block)+block; newx++){
									C1.at<uchar>(newy + (S1.intersection[0].y*block),newx + (S1.intersection[0].x*block)) = m_1[marky][markx];
									if(m_1[marky][markx] == 255){
										A1.at<uchar>(newy + (S1.intersection[0].y*block),newx + (S1.intersection[0].x*block)) = 0;
									} else {
										A1.at<uchar>(newy + (S1.intersection[0].y*block),newx + (S1.intersection[0].x*block)) = 255;
									}
						
									C2.at<uchar>(newy + (S2.intersection[0].y*block),newx + (S2.intersection[0].x*block)) = m_2[marky][markx];
									if(m_2[marky][markx] == 255){
										A2.at<uchar>(newy + (S2.intersection[0].y*block),newx + (S2.intersection[0].x*block)) = 0;
									} else {
										A2.at<uchar>(newy + (S2.intersection[0].y*block),newx + (S2.intersection[0].x*block)) = 255;
									}
									markx = markx + 1;
								}
								marky = marky + 1;
							}
						}
					}
				}
			}
		}
	}

	if(optOthers == 1){
		imwrite("result/encoding/others/2_S1.png",C1);
		imwrite("result/encoding/others/2_S2.png",C2);
	}

	if(optPreview == 1){
		imshow("preview generating S1",C1);
		imshow("preview generating rot S2",C2);
		waitKey(300);
	}

	// :::::::::::::::::::::::::::::::::::::::::
	cout << "\nGenerating area 3" << endl;
	// :::::::::::::::::::::::::::::::::::::::::
	for(int i = 0; i < heightSI; i++){
		for(int j = 0; j < widthSI; j++){
			if(optShift == 2){
				// obtain a time-based seed:
				// random permutation on 'position' to get matrix c
				seed = (chrono::system_clock::now().time_since_epoch().count())+j;
				shuffle (posS2.begin(), posS2.end(), default_random_engine(seed));
				int C[2][4];

				if(filteredSI.at<uchar>(i,j) < 128){
					for(int u = 0; u < 2; u++){
						for(int v = 0; v < 4; v++){
							C[u][v] = M1S2[u][v];
						}
					}
				} else {
					for(int u = 0; u < 2; u++){
						for(int v = 0; v < 4; v++){
							C[u][v] = M0S2[u][v];
						}
					}
				}
				// transform 1D to 2D matrix m
				int m_1[2][2],m_2[2][2];
				for(int l = 0; l < 4; l++){
					int rowm		= l/2;
					m_1[rowm][l % 2]	= C[0][posS2[l]];
					m_2[rowm][l % 2]	= C[1][posS2[l]];
				}
				// asign matrix m to the share
				int marky = 0;
				for(int newy = i*block; newy < (i*block)+block; newy++){
					int markx = 0;
					for(int newx = j*block; newx < (j*block)+block; newx++){
						C1.at<uchar>(newy + ((S1.intersection[0].y + siCoordinateY)*block),newx + ((S1.intersection[0].x + siCoordinateX)*block)) = m_1[marky][markx];
						if(m_1[marky][markx] == 255){
							A1.at<uchar>(newy + ((S1.intersection[0].y + siCoordinateY)*block),newx + ((S1.intersection[0].x + siCoordinateX)*block)) = 0;
						} else {
							A1.at<uchar>(newy + ((S1.intersection[0].y + siCoordinateY)*block),newx + ((S1.intersection[0].x + siCoordinateX)*block)) = 255;
						}
					
						C2.at<uchar>(newy + ((S2.intersection[0].y + siCoordinateY)*block),newx + ((S2.intersection[0].x + siCoordinateX)*block)) = m_2[marky][markx];
						if(m_2[marky][markx] == 255){
							A2.at<uchar>(newy + ((S2.intersection[0].y + siCoordinateY)*block),newx + ((S2.intersection[0].x + siCoordinateX)*block)) = 0;
						} else {
							A2.at<uchar>(newy + ((S2.intersection[0].y + siCoordinateY)*block),newx + ((S2.intersection[0].x + siCoordinateX)*block)) = 255;
						}
						markx = markx + 1;
					}
					marky = marky + 1;
				}
			} else {
				if(optShift == 3){
					// obtain a time-based seed:
					// random permutation on 'position' to get matrix c
					seed = (chrono::system_clock::now().time_since_epoch().count())+j;
					shuffle (posS3.begin(), posS3.end(), default_random_engine(seed));
					int C[2][9];

					if(filteredSI.at<uchar>(i,j) < 128){
						for(int u = 0; u < 2; u++){
							for(int v = 0; v < 9; v++){
								C[u][v] = M1S3[u][v];
							}
						}
					} else {
						for(int u = 0; u < 2; u++){
							for(int v = 0; v < 9; v++){
								C[u][v] = M0S3[u][v];
							}
						}
					}
					// transform 1D to 2D matrix m
					int m_1[3][3],m_2[3][3];
					for(int l = 0; l < 9; l++){
						int rowm		= l/3;
						m_1[rowm][l % 3]	= C[0][posS3[l]];
						m_2[rowm][l % 3]	= C[1][posS3[l]];
					}
					// asign matrix m to the share
					int marky = 0;
					for(int newy = i*block; newy < (i*block)+block; newy++){
						int markx = 0;
						for(int newx = j*block; newx < (j*block)+block; newx++){
							C1.at<uchar>(newy + ((S1.intersection[0].y + siCoordinateY)*block),newx + ((S1.intersection[0].x + siCoordinateX)*block)) = m_1[marky][markx];
							if(m_1[marky][markx] == 255){
								A1.at<uchar>(newy + ((S1.intersection[0].y + siCoordinateY)*block),newx + ((S1.intersection[0].x + siCoordinateX)*block)) = 0;
							} else {
								A1.at<uchar>(newy + ((S1.intersection[0].y + siCoordinateY)*block),newx + ((S1.intersection[0].x + siCoordinateX)*block)) = 255;
							}
					
							C2.at<uchar>(newy + ((S2.intersection[0].y + siCoordinateY)*block),newx + ((S2.intersection[0].x + siCoordinateX)*block)) = m_2[marky][markx];
							if(m_2[marky][markx] == 255){
								A2.at<uchar>(newy + ((S2.intersection[0].y + siCoordinateY)*block),newx + ((S2.intersection[0].x + siCoordinateX)*block)) = 0;
							} else {
								A2.at<uchar>(newy + ((S2.intersection[0].y + siCoordinateY)*block),newx + ((S2.intersection[0].x + siCoordinateX)*block)) = 255;
							}
							markx = markx + 1;
						}
						marky = marky + 1;
					}
				} else {
					if(optShift == 8){
						// obtain a time-based seed:
						// random permutation on 'position' to get matrix c
						seed = (chrono::system_clock::now().time_since_epoch().count())+j;
						shuffle (posS8.begin(), posS8.end(), default_random_engine(seed));
						int C[2][16];

						if(filteredSI.at<uchar>(i,j) < 128){
							for(int u = 0; u < 2; u++){
								for(int v = 0; v < 16; v++){
									C[u][v] = M1S8[u][v];
								}
							}
						} else {
							for(int u = 0; u < 2; u++){
								for(int v = 0; v < 16; v++){
									C[u][v] = M0S8[u][v];
								}
							}
						}
						// transform 1D to 2D matrix m
						int m_1[4][4],m_2[4][4];
						for(int l = 0; l < 16; l++){
							int rowm		= l/4;
							m_1[rowm][l % 4]	= C[0][posS8[l]];
							m_2[rowm][l % 4]	= C[1][posS8[l]];
						}
						// asign matrix m to the share
						int marky = 0;
						for(int newy = i*block; newy < (i*block)+block; newy++){
							int markx = 0;
							for(int newx = j*block; newx < (j*block)+block; newx++){
								C1.at<uchar>(newy + ((S1.intersection[0].y + siCoordinateY)*block),newx + ((S1.intersection[0].x + siCoordinateX)*block)) = m_1[marky][markx];
								if(m_1[marky][markx] == 255){
									A1.at<uchar>(newy + ((S1.intersection[0].y + siCoordinateY)*block),newx + ((S1.intersection[0].x + siCoordinateX)*block)) = 0;
								} else {
									A1.at<uchar>(newy + ((S1.intersection[0].y + siCoordinateY)*block),newx + ((S1.intersection[0].x + siCoordinateX)*block)) = 255;
								}
					
								C2.at<uchar>(newy + ((S2.intersection[0].y + siCoordinateY)*block),newx + ((S2.intersection[0].x + siCoordinateX)*block)) = m_2[marky][markx];
								if(m_2[marky][markx] == 255){
									A2.at<uchar>(newy + ((S2.intersection[0].y + siCoordinateY)*block),newx + ((S2.intersection[0].x + siCoordinateX)*block)) = 0;
								} else {
									A2.at<uchar>(newy + ((S2.intersection[0].y + siCoordinateY)*block),newx + ((S2.intersection[0].x + siCoordinateX)*block)) = 255;
								}
								markx = markx + 1;
							}
							marky = marky + 1;
						}
					}
				}
			}
		}
	}

	if(optOthers == 1){
		imwrite("result/encoding/others/3_S1.png",C1);
		imwrite("result/encoding/others/3_S2.png",C2);
	}

	if(optPreview == 1){
		imshow("preview generating S1",C1);
		imshow("preview generating rot S2",C2);
		waitKey(300);
		if(optPause == 1){
			cout << "\n" ;
			system("pause");
		}
		destroyWindow("preview generating S1");
		destroyWindow("preview generating rot S2");
	}

	channels1.push_back(C1);
    channels1.push_back(C1);
    channels1.push_back(C1);
	channels1.push_back(A1);

	channels2.push_back(C2);
    channels2.push_back(C2);
    channels2.push_back(C2);
	channels2.push_back(A2);

	merge(channels1, S1.share);
	merge(channels2, S2.rotatedShare);

	if(optOthers == 1){
		imwrite("result/encoding/others/eS1.png", S1.share);
		imwrite("result/encoding/others/eS2.png", S2.rotatedShare);
	}
}

void Core::getNormalRotationImage(Share& S){
	if (S.revAngle == 0){
		S.share = S.rotatedShare.clone();
	} else {
		if (S.revAngle == 90){
			transpose(S.rotatedShare, S.share);  
			flip(S.share, S.share,1); //transpose+flip(1)=CW
		} else {
			if (S.revAngle == 270) {
				transpose(S.rotatedShare, S.share);  
				flip(S.share, S.share,0); //transpose+flip(0)=CCW
			} else {
				if (S.revAngle == 180){
					flip(S.rotatedShare, S.share,-1);    //flip(-1)=180
				}
			}
		}
	}
}

void Core::viewDecodingNIMSVCS(Share& S1,Share& S2){
	// =====================================================================================
	// ============================ ESTIMATING DECODING TIME ===============================
	// =====================================================================================
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "======================= ESTIMATING DECODING TIME ==============================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	viewOptionDecodingNIMSVCS();

	cout << "\n> Estimating total decoding time" << endl;
	long long int pos;

	pos = getPossibilities(S1,S2);
	cout << "\nPossibilities \t\t\t: " << pos << " positions" << endl;

	estimate1TimeRun(S1,S2);
	duration1run 	= duration1run + 1;
	printf("\nOne time run \t\t\t: %.2f seconds\n", duration1run);

	mpz_class poss(pos);
	mpz_class res;
	mpz_mul_ui(res.get_mpz_t(),poss.get_mpz_t(),(duration1run*10)+0.5);
	mpz_div_ui(res.get_mpz_t(),res.get_mpz_t(),10);
	cout << "\nTotal time run (in second) \t: " << res.get_ui() << " seconds" << endl;
	mpz_div_ui(res.get_mpz_t(),res.get_mpz_t(),60);
	cout << "\nTotal time run (in minutes) \t: " << res.get_ui() << " minutes" << endl;
	mpz_div_ui(res.get_mpz_t(),res.get_mpz_t(),60);
	cout << "\nTotal time run (in hours) \t: " << res.get_ui() << " hours" << endl;
	mpz_div_ui(res.get_mpz_t(),res.get_mpz_t(),24);
	cout << "\nTotal time run (in days) \t: " << res.get_ui() << " days" << endl;
	mpz_div_ui(res.get_mpz_t(),res.get_mpz_t(),365);
	cout << "\nTotal time run (in years) \t: " << res.get_ui() << " years" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if(optPause == 1){
		cout << "\n" ;
		system("pause");
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================

	// *************************************************************************************
	// ******************************** MANUAL STACKING ************************************
	// *************************************************************************************
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n*******************************************************************************" << endl;
	cout << "***************************** MANUAL STACKING *********************************" << endl;
	cout << "*******************************************************************************" << endl;
	// =====================================================================================
	// ==================================== STACKING =======================================
	// =====================================================================================
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "================================= STACKING ====================================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	manualStacking(S1,S2);

	imwrite("result/decoding/ri.png",canvas);

	if(optPreview == 1){
		namedWindow("preview reconstructed image", 0);
		if(canvas.cols > canvas.rows){
			resizeWindow("preview reconstructed image", 500, ceil(((float)canvas.rows/(float)((float)canvas.cols/(float)500))));
		} else {
			resizeWindow("preview reconstructed image", ceil(((float)canvas.cols/(float)((float)canvas.rows/(float)500))), 500);
		}
		imshow("preview reconstructed image",canvas);
		waitKey(300);
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if(optPause == 1){
		cout << "\n" ;
		system("pause");
	}
					
	if(optPreview == 1){
		destroyWindow("preview reconstructed image");
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================
	
	// =====================================================================================
	// ============================ ESTIMATING STACKING TIME ===============================
	// =====================================================================================
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cout << "\n===============================================================================" << endl;
	cout << "========================= ESTIMATING STACKING TIME ============================" << endl;
	cout << "===============================================================================" << endl;
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	getIndexEstimation(S1,S2);
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if(optPause == 1){
		cout << "\n" ;
		system("pause");
	}
	// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	// =====================================================================================
}

long long int Core::getPossibilities(Share S1, Share S2){
	long long int result;
	long int posX, posY;

	posX	= (S1.width/optBlock) + (S2.width/optBlock) - optMinWidth;
	posY	= (S1.height/optBlock) + (S2.height/optBlock) - optMinHeight;

	result	= (posX * posY) * 4;

	return result;
}

void Core::estimate1TimeRun(Share S1, Share S2){
	clock_t tStart;
	Point2i	pos1, pos2;
	int flag;

	tStart			= clock();
	S1.pointStart	= Point((S2.width/optBlock) - optMinWidth, (S2.height/optBlock) - optMinHeight);
	S2.pointStart	= Point(0,0);
	
	estimateCanvas(S1, S2, 0);
	estimateStack(S1, S2, 0);
	//flag			= checkPattern();

	duration1run	= (double)(clock() - tStart)/CLOCKS_PER_SEC;
}

void Core::estimateCanvas(Share& S1, Share& S2, int r){
	int A1X, B1X, A2X, B2X, minX, maxX;
	A1X		= S1.pointStart.x;
	B1X		= S1.pointStart.x + (S1.width/optBlock);
	A2X		= S2.pointStart.x;
	if(r == 0){
		B2X		= S2.pointStart.x + (S2.width/optBlock);
	} else {
		if(r == 1){
			B2X		= S2.pointStart.x + (S2.rotWidth/optBlock);
		}
	}
	if((A1X >= A2X)&&(A1X < B2X)&&(B1X >= B2X)){
		minX	= A2X;
		maxX	= B1X;
	} else {
		if((A1X >= A2X)&&(A1X < B2X)&&(B1X < B2X)){
			minX	= A2X;
			maxX	= B2X;
		} else {
			if((A1X <= A2X)&&(B1X >= B2X)){
				minX	= A1X;
				maxX	= B1X;
			} else {
				if((A1X <= A2X)&&(B1X < B2X)&&(B1X > A2X)){
					minX	= A1X;
					maxX	= B2X;
				}
			}
		}
	}

	int A1Y, C1Y, A2Y, C2Y, minY, maxY;
	A1Y		= S1.pointStart.y;
	C1Y		= S1.pointStart.y + (S1.height/optBlock);
	A2Y		= S2.pointStart.y;
	if(r == 0){
		C2Y		= S2.pointStart.y + (S2.height/optBlock);
	} else {
		if(r == 1){
			C2Y		= S2.pointStart.y + (S2.rotHeight/optBlock);
		}
	}
	if((A1Y >= A2Y)&&(A1Y < C2Y)&&(C1Y >= C2Y)){
		minY	= A2Y;
		maxY	= C1Y;
	} else {
		if((A1Y >= A2Y)&&(A1Y < C2Y)&&(C1Y < C2Y)){
			minY	= A2Y;
			maxY	= C2Y;
		} else {
			if((A1Y <= A2Y)&&(C1Y >= C2Y)){
				minY	= A1Y;
				maxY	= C1Y;
			} else {
				if((A1Y <= A2Y)&&(C1Y < C2Y)&&(C1Y > A2Y)){
					minY	= A1Y;
					maxY	= C2Y;
				}
			}
		}
	}

	widthCanvas		= (maxX - minX) * optBlock;
	heightCanvas	= (maxY - minY) * optBlock;
}

void Core::estimateStack(Share& S1, Share& S2, int r){
	int positiony, positionx;
	vector<Mat> channelsBB;
	Mat C	= generateNewImg(heightCanvas, widthCanvas, 1);
	Mat A	= Mat::zeros(heightCanvas, widthCanvas, CV_8UC1);
	positiony = 0;
	// stacking share 1 to big box
	for(int i = 0; i < heightCanvas; i++){
		positionx = 0;
		for(int j = 0; j < widthCanvas; j++){
			if((i >= (S1.pointStart.y * optBlock)) && (i < ((S1.pointStart.y * optBlock) + S1.height))){
				if((j >= (S1.pointStart.x * optBlock))&&(j < ((S1.pointStart.x * optBlock) + S1.width))){
					int currentP	= S1.channels[0].at<uchar>(positiony,positionx);
					if(currentP < 128){
						C.at<uchar>(i,j)	= S1.channels[0].at<uchar>(positiony,positionx);
						A.at<uchar>(i,j)	= 255;
					}
					positionx				= positionx + 1;
				}
			}
		}
		if((i >= (S1.pointStart.y * optBlock))&&(i < ((S1.pointStart.y * optBlock) + S1.height))){
			positiony = positiony + 1;
		}
	}
	// stacking share 2 to big box
	positiony = 0;
	for(int i = 0; i < heightCanvas; i++){
		positionx = 0;
		for(int j = 0; j < widthCanvas; j++){
			if(r == 0){
				if((i >= (S2.pointStart.y * optBlock)) && (i < ((S2.pointStart.y * optBlock) + S2.height))){
					if((j >= (S2.pointStart.x * optBlock)) && (j < ((S2.pointStart.x * optBlock) + S2.width))){
						int currentP	= S2.channels[0].at<uchar>(positiony,positionx);
						if(currentP < 128){
							C.at<uchar>(i,j)	= S2.channels[0].at<uchar>(positiony,positionx);
							A.at<uchar>(i,j)	= 255;
						}
						positionx				= positionx + 1;
					}
				}
			} else {
				if(r == 1){
					if((i >= (S2.pointStart.y * optBlock)) && (i < ((S2.pointStart.y * optBlock) + S2.rotHeight))){
						if((j >= (S2.pointStart.x * optBlock)) && (j < ((S2.pointStart.x * optBlock) + S2.rotWidth))){
							int currentP	= S2.rotChannels[0].at<uchar>(positiony,positionx);
							if(currentP < 128){
								C.at<uchar>(i,j)	= S2.rotChannels[0].at<uchar>(positiony,positionx);
								A.at<uchar>(i,j)	= 255;
							}
							positionx				= positionx + 1;
						}
					}
				}
			}
		}
		if(r == 0){
			if((i >= (S2.pointStart.y * optBlock))&&(i < ((S2.pointStart.y * optBlock) + S2.height))){
				positiony = positiony + 1;
			}
		} else {
			if(r == 1){
				if((i >= (S2.pointStart.y * optBlock))&&(i < ((S2.pointStart.y * optBlock) + S2.rotHeight))){
					positiony = positiony + 1;
				}
			}
		}
	}
	channelsBB.push_back(C);
    channelsBB.push_back(C);
    channelsBB.push_back(C);
	channelsBB.push_back(A);

	merge(channelsBB, canvas);
}

int Core::checkPattern(){
	vector<Mat> channels(4);
	split(canvas,channels);
	int result = 0;
	Mat blurIm, resIm;
	blur(channels[0],blurIm,Size(3,3));
	resize(blurIm,resIm,Size(channels[0].cols/2,channels[0].rows/2),1);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours( resIm, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	
    for(size_t i = 0; i < contours.size(); i++){
        if((hierarchy[i][2] == -1)&&(hierarchy[i][3] != -1)){
			int C		= i;
			int B		= hierarchy[i][3];
			if(hierarchy[B][3] != -1){
				int A		= hierarchy[B][3];
				Point2f cornersA[4],cornersB[4],cornersC[4];

				RotatedRect boundingBox = minAreaRect(contours[A]);
				boundingBox.points(cornersA);
				vector<int> tmpA	= minMaxX(cornersA);

				boundingBox			= minAreaRect(contours[B]);
				boundingBox.points(cornersB);
				vector<int> tmpB	= minMaxX(cornersB);

				boundingBox			= minAreaRect(contours[C]);
				boundingBox.points(cornersC);
				vector<int> tmpC	= minMaxX(cornersC);

				int long1, long2, long3, long4, long5;
				int ratio1, ratio2, ratio3, ratio4, ratio5;

				long1	= tmpB[0] - tmpA[0];
				long2	= tmpC[0] - tmpB[0];
				long3	= tmpC[1] - tmpC[0];
				long4	= tmpB[1] - tmpC[1];
				long5	= tmpA[1] - tmpB[1];

				ratio1	= floor(((float)long1/(float)long2)+0.5);
				ratio2	= floor(((float)long2/(float)long1)+0.5);
				ratio3	= floor(((float)long3/(float)long1)+0.5);
				ratio4	= floor(((float)long4/(float)long5)+0.5);
				ratio5	= floor(((float)long5/(float)long4)+0.5);

				if((ratio1 == 1)&&(ratio2 == 1)&&(ratio3 == 3)&&(ratio4 == 1)&&(ratio5 == 1)){
					result	= 1;
				}
			}
		}
	}
	return result;
}

vector<int> Core::minMaxX(Point2f corners[4]) {
	vector<int> result;
	int max		= 0;
    for (int i = 0; i < 4; i++) {
        if(corners[i].x > max){
			max = corners[i].x;
		}
    }

	int min		= corners[0].x;
	for (int i = 1; i < 4; i++) {
        if(corners[i].x < min){
			min = corners[i].x;
		}
    }

	result.push_back(min);
	result.push_back(max);

    return result;
}

void Core::viewOptionDecodingNIMSVCS(){
	setOptionBlock();
	setOptionMinWidth();
	setOptionMinHeight();
}

void Core::setOptionBlock(){
	int flag	= 0;
	int ans;
	while(flag == 0){
		cout << "\n> Block pixel is known (1) yes (2) no : ";
		cin >> ans;
		if(ans == 1){
			int ansBlock;
			cout << "\n> Block size : ";
			cin >> ansBlock;
			optBlock	= ansBlock;
			flag		= 1;
		} else {
			if(ans == 2){
				flag		= 1;
			} else {
				cout << "\nPlease input (1) or (2)";
			}
		}
	}
}

void Core::setOptionMinWidth(){
	int flag	= 0;
	int ans;
	while(flag == 0){
		cout << "\n> Minimum width of SI is known (1) yes (2) no : ";
		cin >> ans;
		if(ans == 1){
			int ansBlock;
			cout << "\n> Minimum block width : ";
			cin >> ansBlock;
			optMinWidth	= ansBlock;
			flag		= 1;
		} else {
			if(ans == 2){
				flag		= 1;
			} else {
				cout << "\nPlease input (1) or (2)";
			}
		}
	}
}

void Core::setOptionMinHeight(){
	int flag	= 0;
	int ans;
	while(flag == 0){
		cout << "\n> Minimum height of SI is known (1) yes (2) no : ";
		cin >> ans;
		if(ans == 1){
			int ansBlock;
			cout << "\n> Minimum block height : ";
			cin >> ansBlock;
			optMinHeight	= ansBlock;
			flag		= 1;
		} else {
			if(ans == 2){
				flag		= 1;
			} else {
				cout << "\nPlease input (1) or (2)";
			}
		}
	}
}

void Core::manualStacking(Share& S1, Share& S2){
	S2.setAngleRotation();
	cout << "\n> Rotating Normal 3OP of S2" << endl;

	getRotationImage(S2);
	S2.getRotationSize();

	S1.setStartPosition();
	S2.setStartPosition();

	int bOptBlock, bOptMinWidth, bOptMinHeight;
	bOptBlock		= optBlock;
	bOptMinWidth	= optMinWidth;
	bOptMinHeight	= optMinHeight;
	resetOptEstimation();

	estimateCanvas(S1, S2, 1);

	cout << "\n> Stacking The Shares" << endl;
	estimateStack(S1, S2, 1);

	setOptEstimation(bOptBlock,bOptMinWidth,bOptMinHeight);
}

void Core::getRotationImage(Share& S){
	vector<Mat> tempChannels(4);
	
	if (S.angle == 0){
		S.rotatedShare = S.share.clone();
	} else {
		if (S.angle == 90){
			transpose(S.share, S.rotatedShare);  
			flip(S.rotatedShare, S.rotatedShare,1); //transpose+flip(1)=CW
		} else {
			if (S.angle == 270) {
				transpose(S.share, S.rotatedShare);  
				flip(S.rotatedShare, S.rotatedShare,0); //transpose+flip(0)=CCW
			} else {
				if (S.angle == 180){
					flip(S.share, S.rotatedShare,-1);    //flip(-1)=180
				}
			}
		}
	}

	split(S.rotatedShare,tempChannels);
	S.rotChannels	= tempChannels;
}

void Core::resetOptEstimation(){
	optBlock	= 1;
	optMinWidth = 1;
	optMinHeight= 1;
}

void Core::setOptEstimation(int bOptBlock, int bOptMinWidth, int bOptMinHeight){
	optBlock	= bOptBlock;
	optMinWidth = bOptMinWidth;
	optMinHeight= bOptMinHeight;
}

void Core::getIndexEstimation(Share& S1, Share& S2){
	int widthArea, heightArea, avWidthArea, avHeightArea, x1, y1, x2, y2;

	widthArea	= (S2.rotWidth/optBlock) + (S1.width/optBlock) - optMinWidth;
	heightArea	= (S2.rotHeight/optBlock) + (S1.height/optBlock) - optMinHeight;

	x1			= (S2.rotWidth/optBlock) - optMinWidth;
	y1			= (S2.rotHeight/optBlock) - optMinHeight;

	int alpha, beta;
	alpha		= x1 - (S1.pointStart.x/optBlock);
	beta		= y1 - (S1.pointStart.y/optBlock);

	x2			= (S2.pointStart.x/optBlock) + alpha;
	y2			= (S2.pointStart.y/optBlock) + beta;
	
	int index, position;
	index		= ((y2 - 1) * widthArea) + x2;
	position	= index + ((S2.angle/90) * (widthArea * heightArea));
	
	mpz_class poss(position);
	mpz_class res;
	cout << endl;
	mpz_mul_ui(res.get_mpz_t(),poss.get_mpz_t(),(duration1run*10)+0.5);
	mpz_div_ui(res.get_mpz_t(),res.get_mpz_t(),10);
	cout << "\Estimated time run (in second) \t\t: " << res.get_ui() << " seconds" << endl;
	mpz_div_ui(res.get_mpz_t(),res.get_mpz_t(),60);
	cout << "\nEstimated time run (in minutes) \t: " << res.get_ui() << " minutes" << endl;
	mpz_div_ui(res.get_mpz_t(),res.get_mpz_t(),60);
	cout << "\nEstimated time run (in hours) \t\t: " << res.get_ui() << " hours" << endl;
	mpz_div_ui(res.get_mpz_t(),res.get_mpz_t(),24);
	cout << "\nEstimated time run (in days) \t\t: " << res.get_ui() << " days" << endl;
	mpz_div_ui(res.get_mpz_t(),res.get_mpz_t(),365);
	cout << "\nEstimated time run (in years) \t\t: " << res.get_ui() << " years" << endl;
	cout << endl;
}