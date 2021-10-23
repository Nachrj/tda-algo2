#include "pila.h"

#include <stdlib.h>
#include <stddef.h>

// Declaro constantes
#define CAPACIDAD 10
#define MULTIPLICADOR_CANTIDAD 4
#define FACTOR_CAPACIDAD 2

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
pila_t *pila_crear(void) {
    pila_t *pila = malloc(sizeof(pila_t));
    if (pila == NULL) return NULL;

    pila->cantidad = 0;
    pila->capacidad = CAPACIDAD;
    pila->datos = malloc((pila->capacidad) * sizeof(void*));
    
    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }

    return pila;
}

bool pila_redimensionar(pila_t *pila, size_t capacidad_nueva) {
    void *datos_nuevos = realloc(pila->datos,capacidad_nueva*sizeof(void*));

    if (capacidad_nueva > 0 && datos_nuevos == NULL) {
        return false;
    }

    pila->capacidad = capacidad_nueva;
    pila->datos = datos_nuevos;
    return true;
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
    return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void *valor) {
    if (pila->cantidad == pila->capacidad) {
        if (!pila_redimensionar(pila, pila->capacidad * FACTOR_CAPACIDAD)) return false;
    }

    pila->cantidad++;
    pila->datos[(pila->cantidad)-1] = valor;
    return true;
}

void *pila_ver_tope(const pila_t *pila) {
    if (pila_esta_vacia(pila)) return NULL;

    return pila->datos[(pila->cantidad)-1];
}

void *pila_desapilar(pila_t *pila) {
    if (pila_esta_vacia(pila)) return NULL;

    void *dato_a_desapilar = pila->datos[(pila->cantidad)-1];

    if (pila->cantidad * MULTIPLICADOR_CANTIDAD <= pila->capacidad) {
        if (!pila_redimensionar(pila, pila->capacidad / FACTOR_CAPACIDAD))
            return false;
    }

    pila->cantidad--;
    return dato_a_desapilar;
}