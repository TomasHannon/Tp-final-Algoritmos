#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "escrituraylectura.h"


bool leer_numero_de_datos(FILE *f, size_t *numero){
    uint32_t num;
    if(fread(&num, sizeof(uint32_t), 1, f) == 1){
        *numero = num;
        return true;
    }
    return false;
}


bool leer_masa(FILE *f, double *x, double *y, double *masa, bool *es_fijo, size_t *id){
    uint32_t idr;
    uint8_t es_fijor;
    if(fread(x, sizeof(double), 1,f) == 1 && 
    fread(y, sizeof(double), 1,f) == 1 && 
    fread(masa, sizeof(double), 1, f) == 1 && 
    fread(&es_fijor, sizeof(uint8_t), 1, f) == 1 &&
    fread(&idr, sizeof(uint32_t), 1, f) == 1){
        *id = idr;
        *es_fijo = es_fijor;
        return true;
    }
    return false;
}


bool leer_resorte(FILE *f, size_t *na, size_t *nb, double *k, double *l){
    uint32_t nar, nbr;
    if(fread(&nar, sizeof(uint32_t), 1,f) == 1 &&
    fread(&nbr, sizeof(uint32_t), 1,f) == 1 &&
    fread(k, sizeof(double), 1, f) == 1 &&
    fread(l, sizeof(double), 1, f) == 1){
        *na = nar;
        *nb = nbr;
        return true;
    }
    return false;
}

bool escribir_numero_de_datos(FILE *f, size_t numero){
    uint32_t num;
    num = numero;
    return(fwrite(&num, sizeof(uint32_t), 1, f) == 1);
}

bool escribir_masa(FILE *f, double x, double y, double masa, bool es_fijo, size_t id){
    uint32_t idw = id;
    uint8_t es_fijow = es_fijo;
    return (fwrite(&x, sizeof(double), 1, f) == 1 &&
    fwrite(&y, sizeof(double), 1, f) == 1 &&
    fwrite(&masa, sizeof(double), 1, f) == 1 &&
    fwrite(&es_fijow, sizeof(uint8_t), 1, f) == 1 &&
    fwrite(&idw, sizeof(uint32_t), 1, f) == 1);
}

bool escribir_resorte(FILE *f, size_t na, size_t nb, double k, double l){
    uint32_t naw = na;
    uint32_t nbw = nb;
    return(fwrite(&naw, sizeof(uint32_t), 1, f) == 1 &&
    fwrite(&nbw, sizeof(uint32_t), 1, f) == 1 &&
    fwrite(&k, sizeof(double), 1, f) == 1 &&
    fwrite(&l, sizeof(double), 1, f) == 1);
}