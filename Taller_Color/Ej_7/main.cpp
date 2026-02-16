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
 
void ejercicio7_vignette(double k = 0.4) {
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
    cout << "Aplicando correccion de viñeteo con k = " << k << endl;
    // ========== PASO 1: Calcular centro de la imagen ==========
    double cx = cols / 2.0;
    double cy = rows / 2.0;
    cout << "Centro de la imagen: (" << cx << ", " << cy << ")" << endl;
    // ========== PASO 2: Calcular distancia máxima (a la esquina) ==========
    double d_max = sqrt(cx * cx + cy * cy);
    cout << "Distancia maxima: " << d_max << endl;
    // ========== PASO 3: Aplicar corrección píxel por píxel ==========
    Mat img_resultado(rows, cols, CV_8UC3);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel = img_bgr.at<Vec3b>(i, j);
            // Calcular distancia al centro
            double dx = j - cx;
            double dy = i - cy;
            double d = sqrt(dx * dx + dy * dy);
            // Distancia normalizada [0, 1]
            double d_norm = d / d_max;
            // Factor de corrección
            double factor = 1.0 / (1.0 - k * d_norm * d_norm);
            // Aplicar corrección a cada canal y limitar a [0, 255]
            double B_nuevo = min(pixel[0] * factor, 255.0);
            double G_nuevo = min(pixel[1] * factor, 255.0);
            double R_nuevo = min(pixel[2] * factor, 255.0);
            img_resultado.at<Vec3b>(i, j) = Vec3b(
                (uchar)max(B_nuevo, 0.0),
                (uchar)max(G_nuevo, 0.0),
                (uchar)max(R_nuevo, 0.0)
            );
        }
    }
    cout << "Correccion de viñeteo completada!" << endl;
    // Mostrar resultados al mismo tamano
    Mat img_resultado_mismo_tamano = ajustar_tamano(img_resultado, img_bgr.size());
    imshow("Original", img_bgr);
    imshow("Vineteo Corregido k=" + to_string(k), img_resultado_mismo_tamano);
    cout << "Presiona 'q' o ESC para cerrar." << endl;
    esperar_q_o_esc();
}
 
int main() {
    // cambiamos el valor de k para ver diferentes niveles de corrección
    ejercicio7_vignette(0.4);  // Corrección moderada
    // ejercicio7_vignette(0.3);  // Corrección suave
    // ejercicio7_vignette(0.5);  // Corrección fuerte
    return 0;
}
