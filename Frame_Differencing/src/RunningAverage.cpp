#include "RunningAverage.h"
#include <iostream>
#include <algorithm>

RunningAverage::RunningAverage(float alpha)
    : alpha_(alpha), initialized_(false) {}

bool RunningAverage::isInitialized() const {
    return initialized_;
}

void RunningAverage::init(const cv::Mat& gray) {
    if (gray.empty() || gray.type() != CV_8UC1) {
        initialized_ = false;
        bg32f_.release();
        return;
    }

    bg32f_.create(gray.rows, gray.cols, CV_32FC1);

    for (int y = 0; y < gray.rows; ++y) {
        const unsigned char* pg = gray.ptr<unsigned char>(y);
        float* pb = bg32f_.ptr<float>(y);
        for (int x = 0; x < gray.cols; ++x) {
            pb[x] = static_cast<float>(pg[x]);
        }
    }

    initialized_ = true;
}

void RunningAverage::update(const cv::Mat& gray) {
    if (!initialized_) {
        init(gray);
        return;
    }

    if (gray.empty() || gray.type() != CV_8UC1 || gray.size() != bg32f_.size()) {
        return;
    }

    const float a = alpha_;
    const float ia = 1.0f - a;

    for (int y = 0; y < gray.rows; ++y) {
        const unsigned char* pg = gray.ptr<unsigned char>(y);
        float* pb = bg32f_.ptr<float>(y);
        for (int x = 0; x < gray.cols; ++x) {
            // bg = (1-a)*bg + a*gray
            pb[x] = ia * pb[x] + a * static_cast<float>(pg[x]);
        }
    }
}

void RunningAverage::backgroundU8(cv::Mat& outBgU8) const {
    if (!initialized_ || bg32f_.empty()) {
        outBgU8.release();
        return;
    }

    outBgU8.create(bg32f_.rows, bg32f_.cols, CV_8UC1);

    for (int y = 0; y < bg32f_.rows; ++y) {
        const float* pb = bg32f_.ptr<float>(y);
        unsigned char* po = outBgU8.ptr<unsigned char>(y);
        for (int x = 0; x < bg32f_.cols; ++x) {
            float v = pb[x];
            if (v < 0.0f) v = 0.0f;
            if (v > 255.0f) v = 255.0f;
            po[x] = static_cast<unsigned char>(v + 0.5f); // redondeo simple
        }
    }
}
