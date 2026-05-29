#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

#include "masa.h"

struct masa{
    double x, y, m;
    bool es_fijo;
    size_t id;
};

void masa_obtener_coords(double *x, double *y, masa_t *masa){
    *x = masa->x;
    *y = masa->y;
}

void masa_cambiar_coords(masa_t *masa, double x, double y){
    masa->x = x;
    masa->y = y;
}

size_t masa_obtener_id(masa_t *masa){
    return masa->id;
}

void masa_cambiar_id(masa_t *masa, size_t id){
    masa->id = id;
}

void masa_cambiar_masa(masa_t *masa, double m){
    masa->m = m;
}

double masa_obtener_masa(masa_t *masa){
    return masa->m;
}

bool masa_es_fijo(masa_t *masa){
    return masa->es_fijo;
}

masa_t *masa_crear(double x, double y, bool es_fijo, size_t id, double m){
    masa_t *masa = malloc(sizeof(masa_t));
    if(masa == NULL) return NULL;
    masa->x = x;
    masa->y = y;
    masa->es_fijo = es_fijo;
    masa->id = id;
    masa->m = m;
    return masa;
}

masa_t *masa_clonar(masa_t *masa){
    return masa_crear(masa->x, masa->y, masa->es_fijo, masa->id, masa->m);
}

void masa_destruir(masa_t *masa){
    free(masa);
}



