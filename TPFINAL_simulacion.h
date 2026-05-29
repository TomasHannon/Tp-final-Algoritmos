#ifndef SIMULACION_H
#define SIMULACION_H

#include <stdbool.h>
#include <stddef.h>

typedef struct malla_simulacion malla_simulacion_t;

typedef struct simulacion simulacion_t;

typedef struct iter_resorte mallas_s_iter_resorte_t;

typedef struct iter_masa mallas_s_iter_masa_t;

/*
 * Crea una nueva simulación.
 * Precondición: La malla de simulación ms no es NULL.
 * Postcondición: Devuelve un puntero a una nueva simulación.
 */
simulacion_t *simulacion_crear(malla_simulacion_t *ms, float g, float b, float dt);

/*
 * Crea una nueva malla de simulación a partir de una malla.
 * Precondición: La malla no es NULL.
 * Postcondición: Devuelve un puntero a una nueva malla de simulación.
 */
malla_simulacion_t *malla_simulacion_crear(malla_t *malla);

/*
 * Destruye una malla de simulación.
 * Precondición: La malla de simulación ms no es NULL.
 * Postcondición: La malla de simulación ms y todos sus recursos asociados son liberados de la memoria.
 */
void malla_simulacion_destruir(malla_simulacion_t *ms);

/*
 * Destruye una simulación.
 * Precondición: La simulación s no es NULL.
 * Postcondición: La simulación s y todos sus recursos asociados son liberados de la memoria.
 */
void simulacion_destruir(simulacion_t *s);

/*
 * Realiza la simulación de la malla de simulación un número determinado de veces.
 * Precondición: La simulación simulacion no es NULL.
 * Postcondición: Devuelve true si la simulación fue exitosa, false en caso contrario.
 */
bool simular(simulacion_t *simulacion, size_t veces);

/*
 * Crea un iterador de masas para recorrer las masas de la malla de simulación.
 * Precondición: La malla de simulación malla no es NULL.
 * Postcondición: Devuelve un puntero a un nuevo iterador de masas.
 */
mallas_s_iter_masa_t *mallas_s_iter_masa_crear(malla_simulacion_t *malla);

/*
 * Avanza el iterador de masas a la siguiente masa.
 * Precondición: El iterador de masas mm no es NULL.
 * Postcondición: El iterador de masas mm avanza a la siguiente masa de la malla de simulación.
 */
void mallas_s_iter_masa_avanzar(mallas_s_iter_masa_t *mm);

/*
 * Verifica si el iterador de masas ha llegado al final de la malla de simulación.
 * Precondición: El iterador de masas mm no es NULL.
 * Postcondición: Devuelve true si el iterador de masas ha llegado al final de la malla de simulación, false en caso contrario.
 */
bool mallas_s_iter_masa_al_final(mallas_s_iter_masa_t *mm);

/*
 * Devuelve la masa actual apuntada por el iterador de masas.
 * Precondición: El iterador de masas mm no es NULL.
 * Postcondición: Devuelve un puntero a la masa actual apuntada por el iterador de masas.
 */
masa_t *mallas_s_iter_ver_masa_actual(mallas_s_iter_masa_t *mm);

/*
 * Destruye el iterador de masas.
 * Precondición: El iterador de masas mm no es NULL.
 * Postcondición: El iterador de masas mm y todos sus recursos asociados son liberados de la memoria.
 */
void mallas_s_iter_masa_destruir(mallas_s_iter_masa_t *mm);

/*
 * Crea un iterador de resortes para recorrer los resortes de la malla de simulación.
 * Precondición: La malla de simulación malla no es NULL.
 * Postcondición: Devuelve un puntero a un nuevo iterador de resortes.
 */
mallas_s_iter_resorte_t *mallas_s_iter_resorte_crear(malla_simulacion_t *malla);

/*
 * Avanza el iterador de resortes al siguiente resorte.
 * Precondición: El iterador de resortes mm no es NULL.
 * Postcondición: El iterador de resortes mm avanza al siguiente resorte de la malla de simulación.
 */
void mallas_s_iter_resorte_avanzar(mallas_s_iter_resorte_t *mm);

/*
 * Verifica si el iterador de resortes ha llegado al final de la malla de simulación.
 * Precondición: El iterador de resortes mm no es NULL.
 * Postcondición: Devuelve true si el iterador de resortes ha llegado al final de la malla de simulación, false en caso contrario.
 */
bool mallas_s_iter_resorte_al_final(mallas_s_iter_resorte_t *mm);

/*
 * Devuelve el resorte actual apuntado por el iterador de resortes.
 * Precondición: El iterador de resortes mm no es NULL.
 * Postcondición: Devuelve un puntero al resorte actual apuntado por el iterador de resortes.
 */
resorte_t *mallas_s_iter_ver_resorte_actual(mallas_s_iter_resorte_t *mm);

/*
 * Destruye el iterador de resortes.
 * Precondición: El iterador de resortes mm no es NULL.
 * Postcondición: El iterador de resortes mm y todos sus recursos asociados son liberados de la memoria.
 */
void mallas_s_iter_resorte_destruir(mallas_s_iter_resorte_t *mm);


#endif
