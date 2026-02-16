#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
 
using namespace cv;
using namespace std;

static Mat ajustar_tamano(const Mat& img, const Size& target_size) {
    if (img.size() == target_size) return img;
    Mat salida;
    resize(img, salida, target_size);
    return salida;
}

static void esperar_q_o_esc() {
    while (true) {
        int key = waitKey(30);
        if (key == 27 || key == 'q' || key == 'Q') break;
    }
    destroyAllWindows();
}
 
void ejercicio2_modificar_saturacion() {
    Mat img_bgr = imread("ImagenPrueba.jpeg");
    if (img_bgr.empty()) img_bgr = imread("imagen.jpg");
    if (img_bgr.empty()) img_bgr = imread("../ImagenPrueba.jpeg");
    if (img_bgr.empty()) img_bgr = imread("../../ImagenPrueba.jpeg");
    if (img_bgr.empty()) {
        cout << "Error: No se pudo cargar la imagen. Rutas probadas: "
             << "ImagenPrueba.jpeg, imagen.jpg, ../ImagenPrueba.jpeg, ../../ImagenPrueba.jpeg" << endl;
        return;
    }
    int rows = img_bgr.rows;
    int cols = img_bgr.cols;
    Mat img_hsv(rows, cols, CV_8UC3);
    // ========== PASO 1: Conversión BGR → HSV ==========
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel_bgr = img_bgr.at<Vec3b>(i, j);
            // Obtener valores BGR y normalizar a [0,1]
            double b = pixel_bgr[0] / 255.0;
            double g = pixel_bgr[1] / 255.0;
            double r = pixel_bgr[2] / 255.0;
            // Calcular Cmax, Cmin, Delta
            double Cmax = max(max(r, g), b);
            double Cmin = min(min(r, g), b);
            double delta = Cmax - Cmin;
            // Calcular Hue (H)
            double H = 0;
            if (delta != 0) {
                if (Cmax == r) {
                    H = 60.0 * fmod(((g - b) / delta), 6.0);
                } else if (Cmax == g) {
                    H = 60.0 * (((b - r) / delta) + 2.0);
                } else if (Cmax == b) {
                    H = 60.0 * (((r - g) / delta) + 4.0);
                }
            }
            if (H < 0) H += 360.0;
            // OpenCV usa [0,180] para H
            double H_opencv = H / 2.0;
            // Calcular Saturation (S)
            double S = (Cmax == 0) ? 0 : (delta / Cmax);
            double S_opencv = S * 255.0;
            // Calcular Value (V)
            double V = Cmax;
            double V_opencv = V * 255.0;
            // Asignar valores HSV al píxel
            img_hsv.at<Vec3b>(i, j) = Vec3b(
                (uchar)H_opencv,
                (uchar)S_opencv,
                (uchar)V_opencv
            );
        }
    }
    // ========== PASO 2: Modificar Saturación ==========
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel_hsv = img_hsv.at<Vec3b>(i, j);
            // Obtener valores H, S, V
            double H = pixel_hsv[0];
            double S = pixel_hsv[1];
            double V = pixel_hsv[2];
            // Multiplicar S por 1.5 (sin exceder 255)
            S = min(S * 1.5, 255.0);
            // Asignar nuevos valores
            img_hsv.at<Vec3b>(i, j) = Vec3b(
                (uchar)H,
                (uchar)S,
                (uchar)V
            );
        }
    }
    // ========== PASO 3: Conversión HSV → BGR ==========
    Mat img_resultado(rows, cols, CV_8UC3);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel_hsv = img_hsv.at<Vec3b>(i, j);
            // Desnormalizar
            double H = pixel_hsv[0] * 2.0;  // [0,180] → [0,360]
            double S = pixel_hsv[1] / 255.0; // [0,255] → [0,1]
            double V = pixel_hsv[2] / 255.0; // [0,255] → [0,1]
            // Calcular C, X, m
            double C = V * S;
            double X = C * (1.0 - fabs(fmod(H / 60.0, 2.0) - 1.0));
            double m = V - C;
            double r_prime = 0, g_prime = 0, b_prime = 0;
            // Según el rango de H
            if (H >= 0 && H < 60) {
                r_prime = C; g_prime = X; b_prime = 0;
            } else if (H >= 60 && H < 120) {
                r_prime = X; g_prime = C; b_prime = 0;
            } else if (H >= 120 && H < 180) {
                r_prime = 0; g_prime = C; b_prime = X;
            } else if (H >= 180 && H < 240) {
                r_prime = 0; g_prime = X; b_prime = C;
            } else if (H >= 240 && H < 300) {
                r_prime = X; g_prime = 0; b_prime = C;
            } else if (H >= 300 && H < 360) {
                r_prime = C; g_prime = 0; b_prime = X;
            }
            // Convertir a [0,255]
            double R = (r_prime + m) * 255.0;
            double G = (g_prime + m) * 255.0;
            double B = (b_prime + m) * 255.0;
            // Asignar valores BGR al píxel resultado
            img_resultado.at<Vec3b>(i, j) = Vec3b(
                (uchar)min(max(B, 0.0), 255.0),
                (uchar)min(max(G, 0.0), 255.0),
                (uchar)min(max(R, 0.0), 255.0)
            );
        }
    }
    // Mostrar resultados al mismo tamano
    Mat img_resultado_mismo_tamano = ajustar_tamano(img_resultado, img_bgr.size());
    imshow("Original", img_bgr);
    imshow("Saturacion Aumentada", img_resultado_mismo_tamano);
    cout << "Presiona 'q' o ESC para cerrar." << endl;
    esperar_q_o_esc();
}
 
int main() {
    ejercicio2_modificar_saturacion();
    return 0;
}
