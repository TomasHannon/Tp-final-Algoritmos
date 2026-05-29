#ifndef MASA_H
#define MASA_H

#include <stdbool.h>

typedef struct masa masa_t;


/*primitivas de las masas */

// crea una masa
// Precondiciones: Los valores proporcionados son válidos
// Crea una masa con las coordenadas 'x' e 'y', indicando si es fija ('es_fijo'),
// un identificador 'id' y una masa 'm'.
// Postcondición: Se crea y devuelve un puntero a una nueva masa con los valores proporcionados
masa_t *masa_crear(double x, double y, bool es_fijo, size_t id, double m);


// Crea una copia de la masa 'masa'.
// Precondición: 'masa' es un puntero válido a una masa creada previamente.
// Postcondición: Se crea y devuelve un puntero a una nueva masa con los mismos valores que la masa proporcionada
masa_t *masa_clonar(masa_t *masa);


// Obtiene el identificador de la masa 'masa'.
// Precondiciones: masa es un puntero válido a una masa existente
// Postcondiciones: Se devuelve el identificador (id) de la masa
size_t masa_obtener_id(masa_t *masa);

// Cambia el identificador de la masa 'masa' por 'id'.
// Precondiciones: masa es un puntero válido a una masa existente
// Postcondiciones: El identificador (id) de la masa se actualiza con el valor proporcionado
void masa_cambiar_id(masa_t *masa, size_t id);


// Cambia la masa de la masa 'masa' por 'm'.
// Precondiciones: masa es un puntero válido a una masa existente
// Postcondiciones: La masa se actualiza con el valor de masa proporcionado
void masa_cambiar_masa(masa_t *masa, double m);

// Obtiene la masa de la masa 'masa'.
// Precondición: 'masa' es un puntero válido a una masa creada previamente.
// Postcondición: Devuelve la masa de la masa.
double masa_obtener_masa(masa_t *masa);


// Verifica si la masa 'masa' es fija.
// Precondición: 'masa' es un puntero válido a una masa creada previamente.
// Postcondición: Devuelve 'true' si la masa es fija, 'false' en caso contrario.
bool masa_es_fijo(masa_t *masa);

// Obtiene las coordenadas 'x' e 'y' de la masa 'masa'.
// Precondición: 'masa' es un puntero válido a una masa creada previamente.
// Postcondición: Las variables 'x' e 'y' contienen las coordenadas de la masa.
void masa_obtener_coords(double *x, double *y, masa_t *masa);

// Cambia las coordenadas de la masa 'masa' por 'x' e 'y'.
// Precondición: 'masa' es un puntero válido a una masa creada previamente.
// Postcondiciones: Las coordenadas de la masa se actualizan con los valores de x e y
void masa_cambiar_coords(masa_t *masa, double x, double y);

// Destruye la masa 'masa'.
// Precondición: 'masa' es un puntero válido a una masa creada previamente.
// Postcondición: La memoria ocupada por la masa se libera.
void masa_destruir(masa_t *masa);

#endif

