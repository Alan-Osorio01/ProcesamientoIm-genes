#pragma once
#include <opencv2/opencv.hpp>

class FrameDifferencing {
public:
    // Convierte BGR (CV_8UC3) a gris (CV_8UC1) a mano
    static void bgrToGrayManual(const cv::Mat& bgr, cv::Mat& gray);

    // |a - b| a mano (CV_8UC1)
    static void absDiffManual(const cv::Mat& a, const cv::Mat& b, cv::Mat& diff);

    // threshold a mano: si diff > thresh => 255, si no => 0
    static void thresholdManual(const cv::Mat& src, cv::Mat& dst, unsigned char thresh);
};
