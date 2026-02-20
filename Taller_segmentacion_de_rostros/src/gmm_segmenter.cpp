#include "gmm_segmenter.h"
#include <iostream>

static const char* kCommonCascadePaths[] = {
    "/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml",
    "/usr/share/opencv/haarcascades/haarcascade_frontalface_default.xml"
};

GMMSegmenter::GMMSegmenter() : GMMSegmenter(Config{}) {}

GMMSegmenter::GMMSegmenter(const Config& cfg) : cfg_(cfg) {
    bool ok = false;

    if (!cfg_.cascade_path.empty()) {
        ok = loadCascade(cfg_.cascade_path);
    }
    if (!ok) {
        ok = loadCascadeAuto();
    }

    if (!ok) {
        std::cerr << "ERROR: No se pudo cargar el Haar cascade para rostros.\n"
                  << "Busca el archivo haarcascade_frontalface_default.xml y pásalo en Config.\n";
    }
}

bool GMMSegmenter::loadCascade(const std::string& path) {
    if (face_.load(path)) {
        std::cout << "Cascade cargado: " << path << "\n";
        return true;
    }
    return false;
}

bool GMMSegmenter::loadCascadeAuto() {
    for (const auto* p : kCommonCascadePaths) {
        if (loadCascade(p)) return true;
    }
    return false;
}

cv::Mat GMMSegmenter::apply(const cv::Mat& frame) {
    last_faces_.clear();

    if (frame.empty() || face_.empty()) {
        return cv::Mat();
    }

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    face_.detectMultiScale(
        gray,
        last_faces_,
        cfg_.scale_factor,
        cfg_.min_neighbors,
        0,
        cv::Size(cfg_.min_size, cfg_.min_size)
    );

    // máscara binaria con rectángulos de rostros
    cv::Mat mask(frame.rows, frame.cols, CV_8UC1, cv::Scalar(0));
    for (const auto& r : last_faces_) {
        cv::rectangle(mask, r, cv::Scalar(255), cv::FILLED);
    }

    return mask;
}

std::vector<cv::Rect> GMMSegmenter::getRegions(const cv::Mat& /*mask*/) const {
    return last_faces_;
}

cv::Mat GMMSegmenter::getBackground() const {
    return cv::Mat(); // no aplica
}
