#ifndef MATRICES_H
#define MATRICES_H

#include <stdlib.h>
#include <stddef.h>

typedef struct matriz matriz_t;

void matriz_destruir(matriz_t *matriz);
// Precondición: El puntero `matriz_t` `matriz` debe apuntar a un objeto de matriz válido.
// Postcondición: El objeto de matriz apuntado por `matriz` se destruye y se libera cualquier memoria asociada.

matriz_t *_matriz_crear(size_t n, size_t m);
// Precondición: Los parámetros `n` y `m` representan dimensiones válidas para la matriz.
// Postcondición: Se crea un nuevo objeto de matriz de tamaño `n` x `m` y se devuelve un puntero a él.
// El puntero devuelto debe manejarse correctamente para evitar fugas de memoria.

size_t matriz_filas(const matriz_t *matriz);
// Precondición: El puntero `matriz_t` `matriz` debe apuntar a un objeto de matriz válido.
// Postcondición: Devuelve el número de filas en la matriz.

size_t matriz_columnas(const matriz_t *matriz);
// Precondición: El puntero `matriz_t` `matriz` debe apuntar a un objeto de matriz válido.
// Postcondición: Devuelve el número de columnas en la matriz.

void matriz_dimensiones(const matriz_t *matriz, size_t *filas, size_t *columnas);
// Precondición: El puntero `matriz_t` `matriz` debe apuntar a un objeto de matriz válido.
// Los punteros `filas` y `columnas` deben ser válidos y escribibles.
// Postcondición: El número de filas en la matriz se almacena en la variable apuntada por `filas`,
// y el número de columnas se almacena en la variable apuntada por `columnas`.

double matriz_obtener(const matriz_t *matriz, size_t fila, size_t columna);
// Precondición: El puntero `matriz_t` `matriz` debe apuntar a un objeto de matriz válido.
// Los parámetros `fila` y `columna` deben representar índices válidos dentro de las dimensiones de la matriz.
// Postcondición: Devuelve el valor en la posición especificada (`fila`, `columna`) dentro de la matriz.

void matriz_sumar_a_posicion(matriz_t *matriz, size_t fila, size_t columna, double valor);
// Precondición: El puntero `matriz_t` `matriz` debe apuntar a un objeto de matriz válido.
// Los parámetros `fila` y `columna` deben representar índices válidos dentro de las dimensiones de la matriz.
// Postcondición: Agrega el valor del parámetro `valor` al valor en la posición especificada (`fila`, `columna`) dentro de la matriz.

matriz_t *matriz_crear(size_t n, size_t m, double a[n][m]);
// Precondición: Los parámetros `n` y `m` representan dimensiones válidas para la matriz.
// El parámetro `a` es una matriz 2D de tamaño `n` x `m`.
// Postcondición: Crea un nuevo objeto de matriz de tamaño `n` x `m` y lo inicializa con los valores proporcionados en la matriz `a`.
// Se devuelve un puntero al nuevo objeto de matriz. El puntero devuelto debe manejarse correctamente para evitar fugas de memoria.

matriz_t *matriz_clonar(const matriz_t *matriz);
// Precondición: El puntero `matriz_t` `matriz` debe apuntar a un objeto de matriz válido.
// Postcondición: Crea una copia del objeto de matriz original `matriz`, y se devuelve un puntero a la nueva matriz.
// El puntero devuelto debe manejarse correctamente para evitar fugas de memoria.

matriz_t *matriz_identidad(size_t n);
// Precondición: El parámetro `n` representa una dimensión válida para la matriz cuadrada.
// Postcondición: Crea un nuevo objeto de matriz cuadrada de tamaño `n` x `n` con todos los elementos diagonales establecidos en 1
// y todos los demás elementos establecidos en 0. Se devuelve un puntero a la nueva matriz.
// El puntero devuelto debe manejarse correctamente para evitar fugas de memoria.

matriz_t *matriz_ceros(size_t n, size_t m);
// Precondición: Los parámetros `n` y `m` representan dimensiones válidas para la matriz.
// Postcondición: Crea un nuevo objeto de matriz de tamaño `n` x `m` con todos los elementos establecidos en 0.
// Se devuelve un puntero a la nueva matriz. El puntero devuelto debe manejarse correctamente para evitar fugas de memoria.

matriz_t *matriz_multiplicar(const matriz_t *a, const matriz_t *b);
// Precondición: Los punteros `matriz_t` `a` y `b` deben apuntar a objetos de matriz válidos.
// Las dimensiones de `a` y `b` deben ser compatibles para la multiplicación de matrices, de lo contrario devuelve NULL.
// Postcondición: Realiza la multiplicación de matrices entre `a` y `b` y devuelve un puntero a la nueva matriz resultante.
// El puntero devuelto debe manejarse correctamente para evitar fugas de memoria.

void matriz_imprimir(matriz_t *matriz);
// Precondición: El puntero `matriz_t` `matriz` debe apuntar a un objeto de matriz válido.
// Postcondición: Imprime los elementos de la matriz en una forma legible por humanos en la salida estándar.
// No se devuelve ningún valor.

#endif