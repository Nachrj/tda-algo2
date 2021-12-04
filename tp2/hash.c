#include "hash.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "lista.h"
#define CAPACIDAD_INICIAL 11

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

char* copia_clave_hash(const char *clave){
	size_t len = strlen(clave);
	char* copia = malloc((len+1)*sizeof(char));
	strcpy((char*)copia,clave);
	return copia;
}

item_t* item_crear(const char *clave, void *dato){
	item_t* item = malloc(sizeof(item_t));
	if (item==NULL) return NULL;
	item->clave = copia_clave_hash(clave);
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
bool hash_guardar(hash_t* hash, const char* clave, void* dato) {
    
    if (hash->cantidad / hash->capacidad >= 2) {
        if (!hash_redimensionar(hash, hash->capacidad * 2)) return false;
    }

    size_t clave_hash = hash_f(clave, hash->capacidad);
    if (!hash_pertenece(hash, clave)) {
        item_t* item = item_crear(clave,dato);
        // Si no existe, inserto
        if (!lista_insertar_ultimo(hash->lista[clave_hash], item)) return false;
        hash->cantidad++;
        return true;
    }

    // Si ya existe, actualizo el dato
    lista_iter_t* iter = lista_iter_crear(hash->lista[clave_hash]);
    while (!lista_iter_al_final(iter)) {
        item_t* item = lista_iter_ver_actual(iter);
        if (!strcmp(item->clave,clave)) {
            if(hash->destruir_dato)
                hash->destruir_dato(item->dato);
            
            item->dato = dato;
            lista_iter_destruir(iter);
            return true;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    hash->cantidad++;
    return true;
    
  
  
}


void* hash_borrar(hash_t* hash, const char* clave) {

    size_t clave_hash = hash_f(clave, hash->capacidad);
    if (!hash->lista[clave_hash]) return NULL;

    lista_iter_t* iter = lista_iter_crear(hash->lista[clave_hash]);
    if (!iter) return NULL;

    while (!lista_iter_al_final(iter)) {
        item_t* actual = lista_iter_ver_actual(iter);
        if (!strcmp(actual->clave,clave)) {
            item_t* item = lista_iter_borrar(iter);
            void* dato = item->dato;
            free((char*)item->clave);
            free(item);
            hash->cantidad--;
            lista_iter_destruir(iter);
            return dato;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    if (hash->cantidad / hash->capacidad <= 1) {
        if (!hash_redimensionar(hash, hash->capacidad / 2)) return false;
    }
    return NULL;
}

void* hash_obtener(const hash_t* hash, const char* clave) {
    size_t clave_hash = hash_f(clave, hash->capacidad);
    if (!hash->lista[clave_hash]) return NULL;

    lista_iter_t* iter = lista_iter_crear(hash->lista[clave_hash]);
    if (!iter) return NULL;

    while (!lista_iter_al_final(iter)) {
        item_t* actual = lista_iter_ver_actual(iter);
        if (!strcmp(actual->clave,clave)) {
        lista_iter_destruir(iter);
        return actual->dato;
        }
        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    return NULL;
}

bool hash_pertenece(const hash_t* hash, const char* clave) {
    size_t clave_hash = hash_f(clave, hash->capacidad);
    if (!hash->lista[clave_hash]) return false;

    lista_iter_t* iter = lista_iter_crear(hash->lista[clave_hash]);
    if (!iter) return false;

    while (!lista_iter_al_final(iter)) {
        item_t* actual = lista_iter_ver_actual(iter);
        if (!strcmp(actual->clave,clave)) {
        lista_iter_destruir(iter);
        return true;
        }
        lista_iter_avanzar(iter);
    }

    lista_iter_destruir(iter);
    return false;
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
/*
int main() {
    hash_t* hash = hash_crear(free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    // Pide memoria para 4 valores 
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    // Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) 
    hash_guardar(hash, clave1, valor1a);
    print_test("Prueba hash obtener clave1 es valor1a", hash_obtener(hash, clave1) == valor1a);
    print_test("Prueba hash obtener clave1 es valor1a", hash_obtener(hash, clave1) == valor1a);
    print_test("Prueba hash insertar clave2", hash_guardar(hash, clave2, valor2a));
    print_test("Prueba hash obtener clave2 es valor2a", hash_obtener(hash, clave2) == valor2a);
    print_test("Prueba hash obtener clave2 es valor2a", hash_obtener(hash, clave2) == valor2a);
    print_test("Prueba hash la cantidad de elementos es 2", hash_cantidad(hash) == 2);

    print_test("Prueba hash insertar clave1 con otro valor", hash_guardar(hash, clave1, valor1b));
    print_test("Prueba hash obtener clave1 es valor1b", hash_obtener(hash, clave1) == valor1b);
    print_test("Prueba hash obtener clave1 es valor1b", hash_obtener(hash, clave1) == valor1b);
    print_test("Prueba hash insertar clave2 con otro valor", hash_guardar(hash, clave2, valor2b));
    print_test("Prueba hash obtener clave2 es valor2b", hash_obtener(hash, clave2) == valor2b);
    print_test("Prueba hash obtener clave2 es valor2b", hash_obtener(hash, clave2) == valor2b);
    print_test("Prueba hash la cantidad de elementos es 2", hash_cantidad(hash) == 2);

    // Se destruye el hash (se debe liberar lo que quedó dentro) 
    hash_destruir(hash);
}*/