#pragma once
#include <string>
#include <unordered_map>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class maskUpdater
{
public:
	maskUpdater();
	maskUpdater(string fname);

	void getMask(Mat* mask);

	void updateMask(int x, int y, bool isAdd); // 0 - cancel / 1 - add

	bool updateSrcImg(string fname);
	bool resetMask();

	void updateRGBth(double value);
	void updateHSVth(double value);
	void switchRGBMode(bool mode); // 0-hsv, 1-rgb
	void setHistoryStep(int num);

	double getRGBth();
	double getHSVth();
	
private:
	double th_rgb;
	double th_hsv;

	int maskFlag; //    0/1代表被掩模部分,仅在显示时参考该值
	int historyStep;
	
	bool rgbMode; // 0-hsv  1-rgb

	int maxStep;
	int maxBackStep;
	unordered_map<int, pair<int,int>> maxStep2xy;

	Mat rgbImg;
	Mat hsvImg;

	Mat rgbMask;
	Mat hsvMask;

	int imgWidth = -1;
	int imgHeight = -1;

	void add_mask(int x, int y);
	void del_mask(int x, int y);
	void hsvThresholdOp(Mat& vChannel, bool isAdd);
	
	void defaultInitalization();
};

