#include "pila.h"
#include <stdio.h>
#include <stdlib.h>
#define CAPACIDAD 10
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
pila_t *pila_crear(void){
    pila_t *pila = malloc(sizeof(pila_t));
    pila->capacidad = CAPACIDAD;
    pila->cantidad = 0;
    pila->datos = malloc(pila->capacidad * sizeof(void*));

    if(pila->capacidad > 0 && pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    return pila;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
    return pila->cantidad == 0;
}

bool redimensionar_pila(pila_t *pila, size_t nuevo_tam){
    void** datos_nuevo = realloc(pila->datos, nuevo_tam * sizeof(void*));
    if(!datos_nuevo){
        return false;
    }
    pila->capacidad = nuevo_tam;
    pila->datos = datos_nuevo;
    return true;
}

bool pila_apilar(pila_t *pila, void *valor){
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    if(pila->capacidad == pila->cantidad){
        if(!redimensionar_pila(pila,pila->capacidad * 2)){
            return false;
        }
    }
    return true;
}

void *pila_ver_tope(const pila_t *pila){
    if(pila_esta_vacia(pila)){
        return NULL;
    }
    return pila->datos[pila->cantidad-1];
}

void *pila_desapilar(pila_t *pila){
    if(pila_esta_vacia(pila)){
        return NULL;
    }
    void* result = pila->datos[pila->cantidad-1];
    pila->cantidad--;
    if(pila->cantidad * 4 <= pila->capacidad){
        size_t nueva_capacidad = pila->capacidad / 2;
        if(nueva_capacidad < CAPACIDAD){
            nueva_capacidad = CAPACIDAD;
        }
        if(!redimensionar_pila(pila,nueva_capacidad)){
            return false;
        } 
    }
    return result;
}
// ...
