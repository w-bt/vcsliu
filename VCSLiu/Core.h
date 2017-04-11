#pragma once
#include "Share.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class Core
{
public:
	int 	widthSI, heightSI, widthCanvas, heightCanvas, optBlock, optMinWidth, optMinHeight;
	static	int optPause, optPreview, optOthers, optScheme, optShift, block;
	Mat 	SI, filteredSI, canvas;
	double	duration1run;
	string	ext;

	Core();
	void	reset();
	int 	viewMainMenu();
	void 	viewEncoding();
	void 	viewDecoding();
	void 	viewOption();
	void	setOptionPause();
	void	setOptionPreview();
	void	setOptionOthers();
	void 	centerString(char* s);
	void 	setSecretImage();
	void	chooseScheme();
	void	setTolerance();
	int		optAutostackIMSVCS();
	void	setCanvas(Share& S1, Share& S2);
	void	viewEncodingIMSVCS(Share& S1, Share& S2);
	void	viewEncodingNIMSVCS(Share& S1, Share& S2);
	void 	filter();
	void	generateShareIMSVCS(Share& S1, Share& S2);
	void	generateShareNIMSVCS(Share& S1, Share& S2);
	Mat		generateNewImg(int h, int w, int channel);
	void	viewDecodingIMSVCS(Share& S1, Share& S2);
	void	viewDecodingNIMSVCS(Share& S1, Share& S2);
	void	stackingIMSVCS(Share& S1, Share& S2);
	static int getRandomNumber();
	void	genIntersection(Share& S1, Share& S2);
	void	randomSIPosition(Share& S1, Share& S2);
	void	previewIA(Share S1, Share S2);
	void	getNormalRotationImage(Share& S);
	long long int getPossibilities(Share S1, Share S2);
	void	estimate1TimeRun(Share S1, Share S2);
	void	estimateCanvas(Share& S1, Share& S2, int r);
	void	estimateStack(Share& S1, Share& S2, int r);
	int		checkPattern();
	vector<int> minMaxX(Point2f corners[4]);
	void	viewOptionDecodingNIMSVCS();
	void	setOptionBlock();
	void	setOptionMinWidth();
	void	setOptionMinHeight();
	void	manualStacking(Share& S1, Share& S2);
	void	getRotationImage(Share& S);
	void	resetOptEstimation();
	void	setOptEstimation(int bOptBlock, int bOptMinWidth, int bOptMinHeight);
	void	getIndexEstimation(Share& S1, Share& S2);
};