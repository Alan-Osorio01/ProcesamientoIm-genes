#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class Camera {
public:
    explicit Camera(int deviceIndex = 0);
    bool open();
    bool read(cv::Mat& frame);
    void release();

private:
    int deviceIndex_;
    cv::VideoCapture cap_;
};
