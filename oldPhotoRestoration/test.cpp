#include <iostream>  
#include <opencv2/opencv.hpp>  

using namespace std;
using namespace cv;

int main1() {
    // 读取图像  
    Mat image = imread("F:\mask.png", IMREAD_GRAYSCALE);

    // 定义结构元素  
    Mat element = Mat::ones(5, 5, CV_8UC1);

    // 膨胀操作  
    Mat dilatedImage;
    dilate(image, dilatedImage, element);

    // 腐蚀操作  
    Mat erodedImage;
    erode(image, erodedImage, element);

    // 显示结果  
    namedWindow("Original Image", WINDOW_NORMAL);
    imshow("Original Image", image);
    namedWindow("Dilated Image", WINDOW_NORMAL);
    imshow("Dilated Image", dilatedImage);
    namedWindow("Eroded Image", WINDOW_NORMAL);
    imshow("Eroded Image", erodedImage);
    waitKey(0);
    return 0;
}