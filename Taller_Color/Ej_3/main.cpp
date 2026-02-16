#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
 
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
 
struct Pixel {
    double r, g, b;
    Pixel() : r(0), g(0), b(0) {}
    Pixel(double r_, double g_, double b_) : r(r_), g(g_), b(b_) {}
};
 
double distancia_euclidiana(const Pixel& p1, const Pixel& p2) {
    double dr = p1.r - p2.r;
    double dg = p1.g - p2.g;
    double db = p1.b - p2.b;
    return sqrt(dr * dr + dg * dg + db * db);
}
 
void ejercicio3_kmeans_manual(int K = 5) {
    Mat img_bgr = imread("ImagenPrueba.jpeg");
    if (img_bgr.empty()) img_bgr = imread("imagen.jpg");
    if (img_bgr.empty()) img_bgr = imread("../ImagenPrueba.jpeg");
    if (img_bgr.empty()) img_bgr = imread("../../ImagenPrueba.jpeg");
    if (img_bgr.empty()) {
        cout << "Error: No se pudo cargar la imagen. Rutas probadas: "
             << "ImagenPrueba.jpeg, imagen.jpg, ../ImagenPrueba.jpeg, ../../ImagenPrueba.jpeg" << endl;
        return;
    }
    // Redimensionar para acelerar
    Mat img_small;
    resize(img_bgr, img_small, Size(160, 120));
    int rows = img_small.rows;
    int cols = img_small.cols;
    int total_pixels = rows * cols;
    cout << "Procesando " << total_pixels << " pixeles con K=" << K << endl;
    // ========== PASO 1: Crear array de píxeles ==========
    vector<Pixel> pixels(total_pixels);
    int idx = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel_bgr = img_small.at<Vec3b>(i, j);
            pixels[idx] = Pixel(pixel_bgr[2], pixel_bgr[1], pixel_bgr[0]); // BGR -> RGB
            idx++;
        }
    }
    // ========== PASO 2: Inicializar K centroides aleatorios ==========
    srand(time(NULL));
    vector<Pixel> centroides(K);
    for (int k = 0; k < K; k++) {
        int random_idx = rand() % total_pixels;
        centroides[k] = pixels[random_idx];
        cout << "Centroide " << k << ": (" 
<< centroides[k].r << ", " 
<< centroides[k].g << ", " 
<< centroides[k].b << ")" << endl;
    }
    // ========== PASO 3: Array para almacenar asignaciones ==========
    vector<int> asignaciones(total_pixels);
    // ========== PASO 4: Iterar K-Means ==========
    int max_iteraciones = 20;
    for (int iter = 0; iter < max_iteraciones; iter++) {
        cout << "Iteracion " << (iter + 1) << "/" << max_iteraciones << endl;
        // PASO 4a: Asignar cada píxel al centroide más cercano
        for (int i = 0; i < total_pixels; i++) {
            double min_distancia = distancia_euclidiana(pixels[i], centroides[0]);
            int cluster_asignado = 0;
            for (int k = 1; k < K; k++) {
                double distancia = distancia_euclidiana(pixels[i], centroides[k]);
                if (distancia < min_distancia) {
                    min_distancia = distancia;
                    cluster_asignado = k;
                }
            }
            asignaciones[i] = cluster_asignado;
        }
        // PASO 4b: Recalcular centroides
        vector<double> sum_r(K, 0.0);
        vector<double> sum_g(K, 0.0);
        vector<double> sum_b(K, 0.0);
        vector<int> count(K, 0);
        for (int i = 0; i < total_pixels; i++) {
            int cluster = asignaciones[i];
            sum_r[cluster] += pixels[i].r;
            sum_g[cluster] += pixels[i].g;
            sum_b[cluster] += pixels[i].b;
            count[cluster]++;
        }
        // Actualizar centroides con los promedios
        for (int k = 0; k < K; k++) {
            if (count[k] > 0) {
                centroides[k].r = sum_r[k] / count[k];
                centroides[k].g = sum_g[k] / count[k];
                centroides[k].b = sum_b[k] / count[k];
            }
        }
    }
    // ========== PASO 5: Crear imagen cuantizada ==========
    Mat img_quantized(rows, cols, CV_8UC3);
    idx = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int cluster = asignaciones[idx];
            Pixel centroide = centroides[cluster];
            img_quantized.at<Vec3b>(i, j) = Vec3b(
                (uchar)centroide.b,
                (uchar)centroide.g,
                (uchar)centroide.r
            );
            idx++;
        }
    }
    // ========== PASO 6: Crear paleta de colores ==========
    int palette_height = 50;
    int palette_width = K * 50;
    Mat paleta(palette_height, palette_width, CV_8UC3);
    for (int k = 0; k < K; k++) {
        Pixel centroide = centroides[k];
        Rect region(k * 50, 0, 50, palette_height);
        rectangle(paleta, region, 
                  Scalar(centroide.b, centroide.g, centroide.r), 
                  -1);
    }
    // Mostrar resultados al mismo tamano
    Mat img_quantized_mismo_tamano = ajustar_tamano(img_quantized, img_small.size());
    imshow("Original", img_small);
    imshow("K-Means Manual K=" + to_string(K), img_quantized_mismo_tamano);
    imshow("Paleta", paleta);
    cout << "Presiona 'q' o ESC para cerrar." << endl;
    esperar_q_o_esc();
}
 
int main() {
    ejercicio3_kmeans_manual(5);
    return 0;
}
