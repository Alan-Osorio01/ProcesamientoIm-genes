#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>
#include "processor.h"

using namespace cv;
using namespace std;

int main() {

    // ── Cargar imagen satelital en escala de grises ───────────────────────────
    std::string rutaImagen = "satelite.png";
    if (!std::filesystem::exists(rutaImagen) && std::filesystem::exists("build/satelite.png")) {
        rutaImagen = "build/satelite.png";
    }

    Mat img = imread(rutaImagen, IMREAD_GRAYSCALE);

    if (img.empty()) {
        cout << "Error: no se encontro 'satelite.png'" << endl;
        cout << "Rutas probadas: ./satelite.png y ./build/satelite.png" << endl;
        cout << "Ejecuta desde la raiz del proyecto o mueve la imagen a una de esas rutas." << endl;
        return -1;
    }

    // ════════════════════════════════════════════════════════════════════════
    // SECCIÓN 1 — Representación digital e histograma
    // ════════════════════════════════════════════════════════════════════════
    imprimirInfoImagen(img);
    imshow("Imagen Original", img);
    imwrite("out_01_original.png", img);

    /*
     * PREGUNTAS:
     * - ¿Alto contraste?   → revisar rango de píxeles en histograma
     * - ¿Tonos oscuros?    → media baja (< 100) indica agua o sombra
     * - Agua ~ muy oscuro | Vegetación ~ gris medio | Ciudad ~ brillante
     */

    // ════════════════════════════════════════════════════════════════════════
    // SECCIÓN 2 — Modelado Gaussiano y Segmentación
    // ════════════════════════════════════════════════════════════════════════
    Scalar media, desviacion;
    calcularEstadisticas(img, media, desviacion);

    // Segmentación con 1σ, 2σ, 3σ para comparar
    Mat mask1 = segmentacionGaussiana(img, media, desviacion, 1.0);
    Mat mask2 = segmentacionGaussiana(img, media, desviacion, 2.0);
    Mat mask3 = segmentacionGaussiana(img, media, desviacion, 3.0);

    imshow("Segmentacion 1-sigma", mask1);
    imshow("Segmentacion 2-sigma (fondo)", mask2);
    imshow("Segmentacion 3-sigma", mask3);
    imwrite("out_02_mask1sigma.png", mask1);
    imwrite("out_02_mask2sigma.png", mask2);
    imwrite("out_02_mask3sigma.png", mask3);

    /*
     * PREGUNTAS:
     * - 2σ captura ~95% de píxeles → el "fondo" estadístico
     * - 1σ es más restrictivo → solo lo más "típico"
     * - 3σ captura casi todo → menos útil para segmentar
     */

    // ════════════════════════════════════════════════════════════════════════
    // SECCIÓN 3 — Filtro Gaussiano (Lowpass)
    // ════════════════════════════════════════════════════════════════════════
    Mat suavizada = filtroGaussiano(img, 7, 1.5);
    imshow("Suavizada Gaussiana 7x7", suavizada);
    imwrite("out_03_suavizada.png", suavizada);

    compararDesviaciones(img, suavizada);

    /*
     * ANÁLISIS:
     * - Desviación baja → varianza reducida → ruido atenuado
     * - Físicamente: los píxeles se "promedian" con vecinos
     * - Para imágenes SAR/radar: reduce ruido speckle significativamente
     */

    // ════════════════════════════════════════════════════════════════════════
    // SECCIÓN 4 — Filtros de Afilado (Highpass)
    // ════════════════════════════════════════════════════════════════════════
    Mat laplaciano = filtroLaplaciano(img);
    Mat realzada   = imagenRealzada(img, laplaciano);

    imshow("Highpass - Laplaciano", laplaciano);
    imshow("Imagen Afilada",        realzada);
    imwrite("out_04_laplaciano.png", laplaciano);
    imwrite("out_04_realzada.png",   realzada);

    /*
     * DISCUSIÓN:
     * - Sí se resaltan bordes (ríos, calles, costas)
     * - El ruido también se amplifica → por eso conviene suavizar ANTES
     * - Pipeline recomendado: Gauss → Laplaciano → Realce
     */

    // ════════════════════════════════════════════════════════════════════════
    // SECCIÓN 5 — Pipeline Completo (Gauss → Laplaciano → Realce)
    // ════════════════════════════════════════════════════════════════════════
    Mat resultado = pipelineCompleto(img);
    imshow("Pipeline Completo", resultado);
    imwrite("out_05_pipeline.png", resultado);

    // ════════════════════════════════════════════════════════════════════════
    // SECCIÓN 6 — Kernel Gaussiano 2D Manual
    // ════════════════════════════════════════════════════════════════════════

    // Análisis con diferentes sigmas
    analizarKernel(0.5);   // muy concentrado en el centro
    analizarKernel(1.0);   // balance estándar
    analizarKernel(3.0);   // muy disperso, suavizado fuerte

    /*
     * CONCEPTO:
     * - σ pequeño (0.5): peso casi todo en el centro → poco suavizado
     * - σ grande  (3.0): pesos distribuidos uniformemente → mucho suavizado
     * - Suma siempre ~1 → conserva brillo promedio (energía)
     * - Aquí σ describe PESO ESPACIAL, no intensidades de píxeles
     */

    cout << "\nTodas las imagenes guardadas como out_*.png" << endl;
    waitKey(0);
    return 0;
}
