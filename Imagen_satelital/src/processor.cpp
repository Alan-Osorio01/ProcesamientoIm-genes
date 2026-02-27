#include "processor.h"

#include <cmath>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

void imprimirInfoImagen(const cv::Mat& img) {
    std::cout << "=== INFO IMAGEN ===" << std::endl;
    std::cout << "  Filas      : " << img.rows << std::endl;
    std::cout << "  Columnas   : " << img.cols << std::endl;
    std::cout << "  Canales    : " << img.channels() << std::endl;
    std::cout << "  Profundidad: " << img.depth() << std::endl;
}

void calcularEstadisticas(const cv::Mat& img, cv::Scalar& media, cv::Scalar& desviacion) {
    cv::meanStdDev(img, media, desviacion);
    std::cout << "\n=== MODELADO GAUSSIANO ===" << std::endl;
    std::cout << "  Media     : " << media[0] << std::endl;
    std::cout << "  Desviacion: " << desviacion[0] << std::endl;
}

cv::Mat segmentacionGaussiana(
    const cv::Mat& img,
    const cv::Scalar& media,
    const cv::Scalar& desviacion,
    double factor
) {
    cv::Mat mascara = cv::Mat::zeros(img.size(), CV_8U);
    double limInf = media[0] - factor * desviacion[0];
    double limSup = media[0] + factor * desviacion[0];

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            unsigned char pixel = img.at<unsigned char>(i, j);
            if (pixel > limInf && pixel < limSup) {
                mascara.at<unsigned char>(i, j) = 255;
            }
        }
    }

    std::cout << "  Segmentacion con factor=" << factor
              << " | rango [" << limInf << ", " << limSup << "]" << std::endl;
    return mascara;
}

cv::Mat filtroGaussiano(const cv::Mat& img, int ksize, double sigma) {
    cv::Mat suavizada;
    cv::GaussianBlur(img, suavizada, cv::Size(ksize, ksize), sigma);
    return suavizada;
}

void compararDesviaciones(const cv::Mat& img, const cv::Mat& suavizada) {
    cv::Scalar media1;
    cv::Scalar desv1;
    cv::Scalar media2;
    cv::Scalar desv2;

    cv::meanStdDev(img, media1, desv1);
    cv::meanStdDev(suavizada, media2, desv2);

    std::cout << "\n=== COMPARACION DE VARIANZAS ===" << std::endl;
    std::cout << "  Desviacion original : " << desv1[0] << std::endl;
    std::cout << "  Desviacion suavizada: " << desv2[0] << std::endl;
    std::cout << "  Reduccion           : "
              << (desv1[0] - desv2[0]) / desv1[0] * 100.0 << " %" << std::endl;
}

cv::Mat filtroLaplaciano(const cv::Mat& img) {
    cv::Mat laplaciano;
    cv::Laplacian(img, laplaciano, CV_16S, 3);
    cv::convertScaleAbs(laplaciano, laplaciano);
    return laplaciano;
}

cv::Mat imagenRealzada(const cv::Mat& img, const cv::Mat& laplaciano) {
    cv::Mat sharpened;
    cv::add(img, laplaciano, sharpened);
    return sharpened;
}

cv::Mat pipelineCompleto(const cv::Mat& img) {
    cv::Mat gauss;
    cv::GaussianBlur(img, gauss, cv::Size(7, 7), 1.5);

    cv::Mat lap;
    cv::Laplacian(gauss, lap, CV_16S, 3);
    cv::convertScaleAbs(lap, lap);

    cv::Mat final_img;
    cv::add(gauss, lap, final_img);
    return final_img;
}

cv::Mat crearKernelGaussiano(int tamano, double sigma) {
    cv::Mat kernel(tamano, tamano, CV_64F);
    int centro = tamano / 2;
    double suma = 0.0;

    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            int x = i - centro;
            int y = j - centro;
            double valor = (1.0 / (2 * CV_PI * sigma * sigma)) *
                           std::exp(-(x * x + y * y) / (2 * sigma * sigma));
            kernel.at<double>(i, j) = valor;
            suma += valor;
        }
    }

    kernel = kernel / suma;
    return kernel;
}

void analizarKernel(double sigma) {
    cv::Mat k = crearKernelGaussiano(5, sigma);
    double sumaTotal = cv::sum(k)[0];
    double centro = k.at<double>(2, 2);

    std::cout << "\n=== KERNEL GAUSSIANO 5x5 (sigma=" << sigma << ") ===" << std::endl;
    std::cout << k << std::endl;
    std::cout << "  Suma total   : " << sumaTotal << " (debe ser ~1)" << std::endl;
    std::cout << "  Valor centro : " << centro << " (debe ser el mayor)" << std::endl;
}
