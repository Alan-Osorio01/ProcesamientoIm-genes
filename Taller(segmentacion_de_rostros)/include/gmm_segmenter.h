#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class GMMSegmenter {
public:
    struct Config {
        std::string cascade_path;   // si está vacío, intentamos rutas comunes
        double      scale_factor = 1.1;
        int         min_neighbors = 5;
        int         min_size      = 60;   // tamaño mínimo de rostro (px)
    };

    GMMSegmenter();
    explicit GMMSegmenter(const Config& cfg);

    // Devuelve una máscara binaria (255 donde hay rostros)
    cv::Mat apply(const cv::Mat& frame);

    // Devuelve cajas de rostros detectados
    std::vector<cv::Rect> getRegions(const cv::Mat& mask) const;

    // No aplica para rostros (dejamos vacío)
    cv::Mat getBackground() const;

private:
    bool loadCascade(const std::string& path);
    bool loadCascadeAuto();

private:
    Config cfg_;
    cv::CascadeClassifier face_;
    std::vector<cv::Rect> last_faces_;
};
