#include <stdbool.h>
#include <stdlib.h>

#include "matrices.h"
#include "mallas.h"
#include "masa.h"
#include "resorte.h"
#include "distancia.h"
#include "config.h"

#include "simulacion.h"

#include <stdio.h>

struct malla_simulacion{
    masa_t **masas;
    size_t n_masas;
    //resorte_t **resortes;
    matriz_t *posiciones;
    matriz_t *datos_resortes;
    size_t n_resortes;
};

struct simulacion{
    malla_simulacion_t *malla_s;
    matriz_t *posiciones;
    matriz_t *posicionesm1;
    matriz_t *posicionesm2;
    float g;
    float b;
    float dt;
};

struct iter_resorte{
    size_t i;
    malla_simulacion_t *malla;
    resorte_t **resortes;
};

struct iter_masa{
    size_t i;
    malla_simulacion_t *malla;
};

static bool calcular_posiciones(simulacion_t *simulacion);

void simulacion_destruir(simulacion_t *s){
    matriz_destruir(s->posicionesm2);
    free(s);
}

simulacion_t *simulacion_crear(malla_simulacion_t *ms, float g, float b, float dt){
    simulacion_t *s = malloc(sizeof(simulacion_t));
    if(s == NULL){
        return NULL;
    }
    s->malla_s = ms;
    s->posiciones = ms->posiciones;
    s->posicionesm1 = ms->posiciones;
    s->posicionesm2 = matriz_clonar(s->malla_s->posiciones);
    if(s->posicionesm2 == NULL){
        simulacion_destruir(s);
        return NULL;
    }
    s->g = g;
    s->b = b;
    s->dt = dt;
    return s;
}
void malla_simulacion_destruir(malla_simulacion_t *ms){
    matriz_destruir(ms->posiciones);
    for(size_t i = 0; i < ms->n_masas; i++){
        masa_destruir(ms->masas[i]);
    }
    free(ms->masas);
    matriz_destruir(ms->datos_resortes);
    free(ms);
}

malla_simulacion_t *malla_simulacion_crear(malla_t *malla){
    malla_simulacion_t *ms = malloc(sizeof(malla_simulacion_t));
    if(ms == NULL) return NULL;
    ms->n_masas = malla_n_masas(malla);
    ms->n_resortes = malla_n_resortes(malla);
    ms->masas = malloc(ms->n_masas * sizeof(masa_t*));
    if(ms->masas == NULL){
        ms->n_masas = 0;
        malla_simulacion_destruir(ms);
        return NULL;
    }
    ms->posiciones = matriz_ceros(ms->n_masas, 2);//se pierde la memoria
    if(ms->posiciones == NULL){
        malla_simulacion_destruir(ms);
        return NULL;
    }
    ms->datos_resortes = matriz_ceros(ms->n_resortes, 4);// datos_resortes[k][lo][ida][idb]
    if(ms->datos_resortes == NULL){
        malla_simulacion_destruir(ms);
        return NULL;
    }
    mallas_iter_masa_t *mm;
    size_t i_m = 0;
    for(mm = mallas_iter_masa_crear(malla); mm != NULL && !mallas_iter_masa_al_final(mm); mallas_iter_masa_avanzar(mm)){
        ms->masas[i_m] = masa_clonar(mallas_iter_ver_masa_actual(mm));
        double x, y;
        masa_obtener_coords(&x, &y, ms->masas[i_m]);
        matriz_sumar_a_posicion(ms->posiciones, i_m, 0, x);
        matriz_sumar_a_posicion(ms->posiciones, i_m, 1, y);
        if(ms->masas[i_m++] == NULL){
            mallas_iter_masa_destruir(mm);
            malla_simulacion_destruir(ms);
            return NULL;
        }
    }
    if(mm == NULL){
        malla_simulacion_destruir(ms);
        return NULL;
    }
    mallas_iter_masa_destruir(mm);
    mallas_iter_resorte_t *mi;
    size_t i_r = 0;
    for(mi = mallas_iter_resorte_crear(malla); mi != NULL && !mallas_iter_resorte_al_final(mi); mallas_iter_resorte_avanzar(mi)){
        resorte_t *resorte = mallas_iter_ver_resorte_actual(mi);
        matriz_sumar_a_posicion(ms->datos_resortes, i_r, 0, resorte_obtener_k(resorte));
        matriz_sumar_a_posicion(ms->datos_resortes, i_r, 1, resorte_obtener_lo(resorte));
        masa_t *ma = NULL;
        masa_t *mb = NULL;
        resorte_obtener_masas_vinculadas(resorte, &ma, &mb);
        matriz_sumar_a_posicion(ms->datos_resortes, i_r, 2, masa_obtener_id(ma));
        matriz_sumar_a_posicion(ms->datos_resortes, i_r, 3, masa_obtener_id(mb));
        i_r++;
    }
    if(mi == NULL){
        malla_simulacion_destruir(ms);
        return NULL;
    }
    mallas_iter_resorte_destruir(mi);
    //matriz_imprimir(ms->datos_resortes);
    return ms;
}

static resorte_t *resorte_simulando(size_t n, malla_simulacion_t *malla_s){
    double k = matriz_obtener(malla_s->datos_resortes, n, 0);
    double lo = matriz_obtener(malla_s->datos_resortes, n, 1);
    size_t ida = matriz_obtener(malla_s->datos_resortes, n, 2);
    size_t idb = matriz_obtener(malla_s->datos_resortes, n, 3);
    masa_t *ma = malla_s->masas[ida];
    masa_t *mb = malla_s->masas[idb];
    resorte_t *r = resorte_crear(ma, mb, 0, k);
    resorte_cambiar_lo(r, lo);
    return r;
}

bool simular(simulacion_t *simulacion, size_t veces){
    for(size_t i = 0; i < veces; i++){
        if(!calcular_posiciones(simulacion)){
            return false;
        }
    }
    for(size_t i = 0; i < simulacion->malla_s->n_masas; i++){
        double x = matriz_obtener(simulacion->posiciones, i, 0);
        double y = matriz_obtener(simulacion->posiciones, i, 1);
        //matriz_imprimir(simulacion->posiciones);
        masa_cambiar_coords(simulacion->malla_s->masas[i], x, y);
    }
    return true;
}
static bool calcular_posiciones(simulacion_t *simulacion){
    matriz_t *abx = matriz_ceros(simulacion->malla_s->n_masas, 2);
    matriz_t *aby = matriz_ceros(simulacion->malla_s->n_masas, 2);
    if(abx == NULL || aby == NULL){
        matriz_destruir(abx);
        matriz_destruir(aby);
        return false;
    }
    //calculo de A y B en masas
    for(size_t i = 0; i < simulacion->malla_s->n_masas; i++){
        double mi = masa_obtener_masa(simulacion->malla_s->masas[i]);
        double ai = (mi/((simulacion->dt)*(simulacion->dt))) + (simulacion->b)/(simulacion->dt);
        matriz_sumar_a_posicion(abx, i, 0, ai);
        matriz_sumar_a_posicion(aby, i, 0, ai);

        double xm1 = matriz_obtener(simulacion->posicionesm1, i, 0);
        double xm2 = matriz_obtener(simulacion->posicionesm2, i, 0);
        double ym1 = matriz_obtener(simulacion->posicionesm1, i, 1);
        double ym2 = matriz_obtener(simulacion->posicionesm2, i, 1);

        double bi_m_x = (mi/((simulacion->dt)*(simulacion->dt)))*((2*xm1) - xm2) + (simulacion->b)/(simulacion->dt) * xm1;
        matriz_sumar_a_posicion(abx, i, 1, bi_m_x);

        double bi_m_y = (mi/((simulacion->dt)*(simulacion->dt)))*((2*ym1) - ym2) + ((simulacion->b)/(simulacion->dt)) * ym1;
        if(!masa_es_fijo(simulacion->malla_s->masas[i])){
            bi_m_y += mi * simulacion->g;
        }
        matriz_sumar_a_posicion(aby, i, 1, bi_m_y);
    }
    //calculo de B por resortes
    for(size_t i = 0; i < simulacion->malla_s->n_resortes; i++){
        double k = matriz_obtener(simulacion->malla_s->datos_resortes, i, 0);
        double lo = matriz_obtener(simulacion->malla_s->datos_resortes, i, 1);
        size_t ida = matriz_obtener(simulacion->malla_s->datos_resortes, i, 2);
        size_t idb = matriz_obtener(simulacion->malla_s->datos_resortes, i, 3);
        //printf("%zd %zd\n", ida, idb);
        //matriz_imprimir(simulacion->posicionesm1);
        double xa = matriz_obtener(simulacion->posicionesm1, ida, 0);
        double ya = matriz_obtener(simulacion->posicionesm1, ida, 1);
        double xb = matriz_obtener(simulacion->posicionesm1, idb, 0);
        double yb = matriz_obtener(simulacion->posicionesm1, idb, 1);

        double lrm1 = distancia_xy(xa, ya, xb, yb); 
        double bida_r_x = k * ((lo - lrm1)/ lrm1) * (xa - xb);
        double bida_r_y = k * ((lo - lrm1)/ lrm1) * (ya - yb);
        if(!masa_es_fijo(simulacion->malla_s->masas[ida])){
            matriz_sumar_a_posicion(abx, ida, 1, bida_r_x);
            matriz_sumar_a_posicion(aby, ida, 1, bida_r_y);
        }
        if(!masa_es_fijo(simulacion->malla_s->masas[idb])){
            matriz_sumar_a_posicion(abx, idb, 1, bida_r_x * (-1));
            matriz_sumar_a_posicion(aby, idb, 1, bida_r_y * (-1));
        }
    }
    matriz_t *aux = simulacion->posicionesm2;
    //matriz_destruir(simulacion->posiciones);
    simulacion->posiciones = matriz_ceros(simulacion->malla_s->n_masas, 2);
    if(simulacion->posiciones == NULL){
        matriz_destruir(abx);
        matriz_destruir(aby);
    }
    for(size_t i = 0; i < simulacion->malla_s->n_masas; i++){
        double ax = matriz_obtener(abx, i, 0);
        double bx = matriz_obtener(abx, i, 1);
        double ay = matriz_obtener(aby, i, 0);
        double by = matriz_obtener(aby, i, 1);

        matriz_sumar_a_posicion(simulacion->posiciones, i, 0, bx/ax);
        matriz_sumar_a_posicion(simulacion->posiciones, i, 1, by/ay);
    }

    simulacion->posicionesm2 = simulacion->posicionesm1;
    simulacion->posicionesm1 = simulacion->posiciones;
    simulacion->malla_s->posiciones = simulacion->posiciones;


    matriz_destruir(aux);


    matriz_destruir(abx);
    matriz_destruir(aby);

    return true;
}

mallas_s_iter_masa_t *mallas_s_iter_masa_crear(malla_simulacion_t *malla){
    mallas_s_iter_masa_t *mm = malloc(sizeof(mallas_s_iter_masa_t));
    if(mm == NULL){
        return NULL;
    }
    mm->i = 0;
    mm->malla = malla;
    return mm;
}

void mallas_s_iter_masa_avanzar(mallas_s_iter_masa_t *mm){
    mm->i++;
}

bool mallas_s_iter_masa_al_final(mallas_s_iter_masa_t *mm){
    return mm->i >= mm->malla->n_masas;
}

masa_t *mallas_s_iter_ver_masa_actual(mallas_s_iter_masa_t *mm){
    return mm->malla->masas[mm->i];
}

void mallas_s_iter_masa_destruir(mallas_s_iter_masa_t *mm){
    free(mm);
}

mallas_s_iter_resorte_t *mallas_s_iter_resorte_crear(malla_simulacion_t *malla){
    mallas_s_iter_resorte_t *mm = malloc(sizeof(mallas_s_iter_resorte_t));
    if(mm == NULL){
        return NULL;
    }
    mm->i = 0;
    mm->malla = malla;
    mm->resortes = malloc(mm->malla->n_resortes * sizeof(resorte_t*));
    if(mm->resortes == NULL){
        free(mm);
    }
    for(size_t i = 0; i < mm->malla->n_resortes; i++){
        mm->resortes[i] = resorte_simulando(i, mm->malla);
    }
    return mm;
}

void mallas_s_iter_resorte_avanzar(mallas_s_iter_resorte_t *mm){  
    mm->i++;
}

bool mallas_s_iter_resorte_al_final(mallas_s_iter_resorte_t *mm){
    return mm->i >= mm->malla->n_resortes;
}
resorte_t *mallas_s_iter_ver_resorte_actual(mallas_s_iter_resorte_t *mm){
    return mm->resortes[mm->i];
}

void mallas_s_iter_resorte_destruir(mallas_s_iter_resorte_t *mm){
    for(size_t i = 0; i < mm->malla->n_resortes; i++){
        resorte_destruir(mm->resortes[i]);
    }
    free(mm->resortes);
    free(mm);
}

