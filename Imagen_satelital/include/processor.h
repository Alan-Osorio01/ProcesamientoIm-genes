#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <opencv2/core.hpp>

void imprimirInfoImagen(const cv::Mat& img);
void calcularEstadisticas(const cv::Mat& img, cv::Scalar& media, cv::Scalar& desviacion);

cv::Mat segmentacionGaussiana(
    const cv::Mat& img,
    const cv::Scalar& media,
    const cv::Scalar& desviacion,
    double factor
);

cv::Mat filtroGaussiano(const cv::Mat& img, int ksize, double sigma);
void compararDesviaciones(const cv::Mat& img, const cv::Mat& suavizada);
cv::Mat filtroLaplaciano(const cv::Mat& img);
cv::Mat imagenRealzada(const cv::Mat& img, const cv::Mat& laplaciano);
cv::Mat pipelineCompleto(const cv::Mat& img);
cv::Mat crearKernelGaussiano(int tamano, double sigma);
void analizarKernel(double sigma);

#endif
