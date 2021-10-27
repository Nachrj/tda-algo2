#include "hash.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#define CAPACIDAD 10

size_t hash_f(char* str){
    size_t hash = 5381;
    int c;
    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c;
    }
    return hash % CAPACIDAD;
}

typedef struct item{
    char* data;
    size_t key;
} item_t;


struct hash{
    item_t** lista;
};


hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t* hash = calloc(1,sizeof(hash_t));
    item_t** lista = calloc(1,sizeof(item_t)*CAPACIDAD);
    if(!hash || !lista) return NULL;
    hash->lista = lista;
    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    size_t clave_hash = hash_f(clave);


}


