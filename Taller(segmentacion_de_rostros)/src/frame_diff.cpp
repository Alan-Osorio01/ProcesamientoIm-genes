#include "frame_diff.h"

FrameDifferencer::FrameDifferencer(int threshold_val, int min_contour_area, double learning_rate)
    : threshold_val_(threshold_val),
      min_area_(min_contour_area),
      learning_rate_(learning_rate) {
    kernel_ = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
}

void FrameDifferencer::setBackground(const cv::Mat& frame) {
    cv::cvtColor(frame, background_, cv::COLOR_BGR2GRAY);
}

cv::Mat FrameDifferencer::process(const cv::Mat& frame) {
    cv::Mat gray, diff, mask;

    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    cv::absdiff(background_, gray, diff);
    cv::threshold(diff, mask, threshold_val_, 255, cv::THRESH_BINARY);

    // Limpieza (mínima) para quitar ruido y rellenar huecos
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel_);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel_);

    return mask;
}

std::vector<cv::Rect> FrameDifferencer::getRegions(const cv::Mat& mask) {
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::Mat tmp = mask.clone();
    cv::findContours(tmp, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Rect> boxes;
    for (const auto& c : contours) {
        double area = cv::contourArea(c);
        if (area >= min_area_) {
            boxes.push_back(cv::boundingRect(c));
        }
    }
    return boxes;
}

void FrameDifferencer::updateBackgroundSelective(const cv::Mat& frame, const cv::Mat& mask) {
    if (learning_rate_ <= 0.0 || background_.empty()) return;

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // Invertir máscara: 255 donde NO hay movimiento
    cv::Mat inv;
    cv::bitwise_not(mask, inv);

    // Mezcla en float para hacer: bg = (1-a)*bg + a*gray SOLO donde inv==255
    cv::Mat bg32f, gray32f;
    background_.convertTo(bg32f, CV_32FC1);
    gray.convertTo(gray32f, CV_32FC1);

    const float a  = static_cast<float>(learning_rate_);
    const float ia = 1.0f - a;

    for (int y = 0; y < bg32f.rows; ++y) {
        float* pbg = bg32f.ptr<float>(y);
        const float* pg = gray32f.ptr<float>(y);
        const unsigned char* pim = inv.ptr<unsigned char>(y);

        for (int x = 0; x < bg32f.cols; ++x) {
            if (pim[x] == 255) { // solo fondo
                pbg[x] = ia * pbg[x] + a * pg[x];
            }
        }
    }

    bg32f.convertTo(background_, CV_8UC1);
}
