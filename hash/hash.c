#include "hash.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testing.h"

#include "lista.h"
#define CAPACIDAD_INICIAL 11
#define FACTOR_DE_CARGA 2
#define FACTOR_DE_DESCARGA 0.7
#define REDIMENSION 2
size_t hash_f(const char* str, size_t n) {
    size_t hash = 5381;
    size_t c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % n;
}

typedef struct item {
    void* dato;
    char* clave;
} item_t;


struct hash {
    lista_t** lista;
    hash_destruir_dato_t destruir_dato;
    size_t capacidad;
    size_t cantidad;
};

struct hash_iter {
    const hash_t* hash;
    lista_iter_t* lista_iter;
    size_t pos;
};

char* copia_clave(const char *clave){
	size_t len = strlen(clave);
	char* copia = malloc((len+1)*sizeof(char));
    if(!copia) return NULL;
	strcpy((char*)copia,clave);
	return copia;
}

item_t* item_crear(const char *clave, void *dato){
	item_t* item = malloc(sizeof(item_t));
	if (item==NULL) return NULL;
	item->clave = copia_clave(clave);
	item->dato = dato;
	return item;
}

void item_destruir(hash_destruir_dato_t destruir_dato, item_t* item){
    if(destruir_dato){
        destruir_dato(item->dato);
    }
    free((char*)item->clave);
    free(item);

}

hash_t* hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t* hash = malloc(sizeof(hash_t));
    if(!hash) return NULL;
    hash->capacidad = CAPACIDAD_INICIAL;
    hash->cantidad = 0;
    lista_t** lista = malloc(CAPACIDAD_INICIAL*sizeof(lista_t**));
    if (!hash || !lista) return NULL;
    for (size_t i = 0; i < hash->capacidad; i++) {
        lista[i] = lista_crear();
    }
    
    hash->lista = lista;
    hash->destruir_dato = destruir_dato;
    return hash;
}

bool hash_redimensionar(hash_t* hash, size_t n) {
  // Creo el nuevo hash con la nueva capacidad
    lista_t** new_lista = malloc(n*sizeof(lista_t**));
    if (!new_lista) return false;
    for (size_t j = 0; j < n; j++) {
        new_lista[j] = lista_crear();
    }
    for (size_t i = 0; i < hash->capacidad; i++) {
		lista_t* lista = hash->lista[i];
		while(!lista_esta_vacia(lista)){
			item_t* item = lista_borrar_primero(lista);
			size_t pos = hash_f(item->clave, n);
			lista_insertar_ultimo(new_lista[pos],item); 
		}
		lista_destruir(lista, NULL);
    }
    free(hash->lista);
    hash->lista = new_lista;
    hash->capacidad = n;
    return true;
}

lista_iter_t* buscar_elemento(const hash_t* hash, const char* clave){
    size_t clave_hash = hash_f(clave, hash->capacidad);
    lista_t* lista = hash->lista[clave_hash];
    lista_iter_t* iter = lista_iter_crear(lista);
    while(!lista_iter_al_final(iter)){
        item_t* item = lista_iter_ver_actual(iter);
        if(!strcmp(item->clave, clave)){
            return iter;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return NULL;
}

bool hash_guardar(hash_t* hash, const char* clave, void* dato) {
    if (hash->cantidad / hash->capacidad >= FACTOR_DE_CARGA) {
        if (!hash_redimensionar(hash, hash->capacidad * REDIMENSION)) return false;
    }
    lista_iter_t* iter = buscar_elemento(hash, clave);
    // Si no existe, inserto
    if(!iter){
        item_t* item = item_crear(clave,dato);
        if (!lista_insertar_ultimo(hash->lista[hash_f(clave, hash->capacidad)], item)) return false;
        hash->cantidad++;
        return true;
    }
    item_t* item = lista_iter_ver_actual(iter);
    if(hash->destruir_dato)
        hash->destruir_dato(item->dato);  
    item->dato = dato;
    lista_iter_destruir(iter);
    return true;
}


void* hash_borrar(hash_t* hash, const char* clave) {
    
    lista_iter_t* iter = buscar_elemento(hash, clave);
    if(!iter) return NULL;
    item_t* item = lista_iter_borrar(iter);
    void* dato = item->dato;
    free((char*)item->clave);
    free(item);
    hash->cantidad--;
    lista_iter_destruir(iter);
    if (hash->cantidad / hash->capacidad <= FACTOR_DE_DESCARGA) {
        if (!hash_redimensionar(hash, hash->capacidad / REDIMENSION)) return false;
    }
    return dato;
}

void* hash_obtener(const hash_t* hash, const char* clave) {
    lista_iter_t* iter = buscar_elemento(hash, clave);
    if (!iter) return NULL;
    item_t* actual = lista_iter_ver_actual(iter);
    lista_iter_destruir(iter);
    return actual->dato;
}

bool hash_pertenece(const hash_t* hash, const char* clave) {
    lista_iter_t* iter = buscar_elemento(hash, clave);
    if (!iter) return false;
    lista_iter_destruir(iter);
    return true;
}

size_t hash_cantidad(const hash_t* hash) {
    return hash->cantidad;  // Devuelve la cantidad de elementos del hash
}

void hash_destruir(hash_t* hash) {
    for (size_t i = 0; i < hash->capacidad; i++) {
        lista_iter_t* iter = lista_iter_crear(hash->lista[i]);
        if (!iter) return;
        while(!lista_iter_al_final(iter)){
            item_t* item = lista_iter_borrar(iter);
            if(!item) break;
            item_destruir(hash->destruir_dato,item);
        }
        
        lista_destruir(hash->lista[i],hash->destruir_dato);
        lista_iter_destruir(iter);
    }
    free(hash->lista);
    free(hash);
}

/* Iterador del hash */

hash_iter_t* hash_iter_crear(const hash_t* hash) {
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if(!iter) return NULL;
    iter->pos = 0;
    if (!iter) return NULL;
    iter->hash = hash;
    iter->lista_iter = NULL;
    for (size_t i=0; i < hash->capacidad; i++){
      if (!lista_esta_vacia(hash->lista[i])){
        iter->lista_iter = lista_iter_crear(hash->lista[i]);
        iter->pos = i;
        break;
      }
    }
    return iter;
}

bool hash_iter_avanzar(hash_iter_t* iter) {
    if (!iter->lista_iter) return false;
    if(hash_iter_al_final(iter)) return false;
    if (lista_iter_avanzar(iter->lista_iter)){
		if (!lista_iter_al_final(iter->lista_iter) || hash_iter_al_final(iter)) return true;
	}
    iter->pos++;
    while (iter->pos < iter->hash->capacidad) {
        if (!lista_esta_vacia(iter->hash->lista[iter->pos])) {
            lista_iter_destruir(iter->lista_iter);
            iter->lista_iter = lista_iter_crear(iter->hash->lista[iter->pos]);
            if (!iter->lista_iter) return false;
            return true;
        }
        iter->pos++;
    }
    return false;
}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char* hash_iter_ver_actual(const hash_iter_t* iter) {
    if(hash_iter_al_final(iter)) return NULL;
    item_t* actual = lista_iter_ver_actual(iter->lista_iter);
    if (!actual) return NULL;
    return actual->clave;
}

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t* iter) {
    if (!iter->lista_iter) return true;
    size_t pos_inicial = iter->pos+1;
    if (lista_iter_al_final(iter->lista_iter)) {
        while(pos_inicial != iter->hash->capacidad) {
            if (!lista_esta_vacia(iter->hash->lista[pos_inicial])) return false;
            pos_inicial++;
        }
        return true;    
    }
    return false;
}

// Destruye iterador
void hash_iter_destruir(hash_iter_t* iter) {
    lista_iter_destruir(iter->lista_iter);
    free(iter);
}
