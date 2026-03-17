#include <opencv2/opencv.hpp>
#include <iostream>
#include "hull.h"

using namespace cv;
using namespace std;

int main() {

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Error: no se puede abrir la camara" << endl;
        return -1;
    }

    cout << "[ Convex Hull ] ESC = salir  |  A = solo Area  |  P = solo Perimetro  |  C = Combinado" << endl;

    Mat frame, gray;
    char modo = 'C'; // C = combinado, A = area, P = perimetro

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cvtColor(frame, gray, COLOR_BGR2GRAY);

        // Canvas negro limpio del mismo tamaño que el frame
        Mat canvas = Mat::zeros(frame.size(), frame.type());

        // Imagen original tenue de fondo (30% opacidad)
        addWeighted(frame, 0.25, canvas, 0.75, 0, canvas);

        // ÁREA 
        if (modo == 'A' || modo == 'C') {
            Mat binario;
            vector<vector<Point>> contArea = ramaArea(gray, binario);
            dibujarHull(canvas, contArea,
                        Scalar(60, 160, 255),  // contorno: naranja suave
                        Scalar(0,  255, 120),  // hull: verde neón
                        "Area");
        }

        // PERÍMETRO 
        if (modo == 'P' || modo == 'C') {
            Mat canny;
            vector<vector<Point>> contPerim = ramaPerimetro(gray, canny);
            dibujarHull(canvas, contPerim,
                        Scalar(80,  80, 255),  // contorno: rojo suave
                        Scalar(0,  220, 255),  // hull: cian
                        "Perim");
        }

        // HUD 
        string modoStr = (modo == 'C') ? "COMBINADO" :
                         (modo == 'A') ? "AREA"      : "PERIMETRO";
        string hud = "Modo: " + modoStr + "  |  ESC salir  A/P/C cambiar modo";
        putText(canvas, hud, Point(10, frame.rows - 12),
                FONT_HERSHEY_SIMPLEX, 0.42, Scalar(180,180,180), 1, LINE_AA);

        // Solo una ventana 
        imshow("Convex Hull - Detector de Poligonos", canvas);

        char key = (char)waitKey(1);
        if (key == 27)  break;        // ESC
        if (key == 'a' || key == 'A') modo = 'A';
        if (key == 'p' || key == 'P') modo = 'P';
        if (key == 'c' || key == 'C') modo = 'C';
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
