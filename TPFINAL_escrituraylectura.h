
#ifndef escrituraylectura_H
#define escrituraylectura_H

#include <stdbool.h>
#include <stdint.h>

bool leer_numero_de_datos(FILE *f, size_t *numero);
// Precondición: El puntero `f` apunta a un archivo abierto en modo lectura.
// El puntero `numero` apunta a una variable de tipo `size_t`.
// Postcondición: Lee un número de datos del archivo `f` y lo guarda en la variable `numero`.
// Devuelve true si la lectura fue exitosa, false en caso contrario.

bool leer_masa(FILE *f, double *x, double *y, double *masa, bool *es_fijo, size_t *id);
// Precondición: El puntero `f` apunta a un archivo abierto en modo lectura.
// Los punteros `x`, `y`, `masa`, `es_fijo` e `id` apuntan a variables de tipo `double`, `double`, `double`, `bool` y `size_t` respectivamente.
// Postcondición: Lee las coordenadas (`x`, `y`), la masa, la información de fijeza (`es_fijo`) y el identificador (`id`) de una partícula del archivo `f`.
// Guarda los valores leídos en las variables correspondientes.
// Devuelve true si la lectura fue exitosa, false en caso contrario.

bool leer_resorte(FILE *f, size_t *na, size_t *nb, double *k, double *l);
// Precondición: El puntero `f` apunta a un archivo abierto en modo lectura.
// Los punteros `na`, `nb`, `k` y `l` apuntan a variables de tipo `size_t` y `double` respectivamente.
// Postcondición: Lee los identificadores de las partículas conectadas por el resorte (`na` y `nb`), la constante del resorte (`k`) y la longitud de equilibrio (`l`) del archivo `f`.
// Guarda los valores leídos en las variables correspondientes.
// Devuelve true si la lectura fue exitosa, false en caso contrario.

bool escribir_numero_de_datos(FILE *f, size_t numero);
// Precondición: El puntero `f` apunta a un archivo abierto en modo escritura.
// El parámetro `numero` es un valor de tipo `size_t`.
// Postcondición: Escribe el número de datos `numero` en el archivo `f`.
// Devuelve true si la escritura fue exitosa, false en caso contrario.

bool escribir_masa(FILE *f, double x, double y, double masa, bool es_fijo, size_t id);
// Precondición: El puntero `f` apunta a un archivo abierto en modo escritura.
// Los parámetros `x`, `y`, `masa`, `es_fijo` e `id` son valores de tipo `double`, `double`, `double`, `bool` y `size_t` respectivamente.
// Postcondición: Escribe las coordenadas (`x`, `y`), la masa, la información de fijeza (`es_fijo`) y el identificador (`id`) de una partícula en el archivo `f`.
// Devuelve true si la escritura fue exitosa, false en caso contrario.

bool escribir_resorte(FILE *f, size_t na, size_t nb, double k, double l);
// Precondición: El puntero `f` apunta a un archivo abierto en modo escritura.
// Los parámetros `na`, `nb`, `k` y `l` son valores de tipo `size_t` y `double` respectivamente.
// Postcondición: Escribe los identificadores de las partículas conectadas por el resorte (`na` y `nb`), la constante del resorte (`k`) y la longitud de equilibrio (`l`) en el archivo `f`.
// Devuelve true si la escritura fue exitosa, false en caso contrario.
#endif
