#include "stdafx.h"
#include "targetver.h"
#include "Share.h"
#include "Core.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <set>
#include <algorithm>    // std::shuffle, sort
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <iostream>     // std::cout
#include <vector>       // std::vector
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace cv;
using namespace std;

Share::Share(int i)
{
	index		= i;
	angle		= 0;
	revAngle	= 0;
	widthSS 	= 0;
	heightSS 	= 0;
	width		= 0;
	height		= 0;
	rotWidth	= 0;
	rotHeight	= 0;
}


Share::~Share(void)
{
}

void Share::setShare(){
	char pathS[100];
	vector<Mat> tempChannels(4);
	int flagS	= 0;
	while(flagS == 0){
		cout << "\n> Input S" << index << "'s path : ";
		cin >> pathS;
		share		= imread(pathS, IMREAD_UNCHANGED);
		if(share.data > 0){
			split(share,tempChannels);
			channels	= tempChannels;
			width 		= share.cols;
			height 		= share.rows;
			flagS		= 1;
		} else {
			cout << "\nNo share found" << endl;
		}
	}
}

void Share::setSSSize(int widthSI, int heightSI)
{
	int flag	= 0;
	int	flagW	= 0;
	int w;
	while(flagW == 0){
		cout << "\n> Input the width of SS" << index << ": ";
		cin >> w;
		if(index == 1){
			if(w >= widthSI){
				widthSS		= w;
				width		= w * 2;
				flagW		= 1;
			} else {
				cout << "\nThe width value should be equal or greater than " << widthSI << endl;
			}
		} else {
			if(w >= widthSI){
				widthSS		= w;
				width		= w * 2;
				flagW		= 1;
			} else {
				if(w >= heightSI){
					widthSS		= w;
					width		= w * 2;
					flagW		= 1;
					flag		= 1;
				} else {
					cout << "\nThe width value should be equal or greater than " << widthSI << " or " << heightSI << endl;
				}
			}
		}
	}

	int	flagH	= 0;
	int h;
	while(flagH == 0){
		cout << "\n> Input the height of SS" << index << ": ";
		cin >> h;
		if(index == 1){
			if(h >= heightSI){
				heightSS	= h;
				height		= h * 2;
				flagH		= 1;
			} else {
				cout << "\nThe height value should be equal or greater than " << heightSI << endl;
			}
		} else {
			if(flag){
				if(h >= widthSI){
					heightSS	= h;
					height		= h * 2;
					flagH		= 1;
				} else {
					cout << "\nThe height value should be equal or greater than " << widthSI << endl;
				}
			} else {
				if(h >= heightSI){
					heightSS	= h;
					height		= h * 2;
					flagH		= 1;
				} else {
					cout << "\nThe height value should be equal or greater than " << heightSI << endl;
				}
			}
		}
	}
}

void Share::setAvailableOrientation(int widthSI, int heightSI){
	if((widthSS >= widthSI)&&(widthSS >= heightSI)&&(heightSS >= widthSI)&&(heightSS >= heightSI)){
		availableOrientation.push_back(0);
		availableOrientation.push_back(1);
		availableOrientation.push_back(2);
		availableOrientation.push_back(3);
	} else {
		if((widthSS >= widthSI)&&(heightSS >= heightSI)){
			availableOrientation.push_back(0);
			availableOrientation.push_back(2);
		} else {
			availableOrientation.push_back(1);
			availableOrientation.push_back(3);
		}
	}
}

void Share::getRotationSize(){
	if((angle == 0)||(angle == 180)){
		if(share.data > 0){
			rotWidth	= width;
			rotHeight	= height;
		} else {
			rotWidthSS	= widthSS;
			rotHeightSS	= heightSS;
		}
	} else {
		if(share.data > 0){
			rotWidth	= height;
			rotHeight	= width;
		} else {
			rotWidthSS	= heightSS;
			rotHeightSS	= widthSS;
		}
	}
}

void Share::genRandomOrientation(){
	int size, r, c;

	size	= availableOrientation.size();
	r		= Core::getRandomNumber();
	c		= r % size;

	if(availableOrientation[c] == 0){
		angle		= 0;
	} else {
		if(availableOrientation[c] == 1){
			angle		= 90;
		} else {
			if(availableOrientation[c] == 2){
				angle		= 180;
			} else {
				if(availableOrientation[c] == 3){
					angle		= 270;
				}
			}
		}
	}

	revAngle	= (360 - angle) % 360;
}

void Share::setIntersection(int startX, int endX, int startY, int endY){
	intersection.push_back(Point(startX,startY));
	intersection.push_back(Point(endX,startY));
	intersection.push_back(Point(startX,endY));
	intersection.push_back(Point(endX,endY));
}

void Share::setAngleRotation(){
	int flag	= 0;
	int ans;
	while(flag == 0){
		cout << "\n> Input the angle rotation of share " << index << " (0/90/180/270) : ";
		cin >> ans;
		if((ans == 0)||(ans == 90)||(ans == 180)||(ans == 270)){
			angle		= ans;
			flag		= 1;
		} else {
			cout << "\nPlease input (0), (90), (180), or (270)";
		}
	}
}

void Share::setStartPosition(){
	int flag	= 0;
	int ans;
	int x;
	while(flag == 0){
		cout << "\n> Input x coordinate of share " << index << " : ";
		cin >> ans;
		if(index == 1){
			if((ans >= 0)&&(ans < width)){
				x			= ans;
				flag		= 1;
			} else {
				cout << "\nPlease input between 0 until " << width;
			}
		} else {
			if(index == 2){
				if(rotWidth == NULL){
					if((ans >= 0)&&(ans < width)){
						x			= ans;
						flag		= 1;
					} else {
						cout << "\nPlease input between 0 until " << width;
					}
				} else {
					if((ans >= 0)&&(ans < rotWidth)){
						x			= ans;
						flag		= 1;
					} else {
						cout << "\nPlease input between 0 until " << rotWidth;
					}
				}
			}
		}
	}

	flag	= 0;
	ans;
	int y;
	while(flag == 0){
		cout << "\n> Input y coordinate of share " << index << " : ";
		cin >> ans;
		if(index == 1){
			if((ans >= 0)&&(ans < height)){
				y			= ans;
				flag		= 1;
			} else {
				cout << "\nPlease input between 0 until " << height;
			}
		} else {
			if(index == 2){
				if(rotHeight == NULL){
					if((ans >= 0)&&(ans < height)){
						y			= ans;
						flag		= 1;
					} else {
						cout << "\nPlease input between 0 until " << height;
					}
				} else {
					if((ans >= 0)&&(ans < rotHeight)){
						y			= ans;
						flag		= 1;
					} else {
						cout << "\nPlease input between 0 until " << rotHeight;
					}
				}
			}
		}
	}

	pointStart	= Point(x,y);
}
