#include <iostream>
#include <string>
#include "camara.h"
#include "imagen.h"

int main(int argc, char** argv) {
    try {
        if (argc >= 2) {
            // Modo imagen
            const std::string path = argv[1];
            cv::Mat img = imagen::load(path);
            if (img.empty()) return 1;

            std::cout << "Imagen cargada correctamente: " << path << "\n";
            imagen::show(img, "OpenCV - Image");
            return 0;
        }

        // Modo cámara
        std::cout << "Abriendo camara... (presiona Q o ESC para salir)\n";
        camara cam(0);
        cam.runPreview("OpenCV - camara");
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Excepcion: " << e.what() << "\n";
        return 1;
    }
}
