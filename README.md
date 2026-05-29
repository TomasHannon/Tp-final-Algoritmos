# Mono Bridge

Juego de simulación física desarrollado como Trabajo Práctico Final para la materia **Algoritmos y Programación** (Ingeniería, UBA).

El jugador construye un puente de masas y resortes que debe soportar su propio peso durante 10 segundos. Si ningún resorte se estira más del límite permitido, el jugador gana y avanza al siguiente nivel.

---

## Cómo jugar

- **Click izquierdo** en un espacio vacío: agrega una masa
- **Click izquierdo** sobre una masa fija (puntos de anclaje): activa modo conexión
- **Click izquierdo** sobre otra masa en modo conexión: conecta las dos masas con un resorte
- **Click derecho** sobre un resorte: lo elimina
- **Click derecho** en espacio vacío: inicia la simulación
- **Click derecho** durante la simulación: cancela y vuelve al editor

Los resortes se vuelven azules si están a punto de romperse. El nivel se guarda automáticamente al ganar.

---

## Requisitos

- GCC con soporte C99
- SDL2
- SDL2_ttf
- Fuente `FreeSansBold.ttf` en el mismo directorio que el ejecutable

En Ubuntu/Debian:
```bash
sudo apt install gcc libsdl2-dev libsdl2-ttf-dev
```

---

## Compilación y ejecución

```bash
make
./programa
```

Para cargar un nivel guardado:
```bash
./programa nivel_2.bin
```

Para limpiar los archivos compilados:
```bash
make clean
```

---

## Estructura del proyecto

| Módulo | Descripción |
|---|---|
| `masa` | Partícula con posición, masa e identificador |
| `resorte` | Vínculo elástico entre dos masas con constante k y longitud de equilibrio |
| `mallas` | Conjunto de masas y resortes; incluye iteradores propios |
| `simulacion` | Integración numérica por el método de Verlet; opera sobre una copia de la malla |
| `matrices` | Implementación genérica de matrices dinámicas usada por el simulador |
| `lista` | Lista simplemente enlazada genérica con iteradores internos y externos |
| `distancia` | Cálculo de distancia euclidiana y distancia punto-segmento |
| `escrituraylectura` | Serialización binaria de mallas para guardar y cargar niveles |
| `config.h` | Constantes globales del juego (FPS, gravedad, parámetros físicos) |
| `main.c` | Loop principal con SDL2: eventos, rendering y lógica de juego |
