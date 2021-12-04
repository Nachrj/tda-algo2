#include "lista.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

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
    nodo_t* anterior;
    nodo_t* actual;
    lista_t* lista;
};

// *** PRIMITIVAS DE NODO ***
nodo_t *nodo_crear_lista(void* dato) {
    nodo_t *nodo = calloc(1,sizeof(nodo_t));
    if (nodo == NULL) return NULL;

    nodo->dato = dato;
    nodo->prox = NULL;

    return nodo;
}

void nodo_destruir_lista(nodo_t* nodo){
    free(nodo);
}

// *** PRIMITIVAS DE ITERADOR EXTERNO ***
lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t *iter = calloc(1,sizeof(lista_iter_t));
    if(!iter) return NULL; 
    iter->anterior = NULL;
    iter->lista = lista;
    iter->actual = lista->primero;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if(lista_iter_al_final(iter)){
        return false;
    }
    iter->anterior = iter->actual;
    iter->actual = iter->actual->prox;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    if(lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)) return NULL;
    
    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
    return !iter->actual;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    nodo_t* nodo = nodo_crear_lista(dato);
    if(!nodo) return false;
    
    // Insertar primero
    if(!iter->anterior){
        if(lista_esta_vacia(iter->lista)){
            iter->lista->ultimo = nodo;
        }
        nodo->prox = iter->actual;
        iter->actual = nodo;
        iter->lista->primero = nodo;
        iter->lista->largo++;
        return true;
    }
    iter->lista->largo++;
    iter->anterior->prox = nodo;

    // Insertar en el ultimo
    if(lista_iter_al_final(iter)){
        iter->actual = nodo;
        iter->lista->ultimo = nodo;
        return true;
    }
    nodo->prox = iter->actual;
    iter->actual = nodo;
    return true;

    
}

void *lista_iter_borrar(lista_iter_t *iter) {
    if(lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)) return NULL;
    
    void* dato = iter->actual->dato;
    nodo_t* aux = iter->actual;
    iter->lista->largo--;
    // Borrar primero
    if(!iter->anterior){
        iter->actual = iter->actual->prox;
        iter->lista->primero = iter->actual;
        free(aux);
        return dato;
    }
    
    iter->anterior->prox = iter->actual->prox;
    iter->actual = iter->actual->prox;
    // Borrar en el ultimmo
    if(lista_iter_al_final(iter)){
        iter->lista->ultimo = iter->anterior;
    }
    free(aux);
    return dato;

    
}

// *** PRIMITIVA ITERADOR INTERNO ***
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
    nodo_t *nodo = lista->primero;

    while(nodo) {
        if(!visitar(nodo->dato, extra)) return;
        nodo = nodo->prox;
    }
    return;
}

// *** PRIMITIVAS DE LISTA ***
lista_t *lista_crear(void) {
    lista_t *lista = calloc(1,sizeof(lista_t));
    
    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->largo = 0;

    return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    return lista->largo == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_t *nodo = nodo_crear_lista(dato);
    if(!nodo) return false;
    // En el caso de insertar a lista vacia
    if (lista_esta_vacia(lista)) {
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
    nodo_t *nodo = nodo_crear_lista(dato);
    if(!nodo) return false;
    // En el caso de insertar a lista vacia
    if (lista_esta_vacia(lista)) {
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
