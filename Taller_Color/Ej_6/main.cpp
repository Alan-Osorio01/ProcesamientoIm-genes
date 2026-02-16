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
 
void ejercicio6_gamma(double gamma = 1.5) {
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
    cout << "Aplicando correccion Gamma = " << gamma << endl;
    // ========== PASO 1: Crear tabla de lookup (para eficiencia) ==========
    // Pre-calcular la transformación para todos los valores 0-255
    unsigned char lookup_table[256];
    for (int i = 0; i < 256; i++) {
        double normalized = i / 255.0;  // Normalizar a [0,1]
        double corrected = pow(normalized, gamma);  // Aplicar gamma
        lookup_table[i] = (unsigned char)(corrected * 255.0);  // Desnormalizar
    }
    // Mostrar algunos valores de ejemplo
    cout << "Valores de lookup table (ejemplo):" << endl;
    cout << "  Pixel 0   -> " << (int)lookup_table[0] << endl;
    cout << "  Pixel 50  -> " << (int)lookup_table[50] << endl;
    cout << "  Pixel 100 -> " << (int)lookup_table[100] << endl;
    cout << "  Pixel 150 -> " << (int)lookup_table[150] << endl;
    cout << "  Pixel 200 -> " << (int)lookup_table[200] << endl;
    cout << "  Pixel 255 -> " << (int)lookup_table[255] << endl;
    // ========== PASO 2: Aplicar transformación a cada píxel ==========
    Mat img_resultado(rows, cols, CV_8UC3);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel = img_bgr.at<Vec3b>(i, j);
            // Aplicar lookup table a cada canal
            img_resultado.at<Vec3b>(i, j) = Vec3b(
                lookup_table[pixel[0]],  // Canal B
                lookup_table[pixel[1]],  // Canal G
                lookup_table[pixel[2]]   // Canal R
            );
        }
    }
    cout << "Correccion Gamma completada!" << endl;
    // Mostrar resultados al mismo tamano
    Mat img_resultado_mismo_tamano = ajustar_tamano(img_resultado, img_bgr.size());
    imshow("Original", img_bgr);
    imshow("Gamma = " + to_string(gamma), img_resultado_mismo_tamano);
    cout << "Presiona 'q' o ESC para cerrar." << endl;
    esperar_q_o_esc();
}
 
int main() {
    // Puedes cambiar el valor de gamma aquí
    ejercicio6_gamma(1.5);  // Oscurece
    // ejercicio6_gamma(0.5);  // Aclara
    // ejercicio6_gamma(2.2);  // Oscurece más
    return 0;
}
