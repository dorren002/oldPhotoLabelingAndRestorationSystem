#include "maskUpdater.h"
#include "util.h"

#include "qdebug.h"

maskUpdater::maskUpdater(int usrSetTH, int usrSetMaskFlag, int usrSetHistoryStep) {
    th = usrSetTH;
    maskFlag = usrSetMaskFlag;
    historyStep = usrSetHistoryStep;
}

maskUpdater::maskUpdater(string fname, int usrSetTH, int usrSetMaskFlag, int usrSetHistoryStep) {
    th = usrSetTH;
    updateSrcImg(fname);
    maskFlag = usrSetMaskFlag;
    historyStep = usrSetHistoryStep;
}

bool maskUpdater::updateSrcImg(string fname) {
    Mat newImg = imread(fname);
    if (newImg.empty())
    {
        return VOS_FAIL;
    }
    imgHeight = newImg.rows;
    imgWidth = newImg.cols;

    cvtColor(newImg, srcImg, COLOR_BGR2RGB);
    resetMask();
    
    return VOS_OK;
}

void maskUpdater::updateMask(int x, int y, bool isAdd) {
    if (isAdd) {
        add_mask(x, y);
    }
    else {
        del_mask(x, y);
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

    cv::Vec3b& pixel = srcImg.at<cv::Vec3b>(cv::Point(x, y));
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

        pixel = srcImg.at<cv::Vec3b>(cv::Point(cur_i, cur_j));
        RGB_POS cur = { pixel[0], pixel[1], pixel[2], cur_i, cur_j };

        if (!visited[cur_i][cur_j] && dist(pivot, cur) < th) {
            mask.at<uchar>(cv::Point(cur_i, cur_j)) = maskFlag;
            visited[cur_i][cur_j] = true;
            
            for (int index = 0; index != 4; ++index) {
                int next_i = cur_i + di[index], next_j = cur_j + dj[index];
                if (visited[next_i][next_j] || next_i < 0 || next_j < 0 || next_i == imgHeight || next_j == imgWidth) {
                    continue;
                }
                queuei.push(next_i);
                queuej.push(next_j);
            }
        }
    }
}

void maskUpdater::del_mask(int x, int y) {

}

bool maskUpdater::resetMask() {
    if (srcImg.empty() || imgHeight == -1 || imgWidth == -1) {
        return VOS_FAIL;
    }
    if (maskFlag == 1) {
        mask = Mat::ones(cv::Size(imgHeight, imgWidth), CV_8UC1);
    }
    else {
        mask = Mat::zeros(cv::Size(imgHeight, imgWidth), CV_8UC1);
    }
    return VOS_OK;
}
