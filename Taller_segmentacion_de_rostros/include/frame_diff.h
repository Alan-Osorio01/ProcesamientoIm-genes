#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class FrameDifferencer {
public:
    FrameDifferencer(int threshold_val, int min_contour_area, double learning_rate = 0.02);

    void setBackground(const cv::Mat& frame);
    cv::Mat process(const cv::Mat& frame);
    std::vector<cv::Rect> getRegions(const cv::Mat& mask);

    // Actualiza el fondo SOLO en zonas que no son foreground
    void updateBackgroundSelective(const cv::Mat& frame, const cv::Mat& mask);

private:
    cv::Mat background_;      // gray CV_8UC1
    int     threshold_val_;
    int     min_area_;
    double  learning_rate_;
    cv::Mat kernel_;
};
