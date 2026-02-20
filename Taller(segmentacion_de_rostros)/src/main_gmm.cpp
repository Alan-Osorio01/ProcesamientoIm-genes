#include <opencv2/opencv.hpp>
#include <iostream>
#include "gmm_segmenter.h"

int main(int argc, char** argv) {
    cv::VideoCapture cap;
    if (argc >= 2) cap.open(argv[1]);
    else cap.open(0);

    if (!cap.isOpened()) {
        std::cerr << "ERROR: No se pudo abrir video/camara.\n";
        return -1;
    }

    GMMSegmenter detector; // ahora detecta rostros

    cv::Mat frame;
    while (true) {
        if (!cap.read(frame) || frame.empty()) break;

        cv::Mat mask = detector.apply(frame);
        auto faces = detector.getRegions(mask);

        for (const auto& r : faces) {
            cv::rectangle(frame, r, cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow("Parte B - Frame (Rostros)", frame);
        if (!mask.empty()) cv::imshow("Parte B - Mask (Rostros)", mask);

        if (cv::waitKey(30) == 27) break; // ESC
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
