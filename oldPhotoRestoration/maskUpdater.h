#pragma once
#include <string>
#include <deque>
#include <io.h>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class maskUpdater
{
public:
	bool detectionFlag;
	bool restoreFlag;

	maskUpdater();
	maskUpdater(string fname);

	int rows();
	int cols();

	Mat& getImage();
	Mat& getMask();
	Mat& getRestoredImage();
	void getMaskedImage(Mat* dst);

	bool resetMask();
	bool saveMask(string root, string fname, string imgFormat);
	bool saveRestoredImage(string root, string fname, string imgFormat);
	void updateMask(int x, int y, bool isAdd); // 0 - cancel / 1 - add
	
	bool updateSrcImg(string fname);
	
	void updateDetectedMask(Mat* mask);
	void updateRestoredImg(string fname);

	void updateRGBth(double value);
	void updateHSVth(double value);
	void switchRGBMode(bool mode); // 0-hsv, 1-rgb
	void setMaxCacheStep(int num);

	bool undo();

	void maskDilate();
	void maskErode();

	double getRGBth();
	double getHSVth();
	double getRatio(int viewSize);

	bool empty();
private:
	double th_rgb;
	double th_hsv;

	int maskFlag; //    0/1代表被掩模部分,仅在显示时参考该值
	bool rgbMode; // 0-hsv  1-rgb

	int cachedStep;
	int maxCacheStep;
	deque<Mat>* historyList;

	Mat rgbImg;
	Mat hsvImg;
	Mat resImg;

	Mat rgbMask;
	Mat hsvMask;
	Mat curMask;

	Mat detectedMask;

	int imgWidth = -1;
	int imgHeight = -1;

	void add_mask(int x, int y);
	void del_mask(int x, int y);
	void cacheHistory();

	void hsvThresholdOp(Mat& vChannel, bool isAdd);
	
	void defaultInitalization();
};

