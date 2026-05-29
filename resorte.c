#include <math.h>
#include <stdlib.h>

#include "masa.h"
#include "resorte.h"
#include "distancia.h"

struct resorte{
    masa_t *ma;
    masa_t *mb;
    double k;
    double lo;
};

void resorte_obtener_coords(double *x1, double *y1, double *x2, double *y2, resorte_t *resorte){
    masa_obtener_coords(x1, y1, resorte->ma);
    masa_obtener_coords(x2, y2, resorte->mb);
}

double resorte_obtener_lo(resorte_t *resorte){
    return resorte->lo;
}

void resorte_cambiar_lo(resorte_t *resorte, double lo){
    resorte->lo = lo;
}

double resorte_obtener_k(resorte_t *resorte){
    return resorte->k;
}

void resorte_obtener_masas_vinculadas(resorte_t *resorte, masa_t **ma, masa_t **mb){
    *ma = resorte->ma;
    *mb = resorte->mb;
}

bool resorte_vinculado_masa(resorte_t *resorte, masa_t *masa){
    return (resorte->ma == masa || resorte->mb == masa);
}

//si se pasa un lo_max negativo o 0, lo no tiene maximo.
resorte_t *resorte_crear(masa_t *ma, masa_t *mb, double lo_max, double k){
    resorte_t *resorte = malloc(sizeof(resorte_t));
    if(resorte == NULL) return NULL;
    resorte->ma = ma;
    resorte->mb = mb;
    double x1, y1, x2, y2;
    resorte_obtener_coords(&x1, &y1, &x2, &y2, resorte);
    double lo = distancia_xy(x1, y1, x2, y2);
    resorte->lo = lo;
    resorte->k = k;
    if(lo_max > 0 && resorte->lo > lo_max){
        resorte_destruir(resorte);
        return NULL;
    }
    return resorte;
}

resorte_t *resorte_clonar(resorte_t *resorte){
    return resorte_crear(resorte->ma, resorte->mb, resorte->lo + 1, resorte->k);
}
bool resorte_actualizar(resorte_t *resorte, double lo_max, double k){
    double x1, y1, x2, y2;
    resorte_obtener_coords(&x1, &y1, &x2, &y2, resorte);
    double lo = distancia_xy(x1, y1, x2, y2);
    if(lo >= lo_max && lo_max > 0){
        return false;
    }
    resorte->lo = lo;
    resorte->k = k;
    return true;
}

void resorte_destruir(resorte_t *resorte){
    free(resorte);
}