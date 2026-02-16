#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class camara {
public:
    explicit camara(int deviceIndex = 0);
    bool open();
    void runPreview(const std::string& windowName = "camara");

private:
    int deviceIndex_;
    cv::VideoCapture cap_;
};
