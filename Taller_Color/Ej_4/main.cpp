#include <opencv2/opencv.hpp>
#include <iostream>
 
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
 
void ejercicio4_gray_world() {
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
    int total_pixels = rows * cols;
    cout << "Aplicando Gray World a imagen de " << rows << "x" << cols << " pixeles" << endl;
    // ========== PASO 1: Calcular suma de cada canal ==========
    double sum_B = 0.0;
    double sum_G = 0.0;
    double sum_R = 0.0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel = img_bgr.at<Vec3b>(i, j);
            sum_B += pixel[0];
            sum_G += pixel[1];
            sum_R += pixel[2];
        }
    }
    // ========== PASO 2: Calcular promedios ==========
    double avg_B = sum_B / total_pixels;
    double avg_G = sum_G / total_pixels;
    double avg_R = sum_R / total_pixels;
    cout << "Promedio B: " << avg_B << endl;
    cout << "Promedio G: " << avg_G << endl;
    cout << "Promedio R: " << avg_R << endl;
    // ========== PASO 3: Calcular promedio gris ==========
    double gray_avg = (avg_R + avg_G + avg_B) / 3.0;
    cout << "Promedio gris: " << gray_avg << endl;
    // ========== PASO 4: Calcular factores de escala ==========
    double scale_B = gray_avg / avg_B;
    double scale_G = gray_avg / avg_G;
    double scale_R = gray_avg / avg_R;
    cout << "Factor escala B: " << scale_B << endl;
    cout << "Factor escala G: " << scale_G << endl;
    cout << "Factor escala R: " << scale_R << endl;
    // ========== PASO 5: Crear imagen corregida ==========
    Mat img_resultado(rows, cols, CV_8UC3);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel = img_bgr.at<Vec3b>(i, j);
            // Aplicar factores de escala y limitar a [0, 255]
            double B_nuevo = min(pixel[0] * scale_B, 255.0);
            double G_nuevo = min(pixel[1] * scale_G, 255.0);
            double R_nuevo = min(pixel[2] * scale_R, 255.0);
            img_resultado.at<Vec3b>(i, j) = Vec3b(
                (uchar)B_nuevo,
                (uchar)G_nuevo,
                (uchar)R_nuevo
            );
        }
    }
    cout << "Correccion Gray World completada!" << endl;
    // Mostrar resultados al mismo tamano
    Mat img_resultado_mismo_tamano = ajustar_tamano(img_resultado, img_bgr.size());
    imshow("Original", img_bgr);
    imshow("Gray World", img_resultado_mismo_tamano);
    cout << "Presiona 'q' o ESC para cerrar." << endl;
    esperar_q_o_esc();
}
 
int main() {
    ejercicio4_gray_world();
    return 0;
}
