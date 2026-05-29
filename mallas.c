#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>


#include "mallas.h"
#include "masa.h"
#include "resorte.h"
#include "distancia.h"
#include "escrituraylectura.h"

struct malla{
    masa_t **masas;
    size_t n_masas;
    size_t pedidos;
    lista_t *resortes;
    size_t n_resortes;
};

struct iter_resorte{
    lista_iter_t *li;
};

struct iter_masa{
    size_t i;
    malla_t *malla;
};

static void resorte_destruir_void(void *resorte){
    resorte_destruir(resorte);
}

void malla_destruir(malla_t *malla){
    lista_destruir(malla->resortes, resorte_destruir_void);
    for(size_t i = 0; i < malla->n_masas; i++){
        masa_destruir(malla->masas[i]);
    }
    free(malla->masas);
    free(malla);
}

malla_t *malla_crear(void){
    malla_t *malla = malloc(sizeof(malla_t));
    if(malla == NULL)
        return NULL;

    malla->masas = malloc(10 * sizeof(masa_t*));
    if(malla->masas == NULL){
        malla_destruir(malla);
        return NULL;
    }
    lista_t *l = lista_crear();
    if(l == NULL){
        malla_destruir(malla);
        return NULL;
    }
    malla->pedidos = 10;
    malla->n_masas = 0;
    malla->resortes = l;
    malla->n_resortes = 0;
    return malla;
}

bool malla_agregar_masa(malla_t *malla, masa_t *masa){
    if(malla->pedidos <= malla->n_masas){
        masa_t **aux = realloc(malla->masas, malla->pedidos * 2 * sizeof(masa_t*));
        if(aux == NULL){
            malla_destruir(malla);
            return false;
        }
        malla->masas = aux;
        malla->pedidos *= 2;
    }
    malla->masas[malla->n_masas++] = masa;
    return true;
}

void malla_sacar_masa(malla_t *malla, masa_t *masa){
    size_t id1 = masa_obtener_id(masa);
    masa_cambiar_id(malla->masas[malla->n_masas - 1], id1);
    masa_destruir(masa);
    malla->masas[id1] = malla->masas[--(malla->n_masas)];
}

bool malla_agregar_resorte(malla_t *malla, resorte_t *resorte){
    malla->n_resortes++;
    return lista_insertar_primero(malla->resortes, resorte);
}

bool malla_sacar_resorte(malla_t *malla, resorte_t *resorte){
    lista_iter_t *li;
    for(li = lista_iter_crear(malla->resortes); !lista_iter_al_final(li); lista_iter_avanzar(li)){
        resorte_t *res = lista_iter_ver_actual(li);
        if(resorte == res){
            lista_iter_borrar(li);
            lista_iter_destruir(li);
            malla->n_resortes--;
            resorte_destruir_void(resorte);
            return true;
        }
    }
    return false;
}

masa_t *masa_buscar(malla_t *malla, double xd, double yd, double rango){
    for(size_t i = 0;i < malla->n_masas; i++){
        double x, y;
        masa_obtener_coords(&x, &y, malla->masas[i]);
        double d = distancia_xy(xd, yd, x, y);
        if(d < rango){
            return malla->masas[i];
        }
    }
    return NULL;
}

bool resorte_buscar_eliminar(malla_t *malla, double xd, double yd, double rango){
    bool resorte_eliminado = false;
    lista_iter_t *li = lista_iter_crear(malla->resortes);  
    while(!lista_iter_al_final(li)){
        resorte_t *resorte = lista_iter_ver_actual(li);
        double x1, y1, x2, y2;
        resorte_obtener_coords(&x1, &y1, &x2, &y2, resorte);
        double distancia = distancia_p_segmento(xd, yd, x1, y1, x2, y2);
        if(distancia < rango){
            lista_iter_borrar(li);
            malla->n_resortes--;
            resorte_destruir_void(resorte);
            resorte_eliminado = true;
        } else{
            lista_iter_avanzar(li);
        }
    }
    lista_iter_destruir(li);
    return resorte_eliminado;
}

bool resorte_buscar_limitar(malla_t *malla, double xd, double yd, double rango){
    lista_iter_t *li = lista_iter_crear(malla->resortes);
    while(!lista_iter_al_final(li)){
        resorte_t *resorte = lista_iter_ver_actual(li);
        double lo = resorte_obtener_lo(resorte);
        if(lo >= rango){
            return false;
        } else{
            lista_iter_avanzar(li);
        }
    }
    lista_iter_destruir(li);
    return true;
}
size_t malla_n_masas(malla_t *malla){
    return malla->n_masas;
}

size_t malla_n_resortes(malla_t *malla){
    return malla->n_resortes;
}

mallas_iter_masa_t *mallas_iter_masa_crear(malla_t *malla){
    mallas_iter_masa_t *mm = malloc(sizeof(mallas_iter_masa_t));
    if(mm == NULL){
        return NULL;
    }
    mm->i = 0;
    mm->malla = malla;
    return mm;
}

void mallas_iter_masa_avanzar(mallas_iter_masa_t *mm){
    mm->i++;
}

bool mallas_iter_masa_al_final(mallas_iter_masa_t *mm){
    return mm->i >= mm->malla->n_masas;
}

masa_t *mallas_iter_ver_masa_actual(mallas_iter_masa_t *mm){
    return mm->malla->masas[mm->i];
}

void mallas_iter_masa_destruir(mallas_iter_masa_t *mm){
    free(mm);
}

mallas_iter_resorte_t *mallas_iter_resorte_crear(malla_t *malla){
    mallas_iter_resorte_t *mi = malloc(sizeof(mallas_iter_resorte_t));
    if(mi == NULL){
        return NULL;
    }
    mi->li = lista_iter_crear(malla->resortes);
    if(mi->li == NULL){
        mallas_iter_resorte_destruir(mi);
        return NULL;
    }
    return mi;
}

bool mallas_iter_resorte_al_final(mallas_iter_resorte_t *mi) {
    return lista_iter_al_final(mi->li);
}


void mallas_iter_resorte_avanzar(mallas_iter_resorte_t *mi){
    lista_iter_avanzar(mi->li);
}

resorte_t *mallas_iter_ver_resorte_actual(mallas_iter_resorte_t *mi){
    return lista_iter_ver_actual(mi->li);
}

void mallas_iter_resorte_destruir(mallas_iter_resorte_t *mi){
    lista_iter_destruir(mi->li);
    free(mi);
}

void masa_resortes_vinculados_destruir(resorte_t **resortes){
    free(resortes);
}

resorte_t **masa_resortes_vinculados(malla_t *malla, masa_t *masa, size_t *n_vinculados){
    resorte_t **resortes = malloc(10 * sizeof(resorte_t*));
    if(resortes == NULL) return NULL;
    size_t pedidos = 10;
    *n_vinculados = 0;

    // Iterar sobre los resortes de la malla
    mallas_iter_resorte_t* mi;
    for (mi = mallas_iter_resorte_crear(malla); mi != NULL && !mallas_iter_resorte_al_final(mi); mallas_iter_resorte_avanzar(mi)) {
        resorte_t* resorte = mallas_iter_ver_resorte_actual(mi);

        // Verificar si el resorte está vinculado a la masa
        if (resorte_vinculado_masa(resorte, masa)) {
            // Agregar el resorte al arreglo
            if ((*n_vinculados) >= pedidos) {
                resorte_t** aux = realloc(resortes, pedidos * 2 * sizeof(resorte_t*));
                if (aux == NULL) {
                    // Error de memoria, liberar recursos y retornar NULL
                    masa_resortes_vinculados_destruir(resortes);
                    mallas_iter_resorte_destruir(mi);
                    *n_vinculados = 0;
                    return NULL;
                }
                resortes = aux;
                pedidos *= 2;
            }
            resortes[(*n_vinculados)++] = resorte;
        }
    }
    if(mi == NULL){
        masa_resortes_vinculados_destruir(resortes);
        return NULL;
    }
    mallas_iter_resorte_destruir(mi);
    return resortes;
}

bool malla_masa_actualizar_resortes_vinculados(malla_t *malla, masa_t *masa, double rango, double x_ant, double y_ant){
    size_t n_r_v;
    resorte_t **resortes_vinculados = masa_resortes_vinculados(malla, masa, &n_r_v);
    for(size_t i = 0; i < n_r_v; i++){
        if(!resorte_actualizar(resortes_vinculados[i], rango, 0)){
            masa_cambiar_coords(masa, x_ant, y_ant);
            for(size_t i = 0; i < n_r_v; i++){
                resorte_actualizar(resortes_vinculados[i], rango, 0);
            }
            masa_resortes_vinculados_destruir(resortes_vinculados);
            return false;
        }
    }
    masa_resortes_vinculados_destruir(resortes_vinculados);
    return true;
}

void malla_cambiar_masas_m_all(malla_t *malla, double masa){
    for(size_t i = 0; i < malla->n_masas; i++){
        masa_cambiar_masa(malla->masas[i], masa);
    }
}

bool malla_cambiar_resortes_k_lo_all(malla_t *malla, double lo_max, double k_base, double potencia_k){
    mallas_iter_resorte_t *mi;
    for(mi = mallas_iter_resorte_crear(malla); mi != NULL && !mallas_iter_resorte_al_final(mi); mallas_iter_resorte_avanzar(mi)){
    resorte_t *resorte = mallas_iter_ver_resorte_actual(mi);
    double lo = resorte_obtener_lo(resorte);
    if(!resorte_actualizar(resorte, lo_max, k_base/pow(lo, potencia_k))){
        mallas_iter_resorte_destruir(mi);
        return false;
    }
    }
    if(mi == NULL){
        return false;
    }
    mallas_iter_resorte_destruir(mi);
    return true;
}

bool escribir_malla(malla_t *malla, FILE *salida, size_t nivel){
    size_t n_m = malla->n_masas;
    if(!escribir_numero_de_datos(salida, n_m)) return false;
    for(size_t i = 0; i < n_m; i++){
        double x, y;
        masa_t *masa = malla->masas[i];
        size_t id = masa_obtener_id(masa);
        bool es_fijo = masa_es_fijo(masa);
        double m = masa_obtener_masa(masa);
        masa_obtener_coords(&x, &y, masa);
        if(!escribir_masa(salida, x, y, m, es_fijo, id)) return false;
    }
    size_t n_r = malla->n_resortes;
    if(!escribir_numero_de_datos(salida, n_r)) return false;
    mallas_iter_resorte_t *mi;
    for(mi = mallas_iter_resorte_crear(malla); mi != NULL && !mallas_iter_resorte_al_final(mi); mallas_iter_resorte_avanzar(mi)){
        resorte_t *resorte = mallas_iter_ver_resorte_actual(mi);
        masa_t *ma = NULL;
        masa_t *mb = NULL;
        resorte_obtener_masas_vinculadas(resorte, &ma, &mb);
        size_t ida = masa_obtener_id(ma);
        size_t idb = masa_obtener_id(mb);
        double lo = resorte_obtener_lo(resorte);
        double k = resorte_obtener_k(resorte);
        if(!escribir_resorte(salida, ida, idb, k, lo)) return false;
    }
    if(mi == NULL) return false;
    mallas_iter_resorte_destruir(mi);
    if(!escribir_numero_de_datos(salida, nivel)) return false;
    return true; 
}

malla_t *leer_malla(FILE* entrada) {
    malla_t *malla = malla_crear();
    if(malla == NULL){
        return NULL;
    }
    size_t n_m;
    if (!leer_numero_de_datos(entrada, &n_m)) {
        malla_destruir(malla);
        return NULL;
    }
    for(size_t i = 0; i < n_m; i++) {
        double x, y, m;
        bool es_fijo;
        size_t id;
        if (!leer_masa(entrada, &x, &y, &m, &es_fijo, &id)) {
            malla_destruir(malla);
            return NULL;
        }
        masa_t *masa = masa_crear(x, y, es_fijo, id, m);
        if(masa == NULL){
            malla_destruir(malla);
            return NULL;
        }
        if(!malla_agregar_masa(malla, masa)){
            malla_destruir(malla);
            return NULL;
        }
    }
    size_t n_r;
    if(!leer_numero_de_datos(entrada, &n_r)){
        malla_destruir(malla);
        return NULL;
    }
    for(size_t i = 0; i < n_r; i++){
        size_t ida, idb;
        double k, lo;
        if(!leer_resorte(entrada, &ida, &idb, &k, &lo)){
            malla_destruir(malla);
            return NULL;
        }
        masa_t *ma = malla_obtener_masa_por_id(malla, ida);
        if(ma == NULL){
            malla_destruir(malla);
            return NULL;
        }
        masa_t *mb = malla_obtener_masa_por_id(malla, idb);
        if(mb == NULL){
            malla_destruir(malla);
            return NULL;
        }
        resorte_t *resorte = resorte_crear(ma, mb, -1, k);

        if(resorte == NULL){
            malla_destruir(malla);
            return NULL;
        }
        if(!malla_agregar_resorte(malla, resorte)){
            malla_destruir(malla);
            return NULL;
        }
    }
    return malla;
}

masa_t* malla_obtener_masa_por_id(malla_t* malla, size_t id){
    if(malla == NULL || malla->n_masas <= id){
        return NULL;
    }
    return malla->masas[id];
}


malla_t *crear_nivel(size_t nivel, size_t *id, double escala){
    masa_t *masab = masa_crear(40/escala, 130/escala, true, (*id)++, 0);
    masa_t *masad = masa_crear((240 + (nivel * 50))/escala, 130/escala, true, (*id)++, 0);
    if(masab == NULL|| masad == NULL){
        return NULL;
    }
    malla_t *malla = malla_crear();
    if(malla == NULL){
        masa_destruir(masab);
        masa_destruir(masad);
        return NULL;
    }
    if(!malla_agregar_masa(malla, masab)|| !malla_agregar_masa(malla, masad)){
        malla_destruir(malla);
        return NULL;
    }
    return malla;
}
