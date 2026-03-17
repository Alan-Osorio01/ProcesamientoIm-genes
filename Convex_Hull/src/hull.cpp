#include "hull.h"
#include <algorithm>
#include <cmath>

double cruzado(const Point2d& O, const Point2d& A, const Point2d& B) {
    return (A.x - O.x) * (B.y - O.y) -
           (A.y - O.y) * (B.x - O.x);
}

vector<Point> convexHullManual(const vector<Point>& puntos) {
    vector<Point> hull;
    int n = (int)puntos.size();
    if (n < 3) { hull = puntos; return hull; }

    // PIVOT
    int idxPivot = 0;
    for (int i = 1; i < n; i++) {
        if (puntos[i].y < puntos[idxPivot].y ||
           (puntos[i].y == puntos[idxPivot].y &&
            puntos[i].x <  puntos[idxPivot].x))
            idxPivot = i;
    }
    Point2d pivot(puntos[idxPivot].x, puntos[idxPivot].y);

    // ORDENAR POR ÁNGULO POLAR
    vector<Point2d> pts;
    pts.reserve(n - 1);
    for (int i = 0; i < n; i++) {
        if (i == idxPivot) continue;
        pts.push_back(Point2d(puntos[i].x, puntos[i].y));
    }
    sort(pts.begin(), pts.end(),
         [pivot](const Point2d& a, const Point2d& b) {
             double angA = atan2(a.y - pivot.y, a.x - pivot.x);
             double angB = atan2(b.y - pivot.y, b.x - pivot.x);
             if (fabs(angA - angB) < 1e-9) {
                 double da = (a.x-pivot.x)*(a.x-pivot.x)+(a.y-pivot.y)*(a.y-pivot.y);
                 double db = (b.x-pivot.x)*(b.x-pivot.x)+(b.y-pivot.y)*(b.y-pivot.y);
                 return da < db;
             }
             return angA < angB;
         });

    // PILA
    vector<Point2d> pila;
    pila.push_back(pivot);
    pila.push_back(pts[0]);
    pila.push_back(pts[1]);

    // DESCARTAR DERECHAS
    for (int i = 2; i < (int)pts.size(); i++) {
        while (pila.size() >= 2) {
            Point2d tope   = pila.back();
            Point2d penult = pila[pila.size() - 2];
            if (cruzado(penult, tope, pts[i]) <= 0)
                pila.pop_back();
            else break;
        }
        pila.push_back(pts[i]);
    }

    hull.reserve(pila.size());
    for (const auto& p : pila)
        hull.emplace_back((int)round(p.x), (int)round(p.y));

    return hull;
}

vector<vector<Point>> ramaArea(const Mat& gray, Mat& outBinario) {
    // Preprocesamiento mejorado
    Mat blur, adaptativo;
    GaussianBlur(gray, blur, Size(7, 7), 1.5);
    adaptiveThreshold(blur, adaptativo, 255, ADAPTIVE_THRESH_GAUSSIAN_C,
                      THRESH_BINARY, 21, 5);
    
    // Morfología: limpiar ruido
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    morphologyEx(adaptativo, outBinario, MORPH_CLOSE, kernel, Point(-1, -1), 2);
    morphologyEx(outBinario, outBinario, MORPH_OPEN, kernel, Point(-1, -1), 1);
    
    vector<vector<Point>> contornos;
    vector<Vec4i> hierarchy;
    findContours(outBinario.clone(), contornos, hierarchy,
                 RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    return contornos;
}

vector<vector<Point>> ramaPerimetro(const Mat& gray, Mat& outCanny) {
    Mat blur;
    GaussianBlur(gray, blur, Size(7, 7), 1.5);
    Canny(blur, outCanny, 50, 150);  // Thresholds más sensibles
    
    // Dilatar para conectar bordes fragmentados
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    dilate(outCanny, outCanny, kernel, Point(-1, -1), 2);
    
    vector<vector<Point>> contornos;
    vector<Vec4i> hierarchy;
    findContours(outCanny.clone(), contornos, hierarchy,
                 RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    return contornos;
}

//Detectar nombre de figura por número de vértices del hull 
static string nombreFigura(int vertices) {
    switch (vertices) {
        case 3:  return "Triangulo";
        case 4:  return "Cuadrilatero";
        case 5:  return "Pentagono";
        case 6:  return "Hexagono";
        default: return vertices > 6 ? "Circulo" : "?";
    }
}

// Calcular ángulo entre tres puntos
double calcularAngulo(const Point& p1, const Point& vertex, const Point& p2) {
    double dx1 = p1.x - vertex.x;
    double dy1 = p1.y - vertex.y;
    double dx2 = p2.x - vertex.x;
    double dy2 = p2.y - vertex.y;
    
    double ang1 = atan2(dy1, dx1);
    double ang2 = atan2(dy2, dx2);
    double angle = fabs(ang2 - ang1) * 180.0 / M_PI;
    
    if (angle > 180.0) angle = 360.0 - angle;
    return angle;
}

// Validar forma por ángulos internos (triángulo ~60°, cuadrado ~90°, etc)
bool esFormaValida(const vector<Point>& hull, int numVertices, double tolerancia) {
    if ((int)hull.size() != numVertices) return false;
    if (numVertices < 3) return false;
    
    double esperado = 180.0 * (numVertices - 2) / numVertices;
    
    int validosOk = 0;
    for (int i = 0; i < (int)hull.size(); i++) {
        Point p1 = hull[(i - 1 + numVertices) % numVertices];
        Point vertex = hull[i];
        Point p2 = hull[(i + 1) % numVertices];
        
        double angle = calcularAngulo(p1, vertex, p2);
        
        // Permitir cierta tolerancia en los ángulos
        if (fabs(angle - esperado) <= tolerancia) {
            validosOk++;
        }
    }
    
    // Validar al menos 60% de los ángulos
    return (validosOk >= (int)(numVertices * 0.6));
}

// Calcular esfericidad (4π * área / perímetro²)
double calcularEsfericidad(const vector<Point>& contorno) {
    double area = contourArea(contorno);
    double perim = arcLength(contorno, true);
    
    if (perim < 1e-6) return 0.0;
    
    double esfericidad = (4.0 * M_PI * area) / (perim * perim);
    return esfericidad;  // 1.0 = círculo perfecto, < 0.5 = forma alargada
}

void dibujarHull(Mat& canvas,
                 const vector<vector<Point>>& contornos,
                 const Scalar& colorContorno,
                 const Scalar& colorHull,
                 const string& etiqueta) {

    for (const auto& c : contornos) {
        if (c.size() < 15) continue;  // Filtro más estricto

        // Filtro 1: área mínima (ignora ruido pequeño)
        double area = contourArea(c);
        if (area < 3000.0) continue;  // Área mínima aumentada

        // Simplificar contorno antes de pasar al hull
        vector<Point> aprox;
        double epsilon = 0.015 * arcLength(c, true);  // Epsilon más conservador
        approxPolyDP(c, aprox, epsilon, true);
        if (aprox.size() < 3) continue;

        // Hull manual sobre contorno simplificado
        vector<Point> hull = convexHullManual(aprox);
        if (hull.size() < 3) continue;

        // Filtro 2: área del hull razonable (no frame entero)
        double areaHull = contourArea(hull);
        if (areaHull > canvas.rows * canvas.cols * 0.85) continue;
        if (areaHull < 2000.0) continue;

        // Perímetro del hull
        double perimetro = arcLength(hull, true);
        
        // Calcular esfericidad para mejor clasificación
        double esf = calcularEsfericidad(aprox);
        
        // Validar forma por ángulos
        bool esTriangulo = esFormaValida(hull, 3, 20.0);
        bool esCuadrilatero = esFormaValida(hull, 4, 18.0);
        bool esCirculo = esf > 0.75;  // Muy esférico

        // Nombre de la figura
        string figura = nombreFigura((int)hull.size());
        
        // Si es demasiado circular, es un círculo
        if (esCirculo) figura = "Circulo";

        // Contorno suavizado (tenue)
        drawContours(canvas, vector<vector<Point>>{aprox},
                     -1, colorContorno, 1, LINE_AA);

        // Hull convexo (grueso)
        polylines(canvas, hull, true, colorHull, 2, LINE_AA);

        // Centroide
        Moments mo = moments(c);
        if (fabs(mo.m00) < 1e-3) continue;
        int cx = (int)(mo.m10 / mo.m00);
        int cy = (int)(mo.m01 / mo.m00);

        // Etiqueta con figura, área, perímetro y esfericidad
        string linea1 = figura;
        string linea2 = etiqueta + " A=" + to_string((int)area) +
                        "  P=" + to_string((int)perimetro);
        string linea3 = "E=" + to_string(esf).substr(0, 4);

        // Fondo semitransparente para el texto
        int tw = 220, th = 52;
        Rect rectTxt(cx - tw/2, cy - th, tw, th);
        rectTxt &= Rect(0, 0, canvas.cols, canvas.rows); // clamp
        Mat roi = canvas(rectTxt);
        Mat overlay = roi.clone();
        rectangle(overlay, Rect(0,0,rectTxt.width,rectTxt.height),
                  Scalar(20,20,20), FILLED);
        addWeighted(overlay, 0.55, roi, 0.45, 0, roi);

        putText(canvas, linea1, Point(cx - 55, cy - 28),
                FONT_HERSHEY_SIMPLEX, 0.6, colorHull, 2, LINE_AA);
        putText(canvas, linea2, Point(cx - 100, cy - 10),
                FONT_HERSHEY_SIMPLEX, 0.35, Scalar(220,220,220), 1, LINE_AA);
        putText(canvas, linea3, Point(cx - 35, cy + 6),
                FONT_HERSHEY_SIMPLEX, 0.35, Scalar(180,220,180), 1, LINE_AA);
    }
}
