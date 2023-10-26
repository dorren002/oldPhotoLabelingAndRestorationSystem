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
    historyStep = 128;
    rgbMode = true;
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

    resetMask();
    
    return VOS_OK;
}

void maskUpdater::updateMask(int x, int y, bool isAdd) {
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
    
}

inline int dist(RGB_POS a, RGB_POS b) {
    float color_diff = THREE_SUM(pow(a.R - b.R, 2), pow(a.G - b.G, 2), pow(a.B - a.B, 2));
    float pos_diff = TWO_SUM(pow(a.X - b.X, 2), pow(a.Y - b.Y, 2));
    return color_diff * 20 + pos_diff;
}

void maskUpdater::add_mask(int x, int y) {
    int di[4] = { 0, 0, 1, -1 };
    int dj[4] = { 1, -1, 0, 0 };

    cv::Vec3b& pixel = rgbImg.at<cv::Vec3b>(cv::Point(x, y));
    RGB_POS pivot = { pixel[0], pixel[1], pixel[2], x, y };

    queue<int> queuei;
    queue<int> queuej;
    queuei.push(x);
    queuej.push(y);

    vector<vector<bool>> visited(imgHeight, vector<bool>(imgWidth, false));

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
            rgbMask.at<uchar>(cv::Point(cur_i, cur_j)) = maxStep;
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
    maxStep++;
}

void maskUpdater::del_mask(int x, int y) {
    int di[4] = { 0, 0, 1, -1 };
    int dj[4] = { 1, -1, 0, 0 };

    cv::Vec3b& pixel = rgbImg.at<cv::Vec3b>(cv::Point(x, y));
    RGB_POS pivot = { pixel[0], pixel[1], pixel[2], x, y };

    queue<int> queuei;
    queue<int> queuej;
    queuei.push(x);
    queuej.push(y);

    vector<vector<bool>> visited(imgHeight, vector<bool>(imgWidth, false));

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
            rgbMask.at<uchar>(cv::Point(cur_i, cur_j)) = maxBackStep;
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
    maxBackStep--;
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

    rgbMask = Mat::zeros(cv::Size(imgWidth, imgHeight), CV_8UC1);
    hsvMask = Mat::zeros(cv::Size(imgWidth, imgHeight), CV_8UC1);

    maxStep = 1;
    maxBackStep = -1;

    return VOS_OK;
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

void maskUpdater::setHistoryStep(int num) {
    historyStep = num;
}

void maskUpdater::getMask(Mat* mask) {
    Mat temp;
    cv::bitwise_or(rgbMask, hsvMask, temp);
    threshold(temp, *mask, 1, 255, cv::THRESH_BINARY);
}

double maskUpdater::getRGBth() {
    return th_rgb;
}

double maskUpdater::getHSVth() {
    return th_hsv;
}