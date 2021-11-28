#include "lista.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct nodo nodo_t;

struct nodo {
    void* dato; // Apunta al dato almacenado
    nodo_t* prox; // Apunta al proximo nodo
};

struct lista {
    nodo_t* primero;
    nodo_t* ultimo;
    size_t largo;
};

struct lista_iter {

};

// *** PRIMITIVAS DE NODO ***
nodo_t *nodo_crear(void* dato) {
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if (nodo == NULL) return NULL;

    nodo->dato = dato;
    nodo->prox = NULL;

    return nodo;
}

// *** PRIMITIVAS DE ITERADOR EXTERNO ***
lista_iter_t *lista_iter_crear(lista_t *lista) {

}

bool lista_iter_avanzar(lista_iter_t *iter) {

}

void *lista_iter_ver_actual(const lista_iter_t *iter) {

}

bool lista_iter_al_final(const lista_iter_t *iter) {

}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {

}

void *lista_iter_borrar(lista_iter_t *iter) {

}

// *** PRIMITIVA ITERADOR INTERNO ***
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
    nodo_t *nodo = lista->primero;

    while(nodo->prox) {
        if(!visitar(nodo->dato, extra)) return;
    
        nodo = nodo->prox;
    }
    return;
}

// *** PRIMITIVAS DE LISTA ***
lista_t *lista_crear(void) {
    lista_t *lista = malloc(sizeof(lista_t));
    
    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->largo = 0;

    return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    return !lista->largo;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_t *nodo = nodo_crear(dato);

    // En el caso de insertar a lista vacia
    if (!lista->largo) {
        lista->primero = nodo;
        lista->ultimo = nodo;
        nodo->prox = NULL;
        lista->largo++;
        return true;
    }

    nodo->prox = lista->primero;
    lista->primero = nodo;
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    nodo_t *nodo = nodo_crear(dato);

    // En el caso de insertar a lista vacia
    if (!lista->largo) {
        lista->primero = nodo;
        lista->ultimo = nodo;
        nodo->prox = NULL;
        lista->largo++;
        return true;
    }

    lista->ultimo->prox = nodo;
    lista->ultimo = nodo;
    lista->largo++;
    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if (lista_esta_vacia(lista)) return NULL;

    void *dato = lista->primero->dato; // Guardamos el dato que vamos a devolver

    // Caso que solo haya un dato en la lista
    if (lista->largo == 1) {
        free(lista->primero);
        lista->primero = NULL;
        lista->ultimo = NULL;
        lista->largo = 0;
        return dato;
    }

    void* antiguo_primero = lista->primero;
    lista->primero = lista->primero->prox;
    lista->largo--;
    free(antiguo_primero);

    return dato;
}

void *lista_ver_primero(const lista_t *lista) {
    if (lista_esta_vacia(lista)) return NULL;
    return lista->primero->dato; 
}

void *lista_ver_ultimo(const lista_t* lista) {
    if (lista_esta_vacia(lista)) return NULL;
    return lista->ultimo->dato; 
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
    while(!lista_esta_vacia(lista)) {
        void *dato = lista_borrar_primero(lista);
        if (destruir_dato) destruir_dato(dato);
    }
    free(lista);
}
