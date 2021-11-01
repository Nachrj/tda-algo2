#include "abb.h"
#include <stdlib.h>
typedef struct nodo nodo_t;

struct nodo {
    void* dato; // Apunta al dato almacenado
    char* clave;
    nodo_t* izq; // Apunta al nodo izquierdo
    nodo_t* der; // Apunta al nodo derecho
};

// *** PRIMITIVAS DE NODO ***
nodo_t *nodo_crear(const char* clave, void* dato) {
    nodo_t *nodo = calloc(1,sizeof(nodo_t));
    if (!nodo) return NULL;

    nodo->dato = dato;
    nodo->izq = NULL;
    nodo->der = NULL;

    return nodo;
}

void nodo_destruir(nodo_t* nodo){
    free(nodo);
}

struct abb{
    nodo_t* raiz;
    size_t cantidad;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
    
};

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = calloc(1,sizeof(abb_t));
    if(!abb) return NULL;
    abb->cmp = cmp;
    abb->destruir_dato = destruir_dato;
    abb->cantidad = 0;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){

}