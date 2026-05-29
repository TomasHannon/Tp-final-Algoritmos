#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "matrices.h"

struct matriz{
    double **m;
    size_t filas, columnas;
};

void matriz_destruir(matriz_t *matriz){
    for(size_t i = 0; i < matriz->filas; i++){
        free(matriz->m[i]);
    }
    free(matriz->m);
    free(matriz);
}

matriz_t *_matriz_crear(size_t n, size_t m){
    matriz_t *matriz = malloc(sizeof(matriz_t));
    if(matriz == NULL){
        return NULL;
    }
    matriz->filas = n;
    matriz->columnas = m;
    matriz->m = malloc(n * sizeof(double*));
    if(matriz->m == NULL){
        free(matriz);
        return NULL;
    }
    for(size_t i = 0; i < n; i++){
        matriz->m[i] = malloc(m * sizeof(double));
        if(matriz->m[i] == NULL){
            matriz->filas = i;
            matriz_destruir(matriz);
            return NULL;
        }
    }
    return matriz;
}
size_t matriz_filas(const matriz_t *matriz){
    return matriz->filas;
}
size_t matriz_columnas(const matriz_t *matriz){
    return matriz->columnas;
}
void matriz_dimensiones(const matriz_t *matriz, size_t *filas, size_t *columnas){
    *filas = matriz->filas;
    *columnas = matriz->columnas;
}

double matriz_obtener(const matriz_t *matriz, size_t fila, size_t columna){
    return matriz->m[fila][columna];
}

void matriz_sumar_a_posicion(matriz_t *matriz, size_t fila, size_t columna, double valor){
    matriz->m[fila][columna] += valor;
}
matriz_t *matriz_crear(size_t n, size_t m, double a[n][m]){
    matriz_t *matriz = _matriz_crear(n,m);
    if(matriz == NULL){
        return NULL;
    }
    for(size_t i = 0; i < n; i++){
        for(size_t j = 0; j < m; j++){
            matriz->m[i][j] = a[i][j];
        }
    }
    return matriz;
}
matriz_t *matriz_clonar(const matriz_t *matriz){
    matriz_t *matriz_clon = _matriz_crear(matriz->filas,matriz->columnas);
    if(matriz_clon == NULL){
        return NULL;
    }
    matriz_clon->filas = matriz->filas;
    matriz_clon->columnas = matriz->columnas;
    for(size_t i = 0; i < matriz->filas; i++){
        for(size_t j = 0; j < matriz->columnas; j++){
            matriz_clon->m[i][j] = matriz->m[i][j];
        }
    }
    return matriz_clon;
}
matriz_t *matriz_identidad(size_t n){
    matriz_t *matriz = _matriz_crear(n, n);
    if(matriz == NULL){
        return NULL;
    }
    for(size_t i = 0; i < n; i++){
        for(size_t j = 0; j < n; j++){
            if(j == i){
                matriz->m[i][j] = 1;
            } else {
                matriz->m[i][j] = 0;
            }
        }
    }
    return matriz;
}

matriz_t *matriz_ceros(size_t n, size_t m){
    matriz_t *matriz = _matriz_crear(n, m);
    if(matriz == NULL) return NULL;

    for(size_t i = 0; i < n; i++){
        for(size_t j = 0; j < m; j++){
            matriz->m[i][j] = 0;
        }
    }
    return matriz;
}

matriz_t *matriz_multiplicar(const matriz_t *a, const matriz_t *b){
    if(a->columnas != b->filas){
        return NULL;
    }
    matriz_t *matriz = _matriz_crear(a->filas, b->columnas);
    if(matriz == NULL){
        return NULL;
    }
    for(size_t i = 0; i < a->filas; i++){
        for(size_t k = 0; k < b->columnas; k++){
            double fila_aux = 0;
            for(size_t j = 0; j < a->columnas; j++){
                fila_aux += a->m[i][j] * b->m[j][k];
            }
        matriz->m[i][k] = fila_aux;
        }
    }
    return matriz;
}

void matriz_imprimir(matriz_t *matriz){
    for(size_t f = 0; f < matriz->filas; f++){
        for(size_t c = 0; c < matriz->columnas; c++){
            printf("%.8f\t",matriz->m[f][c]);
        }
        putchar('\n');
    }
    printf("---------------------------------------------\n");
}