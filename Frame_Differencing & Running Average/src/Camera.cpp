#include "Camera.h"
#include <iostream>

Camera::Camera(int deviceIndex) : deviceIndex_(deviceIndex) {}

bool Camera::open() {
    cap_.open(deviceIndex_);
    if (!cap_.isOpened()) {
        std::cerr << "ERROR: No se pudo abrir la camara (deviceIndex=" << deviceIndex_ << ").\n";
        return false;
    }
    return true;
}

bool Camera::read(cv::Mat& frame) {
    if (!cap_.isOpened()) return false;
    return cap_.read(frame);
}

void Camera::release() {
    if (cap_.isOpened()) cap_.release();
}
