#pragma once
#include <string>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class maskUpdater
{
public:
	Mat srcImg;
	Mat mask;

	maskUpdater(int th = 400, int usrSetMaskFlag = 0, int usrSetHistoryStep = 10);
	maskUpdater(string fname, int th = 0.45, int usrSetMaskFlag = 0, int usrSetHistoryStep = 10);
	bool updateSrcImg(string fname);
	void updateMask(int x, int y, bool isAdd); // 0 - cancel / 1 - add
	bool resetMask();


private:
	int th;
	int maskFlag; //    0/1代表被掩模部分
	int historyStep;
	int imgWidth = -1;
	int imgHeight = -1;

	void add_mask(int x, int y);
	void del_mask(int x, int y);
};

