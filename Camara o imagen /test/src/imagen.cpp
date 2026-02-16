#include "imagen.h"
#include <iostream>

cv::Mat imagen::load(const std::string& path) {
    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cerr << "ERROR: No se pudo cargar la imagen: " << path << "\n";
    }
    return img;
}

void imagen::show(const cv::Mat& img, const std::string& windowName) {
    if (img.empty()) return;

    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    cv::imshow(windowName, img);
    cv::waitKey(0);
    cv::destroyWindow(windowName);
}
