#ifndef RESORTE_H
#define RESORTE_H


typedef struct resorte resorte_t;

/* Primitivas de los resortes */

// Obtiene las coordenadas de los extremos del resorte 'resorte'.
// Precondición: 'resorte' es un puntero válido a un resorte creado previamente.x1, y1, x2 y y2 deben ser punteros no nulos.
// Postcondición: Las variables 'x1', 'y1', 'x2' y 'y2' contienen las coordenadas de los extremos del resorte.
void resorte_obtener_coords(double *x1, double *y1, double *x2, double *y2, resorte_t *resorte);

// Obtiene la longitud de equilibrio del resorte 'resorte'.
// Precondición: 'resorte' es un puntero válido a un resorte creado previamente.
// Postcondición: Devuelve la longitud del resorte.
double resorte_obtener_lo(resorte_t *resorte);

// Cambia la longitud de equilibrio del resorte 'resorte' por 'lo'.
// Precondición: 'resorte' es un puntero válido a un resorte creado previamente.
// Postcondición: La longitud del resorte se actualiza con el valor proporcionado.
void resorte_cambiar_lo(resorte_t *resorte, double lo);

// Obtiene la constante elástica del resorte 'resorte'.
// Precondición: 'resorte' es un puntero válido a un resorte creado previamente.
// Postcondición: Devuelve la constante elástica del resorte.
double resorte_obtener_k(resorte_t *resorte);

// Verifica si el resorte 'resorte' está vinculado a la masa 'masa'.
// Precondición: 'resorte' y 'masa' son punteros válidos a resortes y masas creadas previamente.
// Postcondición: Devuelve 'true' si el resorte está vinculado a la masa, 'false' en caso contrario.
bool resorte_vinculado_masa(resorte_t *resorte, masa_t *masa);

// Actualiza el resorte 'resorte' según los parámetros 'lo_max' y 'k'.
// Precondición: 'resorte' es un puntero válido a un resorte creado previamente.
// Postcondición: - Si se proporciona un lo_max positivo y la longitud de reposo del resorte supera lo_max, las propiedades del resorte no se actualizan y devuelve false.
//                - En caso contrario, la longitud de reposo y la constante de elasticidad del resorte se actualizan con los nuevos valores proporcionados. Devuelve true.
bool resorte_actualizar(resorte_t *resorte, double lo_max, double k);



// Obtiene las masas vinculadas al resorte 'resorte'.
// Precondición: 'resorte' es un puntero válido a un resorte creado previamente.
// Postcondición: Los punteros ma y mb se actualizan con las masas vinculadas por el resorte.
void resorte_obtener_masas_vinculadas(resorte_t *resorte, masa_t **ma, masa_t **mb);


// Crea un resorte que vincula las masas 'ma' y 'mb', con una longitud de equilibrio máxima 'lo_max' y una constante elástica 'k'.
// Precondición: 'ma' y 'mb' son punteros válidos a masas creadas previamente.
// Postcondición: Devuelve un puntero al resorte creado si no hay errores de memoria. Si no se puede asignar memoria para el resorte, devuelve NULL.      
resorte_t *resorte_crear(masa_t *ma, masa_t *mb, double lo_max, double k);


// Crea una copia del resorte 'resorte'.
// Precondición: 'resorte' es un puntero válido a un resorte creado previamente.
// Postcondición:   Devuelve un puntero al resorte clonado si no hay errores de memoria. Si no se puede asignar memoria para el resorte clonado, devuelve NULL.
resorte_t *resorte_clonar(resorte_t *resorte);


// Destruye el resorte 'resorte'.
// Precondición: 'resorte' es un puntero válido a un resorte creado previamente.
// Postcondición: La memoria ocupada por el resorte se libera.
void resorte_destruir(resorte_t *resorte);


#endif