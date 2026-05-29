#ifndef MALLAS_H
#define MALLAS_H

#include "masa.h"
#include "resorte.h"
#include "lista.h"

#include <stdbool.h>
#include <stdio.h>

typedef struct malla malla_t;

typedef struct iter_resorte mallas_iter_resorte_t;

typedef struct iter_masa mallas_iter_masa_t;


// Precondiciones: malla es un puntero válido a una malla creada correctamente
// Postcondiciones: se crea un nuevo iterador mallas_iter_masa_t,
//                  se inicializa su posición en cero y se asigna la malla proporcionada,
//                  se devuelve un puntero al iterador creado o NULL en caso de error
mallas_iter_masa_t *mallas_iter_masa_crear(malla_t *malla);

// Avanza el iterador a la siguiente masa en la malla
// Precondiciones: mm es un puntero válido a un iterador creado correctamente
// Postcondiciones: se incrementa el índice del iterador en 1,
//                  avanzando a la siguiente masa en la malla
void mallas_iter_masa_avanzar(mallas_iter_masa_t *mm);

// Verifica si el iterador ha alcanzado el final de la malla
// Precondiciones: mm es un puntero válido a un iterador creado correctamente
// Postcondiciones: se retorna un valor booleano indicando si el iterador ha llegado
//                  al final de la malla (true) o no (false)
bool mallas_iter_masa_al_final(mallas_iter_masa_t *mm);

// Libera la memoria asignada al iterador de masas
// Precondiciones: mm es un puntero válido a un iterador creado correctamente
// Postcondiciones: la memoria asignada al iterador mm ha sido liberada
void mallas_iter_masa_destruir(mallas_iter_masa_t *mm);

// Obtiene la masa actual apuntada por el iterador
// Precondiciones: mm es un puntero válido a un iterador creado correctamente
// Postcondiciones: se retorna un puntero a la masa actual apuntada por el iterador
//                  o NULL si no se cumplieron las precondiciones
masa_t *mallas_iter_ver_masa_actual(mallas_iter_masa_t *mm);

// Avanza el iterador de resortes a la siguiente posición
// Precondiciones: mi es un puntero válido a un iterador de resortes creado correctamente
// Postcondiciones:
// - El iterador avanza a la siguiente posición si no ha llegado al final de la lista de resortes
// - Si el iterador ha llegado al final de la lista de resortes o es inválido, no se realiza ninguna modificación
void mallas_iter_resorte_avanzar(mallas_iter_resorte_t *li);

// Verifica si el iterador de resortes ha llegado al final de la lista de resortes
// Precondiciones: mi es un puntero válido a un iterador de resortes creado correctamente
// Postcondiciones:
// - Devuelve true si el iterador ha llegado al final de la lista de resortes
// - Devuelve false si el iterador no ha llegado al final de la lista de resortes o si el iterador es inválido
bool mallas_iter_resorte_al_final(mallas_iter_resorte_t *li);


// Destruye el iterador de resortes
// Precondiciones: mi es un puntero válido a un iterador de resortes creado correctamente
// Postcondiciones: El iterador de resortes y todos sus recursos asociados son liberados de memoria
void mallas_iter_resorte_destruir(mallas_iter_resorte_t *li);

// Devuelve el resorte actual apuntado por el iterador de resortes
// Precondiciones: mi es un puntero válido a un iterador de resortes creado correctamente
// Postcondiciones:
// - Si el iterador es válido y no ha llegado al final de la lista de resortes, se devuelve un puntero al resorte actual
// - Si el iterador ha llegado al final de la lista de resortes o es inválido, se devuelve NULL
resorte_t *mallas_iter_ver_resorte_actual(mallas_iter_resorte_t *li);

// Crea un iterador de resortes para recorrer los resortes de una malla
// Precondiciones: malla es un puntero válido a una malla creada correctamente
// Postcondiciones:
// - Si se pudo crear el iterador, se devuelve un puntero al iterador creado
// - Si no se pudo crear el iterador, se devuelve NULL y no se modifica la malla
mallas_iter_resorte_t *mallas_iter_resorte_crear(malla_t *malla);

// Precondición: 'malla' es un puntero válido a una malla creada previamente.
// Postcondición: Devuelve la cantidad de masas en la malla.
size_t malla_n_masas(malla_t *malla);

// Precondición: 'malla' es un puntero válido a una malla creada previamente.
// Postcondición: Devuelve la cantidad de resortes en la malla.
size_t malla_n_resortes(malla_t *malla);

// Precondiciones:
// - malla es un puntero válido a una estructura malla_t.
// Postcondiciones:
// - Se liberan todos los recursos de la malla, incluyendo los resortes y las masas.
// - La memoria ocupada por la malla es liberada.
void malla_destruir(malla_t *malla);

// Precondiciones:
// - Ninguna.
// Postcondiciones:
// - Se crea una nueva estructura malla_t con memoria asignada.
// - Se inicializan los campos de la malla con valores iniciales.
// - Si ocurre algún error durante la creación, se liberan los recursos y se devuelve NULL.
malla_t *malla_crear(void);

// Precondiciones:
// - malla es un puntero válido a una estructura malla_t creada previamente.
// - masa es un puntero válido a una estructura masa_t.
// Postcondiciones:
// - Se agrega la masa especificada a la malla.
// - Si la malla no tiene suficiente espacio para almacenar la masa, se redimensiona el arreglo de masas.
// - Si ocurre algún error durante la operación, se destruye la malla y se devuelve false.
// - En caso contrario, se incrementa el contador de masas en la malla y se devuelve true.
bool malla_agregar_masa(malla_t *malla, masa_t *masa);

// Precondiciones:
// - malla es un puntero válido a una estructura malla_t creada previamente.
// - masa es un puntero válido a una estructura masa_t que se encuentra en la malla.
// Postcondiciones:
// - Se saca la masa especificada de la malla, eliminándola del arreglo de masas.
// - Se actualiza el contador de masas en la malla.
// - Se destruye la masa que se sacó de la malla.
void malla_sacar_masa(malla_t *malla, masa_t *masa);

// Precondiciones:
// - malla es un puntero válido a una estructura malla_t creada previamente.
// - resorte es un puntero válido a una estructura resorte_t.
// Postcondiciones:
// - Se agrega el resorte especificado a la malla, insertándolo al principio de la lista de resortes.
// - Se incrementa el contador de resortes en la malla.
// - Retorna true si se pudo agregar el resorte correctamente, o false en caso contrario.
bool malla_agregar_resorte(malla_t *malla, resorte_t *resorte);

// Precondiciones:
// - malla es un puntero válido a una estructura malla_t creada previamente.
// - resorte es un puntero válido a una estructura resorte_t.
// Postcondiciones:
// - Se decrementa el contador de resortes en la malla.
// - Se destruye el resorte eliminado.
// - Retorna true si se encontró y eliminó el resorte correctamente, o false en caso contrario.
bool malla_sacar_resorte(malla_t *malla, resorte_t *resorte);

// Precondiciones:
// - malla es un puntero válido a una estructura malla_t creada previamente.
// - xd y yd son las coordenadas x e y respectivamente para buscar una masa cercana.
// - rango es el rango máximo permitido para considerar una masa como cercana.
// Postcondiciones:
// - Retorna un puntero a la masa encontrada si existe una dentro del rango, o NULL en caso contrario.
masa_t *masa_buscar(malla_t *malla, double x, double y, double rango);

// Precondiciones:
// - malla es un puntero válido a una estructura malla_t creada previamente.
// - xd y yd son las coordenadas x e y respectivamente para buscar y eliminar resortes cercanos.
// - rango es el rango máximo permitido para considerar un resorte como cercano.
// Postcondiciones:
// - Retorna true si se eliminó al menos un resorte, o false si no se eliminó ningún resorte.
bool resorte_buscar_eliminar(malla_t *malla, double xd, double yd, double rango);

// Precondiciones:
// - malla es un puntero válido a una estructura malla_t creada previamente.
// - xd y yd son las coordenadas x e y respectivamente para buscar y limitar resortes.
// - rango es el valor máximo permitido para el parámetro "lo" de un resorte.
// Postcondiciones:
// - Retorna false si se encuentra algún resorte cuyo "lo" sea mayor o igual a rango, o true si no se encuentra ningún resorte que cumpla con esa condición.
bool resorte_buscar_limitar(malla_t *malla, double xd, double yd, double rango);

// Cambia los valores de longitud de reposo (lo) y constante elástica (k) de todos los resortes en una malla
// Precondiciones:
// - malla es un puntero válido a una malla creada correctamente
// - lo_max es un valor válido para la longitud de reposo máxima
// - k_base es un valor válido para la constante elástica base
// - potencia_k es un valor válido para la potencia aplicada a la constante elástica
// Postcondiciones:
// - Se cambian los valores de longitud de reposo (lo) y constante elástica (k) de todos los resortes en la malla
// - Si ocurre algún error durante la actualización de los resortes, se destruye el iterador y se devuelve false
// - Si la malla es inválida, se devuelve false
// - Si se actualizan correctamente todos los resortes, se destruye el iterador y se devuelve true
bool malla_cambiar_resortes_k_lo_all(malla_t *malla, double lo_max, double k_base, double potencia_k);

// Cambia la masa de todas las masas en una malla
// Precondiciones:
// - malla es un puntero válido a una malla creada correctamente
// - masa es un valor válido para la masa
// Postcondiciones:
// - Se cambia la masa de todas las masas en la malla al valor especificado
void malla_cambiar_masas_m_all(malla_t *malla, double masa);

// Obtiene los resortes vinculados a una masa en una malla
// Precondiciones:
// - malla es un puntero válido a una malla creada correctamente
// - masa es un puntero válido a una masa existente en la malla
// - n_vinculados es un puntero válido a una variable donde se almacenará el número de resortes vinculados encontrados
// Postcondiciones:
// - Se retorna un arreglo de punteros a resortes que están vinculados a la masa
// - Se asigna el número de resortes vinculados encontrados en la variable n_vinculados
// - En caso de error, se retorna NULL y n_vinculados se establece en 0
resorte_t **masa_resortes_vinculados(malla_t *malla, masa_t *masa, size_t *n_vinculados);

// Actualiza los resortes vinculados a una malla
// Precondiciones:
// - malla es un puntero válido a una malla creada
// - masa es un puntero válido a una masa existente en la malla
// - y_ant y x_ant son las coordenadas inmediatamente posteriores de la masa
// Postcondiciones:
// - se actualizaron todos los resortes a su nueva longitud, si algun resorte tiene una longitud mayor a 'rango',
// se actualizan las coordenadas de masa a x_ant e y_ant, se actualizan los resortes y devuelve false. Si las longitudes
// no pasan de 'rango', devuelve true.
bool malla_masa_actualizar_resortes_vinculados(malla_t *malla, masa_t *masa, double rango, double x_ant, double y_ant);

// Destruye el arreglo de resortes vinculados a una masa
// Precondiciones: resortes es un puntero válido a un arreglo de punteros a resortes
// Postcondiciones: El arreglo de resortes y todos sus recursos asociados son liberados de memoria
void masa_resortes_vinculados_destruir(resorte_t **resortes);

// Escribe los datos de una malla en un archivo de salida
// Precondiciones:
// - malla es un puntero válido a una malla creada correctamente
// - salida es un puntero válido a un archivo abierto en modo escritura
// - nivel es un valor válido para representar el nivel de la malla
// Postcondiciones:
// - Se escriben los datos de la malla en el archivo de salida según el formato especificado
// - Si ocurre algún error durante la escritura de los datos, se destruyen los iteradores y se devuelve false
// - Si la malla es inválida, se devuelve false
// - Si se escriben correctamente todos los datos, se destruyen los iteradores y se devuelve true
bool escribir_malla(malla_t *malla, FILE *salida, size_t nivel);


// Lee los datos de una malla desde un archivo de entrada y crea la malla correspondiente
// Precondiciones:
// - entrada es un puntero válido a un archivo abierto en modo lectura
// Postcondiciones:
// - Se crea una nueva malla y se llenan los datos leídos desde el archivo
// - Si ocurre algún error durante la lectura de los datos, se destruye la malla creada y se devuelve NULL
// - Si los datos se leen correctamente, se devuelve un puntero a la malla creada
malla_t* leer_malla(FILE* entrada);

// Precondiciones:
// - malla es un puntero válido a una malla existente

// Postcondiciones:
// - Si malla es NULL o id es mayor o igual a la cantidad de masas en la malla, se retorna NULL
// - Si se encuentra la masa con el ID especificado, se retorna un puntero a la masa correspondiente
masa_t* malla_obtener_masa_por_id(malla_t* malla, size_t id);

// Precondiciones:
// - nivel es el nivel del juego para el cual se desea crear la malla
// - id es un puntero válido que apunta a un valor entero para asignar los ID de las masas
// - escala es el valor de escala para ajustar las coordenadas de las masas
// Postcondiciones:
// - Si ocurre algún error durante la creación de las masas o de la malla, se retorna NULL
// - Se crea una nueva malla para el nivel especificado y se agregan las masas correspondientes
// - El puntero id se incrementa para el siguiente ID disponible
malla_t *crear_nivel(size_t nivel, size_t *id, double escala);


#endif


