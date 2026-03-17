#ifndef HULL_H
#define HULL_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace cv;
using namespace std;


// Producto cruzado OA x OB
// > 0 → giro izquierda (CCW)   <= 0 → giro derecha o colineal (descartable)
double cruzado(const Point2d& O, const Point2d& A, const Point2d& B);


vector<Point> convexHullManual(const vector<Point>& puntos);

// ÁREA: umbralizar → binario → contornos
vector<vector<Point>> ramaArea(const Mat& gray, Mat& outBinario);

// PERÍMETRO: gaussiano → canny → contornos
vector<vector<Point>> ramaPerimetro(const Mat& gray, Mat& outCanny);

// ── Validar forma por ángulos internos
bool esFormaValida(const vector<Point>& hull, int numVertices, double tolerancia = 15.0);

// ── Calcular esfericidad (circularity)
double calcularEsfericidad(const vector<Point>& contorno);

// ── Dibujar hull + área + perímetro sobre frame 
void dibujarHull(Mat& canvas,
                 const vector<vector<Point>>& contornos,
                 const Scalar& colorContorno,
                 const Scalar& colorHull,
                 const string& etiqueta);

#endif
