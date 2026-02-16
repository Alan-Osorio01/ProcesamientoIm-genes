#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class imagen {
public:
    static cv::Mat load(const std::string& path);
    static void show(const cv::Mat& img, const std::string& windowName = "Image");
};
