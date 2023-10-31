#include "maskUpdater.h"
#include "util.h"

#include "qdebug.h"

maskUpdater::maskUpdater() {
    defaultInitalization();
}

maskUpdater::maskUpdater(string fname) {
    defaultInitalization();    
    updateSrcImg(fname);
}

void maskUpdater::defaultInitalization() {
    th_rgb = 400.0;
    th_hsv = 0.8;
    maskFlag = 1;
    maxCacheStep = 10;
    rgbMode = true;

    historyList = new deque<Mat>();
}

bool maskUpdater::updateSrcImg(string fname) {
    Mat newImg = imread(fname);

    if (newImg.empty())
    {
        return VOS_FAIL;
    }

    imgHeight = newImg.rows;
    imgWidth = newImg.cols;

    cvtColor(newImg, rgbImg, COLOR_BGR2RGB);
    cvtColor(newImg, hsvImg, COLOR_BGR2HSV);
    
    return VOS_OK;
}

void maskUpdater::updateDetectedMask(Mat* mask) {
    if (mask) {
        mask->copyTo(detectedMask);
        resetMask();
    }
    else {
        detectedMask = NULL;
    }
    
}

void maskUpdater::updateMask(int x, int y, bool isAdd) {
    cacheHistory();

    if (rgbMode) {
        if (isAdd) {
            add_mask(x, y);
        }
        else {
            del_mask(x, y);
        }
    }
    else {
        vector<Mat> mv;
        split(hsvImg, mv);
        hsvThresholdOp(mv.at(2), isAdd);
    }

    cv::bitwise_or(rgbMask, hsvMask, curMask);
}

inline int dist(RGB_POS a, RGB_POS b) {
    float color_diff = THREE_SUM(pow(a.R - b.R, 2), pow(a.G - b.G, 2), pow(a.B - a.B, 2));
    float pos_diff = TWO_SUM(pow(a.X - b.X, 2), pow(a.Y - b.Y, 2));
    return color_diff * 20 + pos_diff;
}

void maskUpdater::add_mask(int x, int y) {
    if (curMask.at<uchar>(cv::Point(x, y)) == 1) {
        return;
    }

    queue<int> queuei;
    queue<int> queuej;
    queuei.push(x);
    queuej.push(y);

    vector<vector<bool>> visited(imgHeight, vector<bool>(imgWidth, false));

    int di[4] = { 0, 0, 1, -1 };
    int dj[4] = { 1, -1, 0, 0 };

    cv::Vec3b& pixel = rgbImg.at<cv::Vec3b>(cv::Point(x, y));
    RGB_POS pivot = { pixel[0], pixel[1], pixel[2], x, y };

    while (!queuei.empty()) {
        int cur_i = queuei.front(), cur_j = queuej.front();
        queuei.pop();
        queuej.pop();

        if (visited[cur_i][cur_j]) {
            continue;
        }

        pixel = rgbImg.at<cv::Vec3b>(cv::Point(cur_i, cur_j));
        RGB_POS cur = { pixel[0], pixel[1], pixel[2], cur_i, cur_j };

        if (!visited[cur_i][cur_j] && dist(pivot, cur) < th_rgb) {
            rgbMask.at<uchar>(cv::Point(cur_i, cur_j)) = 1;
            visited[cur_i][cur_j] = true;
            
            for (int index = 0; index != 4; ++index) {
                int next_i = cur_i + di[index], next_j = cur_j + dj[index];
                if (next_i < 0 || next_j < 0 || next_i == imgHeight || next_j == imgWidth || visited[next_i][next_j]) {
                    continue;
                }
                queuei.push(next_i);
                queuej.push(next_j);
            }
            qDebug() << rgbMask.at<uchar>(cv::Point(cur_i, cur_j));
        }
    }
}

void maskUpdater::del_mask(int x, int y) {
    if (curMask.at<uchar>(cv::Point(x, y)) == 0) {
        return;
    }

    queue<int> queuei;
    queue<int> queuej;
    queuei.push(x);
    queuej.push(y);

    vector<vector<bool>> visited(imgHeight, vector<bool>(imgWidth, false));

    int di[4] = { 0, 0, 1, -1 };
    int dj[4] = { 1, -1, 0, 0 };

    cv::Vec3b& pixel = rgbImg.at<cv::Vec3b>(cv::Point(x, y));
    RGB_POS pivot = { pixel[0], pixel[1], pixel[2], x, y };

    while (!queuei.empty()) {
        int cur_i = queuei.front(), cur_j = queuej.front();
        queuei.pop();
        queuej.pop();

        if (visited[cur_i][cur_j]) {
            continue;
        }

        pixel = rgbImg.at<cv::Vec3b>(cv::Point(cur_i, cur_j));
        RGB_POS cur = { pixel[0], pixel[1], pixel[2], cur_i, cur_j };

        if (!visited[cur_i][cur_j] && dist(pivot, cur) < th_rgb) {
            rgbMask.at<uchar>(cv::Point(cur_i, cur_j)) = 0;
            visited[cur_i][cur_j] = true;

            for (int index = 0; index != 4; ++index) {
                int next_i = cur_i + di[index], next_j = cur_j + dj[index];
                if (next_i < 0 || next_j < 0 || next_i == imgHeight || next_j == imgWidth || visited[next_i][next_j]) {
                    continue;
                }
                queuei.push(next_i);
                queuej.push(next_j);
            }
        }
    }
}

void maskUpdater::maskDilate() {
    cacheHistory();
    Mat cur = curMask;
    Mat element = Mat::ones(5, 5, CV_8UC1);

    dilate(cur, curMask, element);
}

void maskUpdater::maskErode() {
    cacheHistory();
    Mat cur = curMask;
    Mat element = Mat::ones(5, 5, CV_8UC1);

    erode(cur, curMask, element);
}

void maskUpdater::hsvThresholdOp(Mat& vChannel, bool isAdd) {
    if (isAdd) {
        threshold(vChannel, hsvMask, th_hsv, 1, cv::THRESH_BINARY);
    }
    else {
        threshold(vChannel, hsvMask, th_hsv, 1, cv::THRESH_BINARY_INV);
    }
}

bool maskUpdater::resetMask() {
    if (rgbImg.empty() || imgHeight == -1 || imgWidth == -1) {
        return VOS_FAIL;
    }

    if (detectedMask.empty()) {
        rgbMask = Mat::zeros(cv::Size(imgWidth, imgHeight), CV_8UC1);
        hsvMask = Mat::zeros(cv::Size(imgWidth, imgHeight), CV_8UC1);
        curMask = Mat::zeros(cv::Size(imgWidth, imgHeight), CV_8UC1);
    }
    else {
        detectedMask.copyTo(curMask);
        detectedMask.copyTo(rgbMask);
        detectedMask.copyTo(hsvMask);
    }
    

    cachedStep = 0;
    if (!historyList->empty()) {
        historyList->clear();
    }

    return VOS_OK;
}

bool maskUpdater::saveMask(string root, string imgFormat) {
    const char* p = root.c_str();
    if ((_access(p, 0)) != -1) {
        imwrite(root + "/output." + imgFormat, curMask);
        return VOS_OK;
    }
    else {
        return VOS_FAIL;
    }
    
}

void maskUpdater::updateRGBth(double value) {
    th_rgb = value;
}

void maskUpdater::updateHSVth(double value) {
    th_hsv = value;
}

void maskUpdater::switchRGBMode(bool mode) {
    rgbMode = mode;
}

void maskUpdater::setMaxCacheStep(int num) {
    maxCacheStep = num;
}

Mat& maskUpdater::getImage() {
    return rgbImg;
}

Mat& maskUpdater::getMask() {
    return curMask;
}

double maskUpdater::getRGBth() {
    return th_rgb;
}

double maskUpdater::getHSVth() {
    return th_hsv;
}

int maskUpdater::rows() {
    return imgHeight;
}

int maskUpdater::cols() {
    return imgWidth;
}

/*
    cache
*/
void maskUpdater::cacheHistory() {
    if (cachedStep == maxCacheStep) {
        historyList->pop_front();
        cachedStep--;
    }
    Mat temp;
    curMask.copyTo(temp);
    historyList->push_back(temp);
    cachedStep++;
}

bool maskUpdater::undo() {
    if (cachedStep <= 0) {
        return VOS_FAIL;
    }
    curMask = historyList->at(cachedStep-1);
    historyList->pop_back();
    cachedStep--;
    return VOS_OK;
}

bool maskUpdater::empty() {
    if (rgbImg.empty()) {
        return true;
    }
    else {
        return false;
    }
}