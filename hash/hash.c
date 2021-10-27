#include "hash.h"
#include "lista.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#define CAPACIDAD_INICIAL 11
size_t hash_f(char* str, size_t n){
    size_t hash = 5381;
    size_t c;
    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c;
    }
    return hash % n;
}

typedef struct item{
    void* dato;
    char* clave;
} item_t;


struct hash{
    lista_t** lista;
    hash_destruir_dato_t destruir_dato;
    size_t capacidad;
    size_t cantidad;
};

hash_t *_hash_malloc(hash_destruir_dato_t destruir_dato, size_t capacidad){
    hash_t* hash = calloc(1,sizeof(hash_t));
    hash->capacidad = capacidad;
    hash->cantidad = 0;
    lista_t** lista = calloc(capacidad,sizeof(lista_t*));
    if(!hash || !lista) return NULL;
    hash->lista = lista;
    hash->destruir_dato = destruir_dato;
    return hash;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    _hash_malloc(destruir_dato, CAPACIDAD_INICIAL);
}

bool hash_redimensionar(hash_t* hash){
    // Creo el nuevo hash con la nueva capacidad 
    hash_t* new_hash = _hash_malloc(hash->destruir_dato, hash->capacidad*2);

    // Rehashing de todos los elementos del hash antiguo
    for(size_t i = 0; i < hash->capacidad; i++){
        lista_iter_t* iter = lista_iter_crear(hash->lista[i]);
        if(!iter) return false;
        item_t* actual = (item_t*)lista_iter_ver_actual(iter);
        hash_guardar(new_hash, actual->clave, actual->dato);
    }
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    size_t clave_hash = hash_f(clave,hash->capacidad);
    item_t* item = calloc(1,sizeof(item_t));
    if(!item) return false;
    item->dato = dato;
    item->clave = clave;
     
    // Si no existe la lista, la creo
    if(!hash->lista[clave_hash]){
        lista_t* lista = lista_crear();
        hash->lista[clave_hash] = lista;
        if(!lista) return false;
    }
    // Preguntar si ya existe

    // Inserto y pregunto si hay que redimensionar el hash
    if(!lista_insertar_ultimo(hash->lista[clave_hash],&item)) return false;
    hash->cantidad++;
    if(hash->cantidad/hash->capacidad > 1) return hash_redimensionar(hash);
    return true;
}








