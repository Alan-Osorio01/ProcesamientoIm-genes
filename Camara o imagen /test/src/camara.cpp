#include "camara.h"
#include <iostream>

camara::camara(int deviceIndex) : deviceIndex_(deviceIndex) {}

bool camara::open() {
    cap_.open(deviceIndex_);
    if (!cap_.isOpened()) {
        std::cerr << "ERROR: No se pudo abrir la camara (deviceIndex=" << deviceIndex_ << ").\n";
        return false;
    }
    return true;
}

void camara::runPreview(const std::string& windowName) {
    if (!cap_.isOpened() && !open()) {
        return;
    }

    cv::Mat frame;
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

    while (true) {
        cap_ >> frame;
        if (frame.empty()) {
            std::cerr << "ERROR: Frame vacio desde la camara.\n";
            break;
        }

        cv::imshow(windowName, frame);

        // Teclas:
        //  q o ESC para salir
        const int key = cv::waitKey(1);
        if (key == 27 || key == 'q' || key == 'Q') {
            break;
        }
    }

    cv::destroyWindow(windowName);
}
