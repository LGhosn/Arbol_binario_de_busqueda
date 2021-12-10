#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct abb abb_t;
typedef int (*abb_comparar_clave_t)(const char *, const char *);
typedef void (*abb_destruir_dato_t)(void *);
typedef struct abb_iter abb_iter_t;

/* ******************************************************************
 *                        PRIMITIVAS
 * *****************************************************************/

/*
Crea un abb
*/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un elemento en el abb, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura abb fue inicializada
 * Post: Se almacenó el par (clave, dato)
*/
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra un elemento del abb y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura abb fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene el valor de un elemento del abb, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura abb fue inicializada
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Determina si clave pertenece o no al abb.
 * Pre: La estructura abb fue inicializada
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos del abb.
 * Pre: La estructura abb fue inicializada
 */
size_t abb_cantidad(const abb_t *arbol);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura abb fue inicializada
 * Post: La estructura abb fue destruida
 */
void abb_destruir(abb_t *arbol);

/* ******************************************************************
 *                        ITERADOR INTERNO
 * *****************************************************************/

/*
* Itera toda el abb mientras visitar sea true.
* Pre: la abb fue creada.
* Post: se iteró la abb utilizando visitar en cada elemento.
*/
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/* ******************************************************************
 *                        ITERADOR EXTERNO
 * *****************************************************************/

// Crea un iterador del abb.
// Pre: el abb fue creado.
// Post: devuelve un nuevo iterador.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanaza un posicion en el abb en caso de ser posible.
// Pre: el itereador fue creado.
// Post: si el iterador no se encuentra en el final, avanza un posicion.
// sino devuelve false.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Devuelve el dato actual que esta el iterador.
// Pre: el iterador fue creado.
// Post: devuelve el dato actual.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Devuelve si el iterador esta en el ultimo elemento del abb.
// Pre: el iterador fue creado.
// Post: devuelve true si esta al final del abb, false en caso contrario.
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se destruyó el iterador.
void abb_iter_in_destruir(abb_iter_t *iter);