#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class Share
{
public:
	int			index, width, height, rotWidth, rotHeight, widthSS, heightSS, rotWidthSS, rotHeightSS, angle, revAngle;
	vector<int> availableOrientation;
	vector<Point2i> intersection, position;
	Mat			share, rotatedShare;
	Point2i		pointStart, p;
	vector<Mat> channels, rotChannels;

	Share(int i);
	~Share();
	void 	setShare();
	void	setSSSize(int widthSI, int heightSI);
	void	setAvailableOrientation(int widthSI, int heightSI);
	void	getRotationSize();
	void	genRandomOrientation();
	void	setIntersection(int startX, int endX, int startY, int endY);
	void	setAngleRotation();
	void	setStartPosition();
};

