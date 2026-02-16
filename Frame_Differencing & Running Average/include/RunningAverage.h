#pragma once
#include <opencv2/opencv.hpp>

class RunningAverage {
public:
    explicit RunningAverage(float alpha = 0.05f);

    // Inicializa el fondo con una imagen gray (CV_8UC1)
    void init(const cv::Mat& gray);

    // Actualiza: bg = (1-alpha)*bg + alpha*gray   (a mano)
    void update(const cv::Mat& gray);

    // Devuelve una versión 8-bit del fondo (para visualizar o absdiff)
    void backgroundU8(cv::Mat& outBgU8) const;

    bool isInitialized() const;

private:
    float alpha_;
    bool initialized_;
    cv::Mat bg32f_; // CV_32FC1
};
