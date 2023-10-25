#include <opencv2/opencv.hpp>  
#include <iostream>  

using namespace cv;
using namespace std;

int main2() {
    Mat img1 = Mat::zeros(cv::Size(123, 123), CV_8SC1);
    Mat img2 = Mat::zeros(cv::Size(123, 123), CV_8SC1);
    
    for (int i = 0; i < 123; i++) {
        for (int j = 0; j < 123; j++) {
            if (i == j) {
                img1.at<uchar>(i,j) = i;
                img2.at<uchar>(i, j) = i;
            }
            if (i == 123 - j) {
                img2.at<uchar>(i, j) = i;
            }
        }
    }

    Mat dst, dst2;
    bitwise_or(img1, img2, dst);
    threshold(dst, dst2, 1, 255, cv::THRESH_BINARY);

    imshow("Source 1", img1);
    imshow("Source 2", img2);
    imshow("Destination", dst);
    imshow("Destination 2", dst2);
    waitKey(0);

    return 0;
}