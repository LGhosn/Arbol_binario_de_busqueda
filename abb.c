#define _POSIX_C_SOURCE 200809L

#include "abb.h"
#include "pila.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* *****************************************************************
 *                          ESTRUCTURAS
 * *****************************************************************/
typedef struct abb_nodo {
    char* clave;
    void* valor;
    struct abb_nodo* izq;
    struct abb_nodo* der;
} abb_nodo_t;

struct abb {
    abb_nodo_t* raiz;
    size_t cantidad;
    abb_comparar_clave_t comparar;
    abb_destruir_dato_t destruir_dato;  
};


struct abb_iter{
    const abb_t *arbol;
    pila_t *pila;
};

/* *****************************************************************
 *                          AUXILIARES
 * *****************************************************************/

/*
Crea un nodo binario que va almacena la clave y el valor.
*/
abb_nodo_t *nodo_crear(char* clave, void *valor){
    abb_nodo_t *nodo = malloc(sizeof(abb_nodo_t));
    if (!nodo){
        return NULL;
    }

    nodo->clave = clave;
    nodo->valor = valor;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

/*
Busca el padre de cualquier nodo, en caso de no tenerlo devuelve null.
Si se pasa true por el parametro 'tracker' devuelve el nodo en si.
*/
abb_nodo_t *buscar_nodo(const abb_t* arbol, const char* clave, abb_nodo_t* nodo, bool tracker) {
    if (!nodo || arbol->comparar(clave, nodo->clave) == 0){
        if (tracker) return nodo;
        return NULL;
    }

    if (nodo->izq && arbol->comparar(nodo->izq->clave, clave) == 0){
        if (tracker) return nodo->izq;
        return nodo;
    }
    if (nodo->der && arbol->comparar(nodo->der->clave, clave) == 0){
        if (tracker) return nodo->der;
        return nodo;
    }
    int res = arbol->comparar(clave, nodo->clave);
    
    if (res < 0) {
        return buscar_nodo(arbol, clave, nodo->izq, tracker);
    }
    return buscar_nodo(arbol, clave, nodo->der, tracker);    
}

/*
Asigna un hijo a un correspondiente padre
*/
void asignar_hijo(const abb_t *arbol, const char *clave, abb_nodo_t * nodo, abb_nodo_t* nodo_a_guardar){
    int res = arbol->comparar(clave, nodo->clave);
    if (res < 0) {
        if (!nodo->izq) {
            nodo->izq = nodo_a_guardar;
        }
        else{
            asignar_hijo(arbol, clave, nodo->izq, nodo_a_guardar);
        }
    }

    if (res > 0){
        if (!nodo->der){
            nodo->der = nodo_a_guardar;
        }
        else{
            asignar_hijo(arbol, clave, nodo->der, nodo_a_guardar);
        }
    }
    
}

/*
wrapper para destruir el abb en forma post-order
*/
void _abb_destruir(abb_t *arbol, abb_nodo_t *nodo){
    if (!nodo){
        return;
    }
    _abb_destruir(arbol, nodo->izq);
    _abb_destruir(arbol, nodo->der);
    
    if (arbol->destruir_dato){
        arbol->destruir_dato(nodo->valor);
    }
    free(nodo->clave);
    free(nodo);
    arbol->cantidad--;
}

/*
Apila el nodo actual y todos hijos izquierdos a la pila.
*/
void apilar_todos_izq(pila_t *pila, abb_nodo_t *nodo){
    if (!nodo){
        return;
    }

    pila_apilar(pila, nodo);
    apilar_todos_izq(pila, nodo->izq);
}

/*
Borra un elemento que no tiene hijos
*/
void *_abb_borrar_sin_hijos(abb_t *arbol, abb_nodo_t *a_borrar){
    abb_nodo_t *padre = buscar_nodo(arbol, a_borrar->clave, arbol->raiz, false);
    if (!padre){
        arbol->raiz = NULL;
    }
    else if (padre->izq && arbol->comparar(padre->izq->clave, a_borrar->clave) == 0){
        padre->izq = NULL;
    }
    else if (padre->der && arbol->comparar(padre->der->clave, a_borrar->clave) == 0){
        padre->der = NULL;
    }

    void *valor = a_borrar->valor;
    free(a_borrar->clave);
    free(a_borrar);
    arbol->cantidad--;
    return valor;
}

/*
Borra un elemento que solo tiene un hijo
*/
void *_abb_borrar_un_hijo(abb_t *arbol, abb_nodo_t *a_borrar) {
    abb_nodo_t *padre = buscar_nodo(arbol, a_borrar->clave, arbol->raiz, false);
    if (!padre){
        if (arbol->raiz->izq) arbol->raiz = arbol->raiz->izq;
        else arbol->raiz = arbol->raiz->der;
    }
    else if (padre-> izq && arbol->comparar(padre->izq->clave, a_borrar->clave) == 0){
        if (a_borrar->izq) padre->izq = a_borrar->izq;
        else padre->izq = a_borrar->der;
    }
    else if (padre-> der && arbol->comparar(padre->der->clave, a_borrar->clave) == 0){
        if (a_borrar->izq) padre->der = a_borrar->izq;
        else padre->der = a_borrar->der;
    }

    void* res = a_borrar->valor;
    free(a_borrar->clave);
    free(a_borrar);
    arbol->cantidad--;
    return res;
}

/*
Busca de los hijos menores el mas grande entre ellos de cualquier elemento
*/
abb_nodo_t *hijo_menor_mas_grande(abb_t *arbol, abb_nodo_t *a_borrar){
    abb_nodo_t *res = a_borrar->izq;
    while (res->der){
        res = res->der;
    }
    return res;
}

/*
Borra un elemento que tiene dos hijos.
*/
void *_abb_borrar_dos_hijos(abb_t *arbol, abb_nodo_t *a_borrar){
    abb_nodo_t *reemplazo = hijo_menor_mas_grande(arbol, a_borrar);
    char *clave_reemplazo = strdup(reemplazo->clave);
    void *valor_reemplazo = abb_borrar(arbol, clave_reemplazo);

    void *valor = a_borrar->valor;
    a_borrar->valor = valor_reemplazo;
    free(a_borrar->clave);
    a_borrar->clave = clave_reemplazo;

    return valor;
}


/* *****************************************************************
 *                          PRIMITIVAS
 * *****************************************************************/

abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t *abb = malloc(sizeof(abb_t));
    if (!abb){
        return NULL;
    }

    abb->raiz = NULL;
    abb->comparar = cmp;
    abb->destruir_dato = destruir_dato;
    abb->cantidad = 0;

    return abb;
}

size_t abb_cantidad(const abb_t *arbol){
    return arbol->cantidad;
}

void abb_destruir(abb_t *arbol){
    _abb_destruir(arbol, arbol->raiz);
    free(arbol);
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    abb_nodo_t *reemplazar = buscar_nodo(arbol, clave, arbol->raiz, true);
    if (reemplazar){
        if (arbol->destruir_dato){
            arbol->destruir_dato(reemplazar->valor);
        }
        reemplazar->valor = dato;
        return true;
    }


    char *duplicado = strdup(clave);
    if (!duplicado){
        return false;
    }
    abb_nodo_t *nodo = nodo_crear(duplicado, dato);
    if (!nodo){
        free(duplicado);
        return false;
    }

    if (abb_cantidad(arbol) == 0){
        arbol->raiz = nodo;
    }
    else{
        asignar_hijo(arbol, clave, arbol->raiz, nodo);
    }

    arbol->cantidad++;
    return true;
}


bool abb_pertenece(const abb_t *arbol, const char *clave){
    abb_nodo_t *nodo = buscar_nodo(arbol, clave, arbol->raiz, true);
    return !nodo ? false : true;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    abb_nodo_t *nodo = buscar_nodo(arbol, clave, arbol->raiz, true);
    if (!nodo) return NULL;

    void *valor = nodo->valor;
    return valor;
}

void *abb_borrar(abb_t* arbol, const char* clave) {
    abb_nodo_t* a_borrar = buscar_nodo(arbol, clave, arbol->raiz, true);
    if (!a_borrar) return NULL;

    //Caso de borrar sin hijos
    if (!a_borrar->izq && !a_borrar->der){ 
        return _abb_borrar_sin_hijos(arbol, a_borrar);
    }
    
    //Caso de borrar con un hijo
    if (!a_borrar->izq || !a_borrar->der){
        return _abb_borrar_un_hijo(arbol, a_borrar);
    }

    //Caso de borrar con dos hijos
    return _abb_borrar_dos_hijos(arbol, a_borrar);
}

/* *****************************************************************
 *                       ITERADOR INTERNO
 * *****************************************************************/

void abb_in_order(abb_t *arbol, bool visitar(const char *clave, void *valor, void *extra), void *extra) {
    pila_t *pila = pila_crear();
    if (!pila) {
        return;
    }

    apilar_todos_izq(pila, arbol->raiz);
    abb_nodo_t* actual = pila_desapilar(pila);

    while (actual && visitar(actual->clave, actual->valor, extra)) {
        apilar_todos_izq(pila, actual->der);
        actual = pila_desapilar(pila);
    }

    pila_destruir(pila);
}
/* *****************************************************************
 *                       ITERADOR EXTERNO
 * *****************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t *iter = malloc(sizeof(abb_iter_t));
    if (!iter) return NULL;

    pila_t* pila = pila_crear();

    if (!pila){
        free(iter);
        return NULL;
    }
    
    iter->arbol = arbol;
    iter->pila = pila;

    apilar_todos_izq(iter->pila, arbol->raiz);
    return iter;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if (abb_iter_in_al_final(iter)){
        return NULL;
    }
    abb_nodo_t *nodo_actual = pila_ver_tope(iter->pila);
    const char *clave = nodo_actual->clave;
    return clave;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if (abb_iter_in_al_final(iter)){
        return false;
    }

    abb_nodo_t *nodo_viejo = pila_desapilar(iter->pila);
    if (nodo_viejo->der) {
        apilar_todos_izq(iter->pila, nodo_viejo->der);
    }
    return true;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t *iter) {
    pila_destruir(iter->pila);
    free(iter);
}
