# Autores

#### Alan Osorio 
#### Camilo Gallardo 

# Convex Hull 

Detector de formas geométricas en tiempo real usando visión por computadora. Identifica **triángulos, cuadriláteros, círculos** y otras formas usando la cámara web.

## 📦 Requisitos

- **CMake** >= 3.10
- **OpenCV** >= 4.0
- **C++11** o superior
- **Cámara web** conectada

### Instalar OpenCV (Ubuntu/Debian)
```bash
sudo apt-get install libopencv-dev
```

## 🔨 Compilación

```bash
cd build
cmake ..
make
```

## ▶️ Ejecución

```bash
./convex
```

## 🎮 Controles

| Tecla | Función |
|-------|---------|
| **A** | Modo Área (análisis de región) |
| **P** | Modo Perímetro (análisis de bordes) |
| **C** | Modo Combinado (recomendado) |
| **ESC** | Salir |

## 📊 Información Mostrada

Para cada forma detectada se muestra:
- **Nombre**: Triángulo, Cuadrilátero, Círculo, etc.
- **Área (A)**: Cantidad de píxeles
- **Perímetro (P)**: Longitud del contorno
- **Esfericidad (E)**: Qué tan circular es (0-1)

## 📁 Estructura del Proyecto

```
.
├── CMakeLists.txt          # Configuración de compilación
├── main.cpp                # Programa principal
├── include/
│   └── hull.h             # Definiciones de funciones
└── src/
    └── hull.cpp           # Implementación de algoritmos
```

## 🧮 Algoritmos

- **Convex Hull Manual**: Algoritmo de Graham Scan modificado
- **Umbralización Adaptativa**: Para detección robusta de áreas
- **Canny Edge Detection**: Para análisis de perímetros
- **Morfología Digital**: Limpieza y procesamiento de imágenes

## 💡 Modo de Uso

**Modo Área**: Mejor para objetos sólidos bien iluminados
```
A = Umbralización adaptativa + Morfología
```

**Modo Perímetro**: Mejor para contornos bien definidos
```
P = Blur Gaussiano + Canny + Dilatación
```

**Modo Combinado**: Usa ambos métodos simultáneamente
```
C = Área + Perímetro (máxima precisión)
```

## 🎬 Ejemplo de Uso

1. Ejecuta `./convex`
2. Forma un triángulo con tus manos frente a la cámara
3. El programa detectará automáticamente la forma
4. Verás etiquetas con: nombre, área y perímetro
5. Presiona **ESC** para salir

## 🔧 Ajustes Disponibles

Para modificar la sensibilidad, edita `src/hull.cpp`:

```cpp
// Área mínima de detección
if (area < 3000.0) continue;  // Aumenta para menos sensibilidad

// Umbral de Canny
Canny(blur, outCanny, 50, 150);  // Valores: (bajo, alto)

// Tolerancia en ángulos
esFormaValida(hull, 3, 20.0);  // ±20° para triángulos
```