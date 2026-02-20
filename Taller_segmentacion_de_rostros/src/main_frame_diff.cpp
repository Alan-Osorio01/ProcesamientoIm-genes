#include <opencv2/opencv.hpp>
#include <iostream>
#include "frame_diff.h"

int main(int argc, char** argv) {
    cv::VideoCapture cap;
    if (argc >= 2) cap.open(argv[1]);
    else cap.open(0);

    if (!cap.isOpened()) {
        std::cerr << "ERROR: No se pudo abrir video/camara.\n";
        return -1;
    }

    // Ajustes típicos:
    // threshold 25–35, min_area sube si hay ruido.
    FrameDifferencer fd(/*threshold*/30, /*min_area*/2000, /*learning_rate*/0.03);

    cv::Mat frame;
    if (!cap.read(frame) || frame.empty()) {
        std::cerr << "ERROR: No se pudo leer el primer frame.\n";
        return -1;
    }

    fd.setBackground(frame);

    while (true) {
        if (!cap.read(frame) || frame.empty()) break;

        cv::Mat mask = fd.process(frame);

        // Fondo adaptativo SOLO donde no hay movimiento
        fd.updateBackgroundSelective(frame, mask);

        auto boxes = fd.getRegions(mask);
        for (const auto& r : boxes) {
            cv::rectangle(frame, r, cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow("Parte A - Frame", frame);
        cv::imshow("Parte A - Mask", mask);

        if (cv::waitKey(30) == 27) break; // ESC
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
