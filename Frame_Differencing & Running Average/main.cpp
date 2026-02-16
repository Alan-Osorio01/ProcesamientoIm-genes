#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

#include "Camera.h"
#include "FrameDifferencing.h"
#include "RunningAverage.h"

static std::string modeFromArgs(int argc, char** argv) {
    if (argc >= 2) return std::string(argv[1]);
    return "fd";
}

static float alphaFromArgs(int argc, char** argv, float def = 0.05f) {
    if (argc >= 3) {
        try { return std::stof(argv[2]); }
        catch (...) { return def; }
    }
    return def;
}

int main(int argc, char** argv) {
    const std::string mode = modeFromArgs(argc, argv);
    const unsigned char THRESH = 25;

    Camera cam(0);
    if (!cam.open()) return 1;

    cv::Mat frame, prevFrame;
    cv::Mat gray, prevGray, diff, mask;

    // Leer primer frame
    if (!cam.read(prevFrame) || prevFrame.empty()) {
        std::cerr << "ERROR: No se pudo leer el primer frame.\n";
        return 1;
    }
    FrameDifferencing::bgrToGrayManual(prevFrame, prevGray);
    if (prevGray.empty()) {
        std::cerr << "ERROR: No se pudo convertir a gris el primer frame.\n";
        return 1;
    }

    RunningAverage ra(alphaFromArgs(argc, argv, 0.05f));
    if (mode == "ra") {
        ra.init(prevGray); // inicializa el fondo con el primer gray
    }

    while (true) {
        if (!cam.read(frame) || frame.empty()) break;

        FrameDifferencing::bgrToGrayManual(frame, gray);
        if (gray.empty()) break;

        if (mode == "fd") {
            // Frame Differencing: |gray - prevGray|
            FrameDifferencing::absDiffManual(gray, prevGray, diff);
            FrameDifferencing::thresholdManual(diff, mask, THRESH);
            prevGray = gray.clone(); // si te piden esto "a mano" también, lo cambiamos

            cv::imshow("Frame", frame);
            cv::imshow("Movimiento - Frame Differencing", mask);
        }
        else if (mode == "ra") {
            // Running Average:
            // 1) actualizar fondo
            ra.update(gray);

            // 2) obtener fondo en 8u
            cv::Mat bgU8;
            ra.backgroundU8(bgU8);

            // 3) diff = |gray - bg|
            FrameDifferencing::absDiffManual(gray, bgU8, diff);

            // 4) threshold
            FrameDifferencing::thresholdManual(diff, mask, THRESH);

            cv::imshow("Frame", frame);
            cv::imshow("Background (Running Average)", bgU8);
            cv::imshow("Movimiento - Running Average", mask);
        }
        else {
            std::cerr << "Modo invalido. Usa: fd | ra\n";
            break;
        }

        const in
