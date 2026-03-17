## Autorres

#### Alan Osorio 
#### Camilo gallardo 


## Filtros Implementados

1. **Gray** - Conversión a escala de grises usando pesos RGB estándar
2. **LoG (Laplacian of Gaussian)** - Detector de bordes que aplica suavizado Gaussiano seguido del Laplaciano
3. **Zero Crossing** - Detecta cruces por cero en la salida del Laplaciano
4. **Scharr** - Detector de bordes usando kernels Scharr (mejor que Sobel para ángulos pequeños)
5. **Laplaciano** - Kernel Laplaciano estándar para detección de bordes de segundo orden
6. **Sobel Magnitude** - Magnitud del gradiente usando kernels Sobel
7. **Canny** - Detector de bordes Canny con supresión no-máxima e histéresis
8. **Todos en cuadrícula** - Visualiza todos los filtros simultáneamente en una cuadrícula 2x4

## Requisitos

- **CMake** 3.10 o superior
- **OpenCV** 4.x
- **C++17** o superior
- **Compilador GCC o Clang**

### Instalación de dependencias (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install cmake g++ libopencv-dev
```

### Instalación de zenity (para diálogo de archivo)

```bash
sudo apt-get install zenity
```

## Compilación

```bash
cd /ruta/del/proyecto
mkdir build
cd build
cmake ..
make
```

## Uso

### Ejecutar el programa

```bash
./FILTROS
```

### Controles

**Con cámara web:**
- Presionar 'q' o ESC para salir

**Con imagen:**
- Presionar cualquier tecla para cerrar la ventana

## Estructura del Proyecto

```
filtros/
├── include/
│   ├── filters.h      # Declaraciones de funciones de filtros
│   └── utils.h        # Funciones utilitarias
├── src/
│   ├── canny.cpp      # Implementación del filtro Canny
│   ├── gray.cpp       # Conversión a escala de grises
│   ├── laplacian.cpp  # Kernel Laplaciano
│   ├── log.cpp        # Laplacian of Gaussian
│   ├── scharr.cpp     # Filtro Scharr
│   ├── sobel_magnitude.cpp  # Magnitud Sobel
│   ├── utils.cpp      # Funciones utilitarias
│   └── zero_crossing.cpp    # Detección de cruces por cero
├── CMakeLists.txt     # Configuración de CMake
├── main.cpp           # Función principal
└── README.md          # Este archivo
```


## Algoritmos Implementados

### Convolución Flotante
Aplica un kernel a una imagen con valores flotantes para mayor precisión en operaciones matemáticas.

### Gaussian Blur Manual
Crea un kernel Gaussiano dinámico basado en tamaño y sigma, luego aplica convolución.

### Non-Maximum Suppression (Canny)
Elimina píxeles no-máximos en la dirección del gradiente para afinar bordes.

### Double Thresholding (Canny)
Clasifica píxeles en fuertes, débiles y no-bordes usando dos umbrales.

### Hysteresis (Canny)
Rastreo de bordes débiles conectados a bordes fuertes para producir bordes continuos.

