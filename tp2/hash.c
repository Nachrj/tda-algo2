#include "hash.h"
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FACTOR_REDIMENSION 0.75

typedef enum { VACIO, OCUPADO, BORRADO } estado;

typedef struct campo {
  char* clave;
  void* valor;
  estado estado;
} campo_t;

struct hash {
  size_t cantidad;
  size_t cantidad_total;
  hash_destruir_dato_t destructor;
  size_t capacidad;
  campo_t** tabla;
};

struct hash_iter {
  const hash_t* hash;
  size_t pos;
};

campo_t* campo_crear(const char* clave, void* dato) {
  campo_t* campo = malloc(sizeof(campo_t));

  if (!campo) {
    return NULL;
  }

  campo->clave = NULL;
  campo->estado = VACIO;
  campo->valor = dato;

  if (clave) {
    campo->clave = strdup(clave);
  }

  return campo;
}

bool campo_vaciar(campo_t* campo) {
  free(campo->clave);
  campo->clave = NULL;
  campo->valor = NULL;
  campo->estado = BORRADO;

  return true;
}

bool campo_guardar(campo_t* campo, const char* clave, void* dato) {
  campo->valor = dato;
  campo->estado = OCUPADO;
  if (clave) {
    campo->clave = strdup(clave);
  }

  return true;
}

hash_t* _hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad) {
  hash_t* hash = calloc(1, sizeof(hash_t));

  if (!hash) {
    return NULL;
  }

  hash->destructor = destruir_elemento;
  hash->capacidad = capacidad;

  hash->tabla = calloc(hash->capacidad, sizeof(campo_t));

  if (!hash->tabla) {
    free(hash);
    return NULL;
  }

  for (size_t i = 0; i < hash->capacidad; i++) {
    hash->tabla[i] = campo_crear(NULL, NULL);
    if (hash->tabla[i] == NULL) {
      free(hash);
      return NULL;
    }
  }

  return hash;
}

hash_t* hash_crear(hash_destruir_dato_t destruir_elemento) {
  return _hash_crear(destruir_elemento, 29);
}

size_t contador_ocupados(const hash_t* hash) {
  size_t counter = 0;
  for (size_t i = 0; i < hash->capacidad; i++) {
    if (hash->tabla[i] && hash->tabla[i]->estado == OCUPADO) counter++;
  }

  return counter;
}

bool hash_rehasheo_v2(hash_t* hash_viejo, hash_t* hash) {
  bool ok = true;
  for (size_t i = 0; i < hash_viejo->capacidad; i++) {
    if (hash_viejo->tabla[i]->estado == OCUPADO) {
      ok &= hash_guardar(hash, hash_viejo->tabla[i]->clave,
                         hash_viejo->tabla[i]->valor);
    }
  }

  return ok;
}

size_t funcion_hash(const char* str, size_t largo) {
  size_t hash = 5381, c;

  while ((c = (size_t)*str++)) {
    hash += (hash << 5) + (c | 040);
  }
  return hash % largo;
}

void hash_tabla_destruir(campo_t** tabla, size_t capacidad,
                         hash_destruir_dato_t destruir) {
  for (size_t i = 0; i < capacidad; i++) {
    if (tabla[i]->estado == OCUPADO) {
      if (destruir) destruir(tabla[i]->valor);

      free(tabla[i]->clave);
    }
    free(tabla[i]);
  }
  free(tabla);
}

bool hash_redimensionar(hash_t* hash) {
  hash_t* nuevo = _hash_crear(hash->destructor, hash->capacidad * 2);

  if (!nuevo) {
    return false;
  }

  bool rehasheo = hash_rehasheo_v2(hash, nuevo);

  if (!rehasheo) {
    return false;
  }

  hash_tabla_destruir(hash->tabla, hash->capacidad, NULL);

  hash->tabla = nuevo->tabla;
  hash->cantidad = nuevo->cantidad;
  hash->capacidad = nuevo->capacidad;
  hash->cantidad_total = nuevo->cantidad;

  free(nuevo);

  return true;
}

bool factor_redimension(hash_t* hash) {
  double factor = (double)hash->cantidad_total / (double)hash->capacidad;
  return factor > FACTOR_REDIMENSION;
}

bool posicion_vacia(const hash_t* hash, size_t posicion) {
  return hash->tabla[posicion]->estado == VACIO;
}

bool posicion_ocupado(const hash_t* hash, size_t posicion) {
  return hash->tabla[posicion]->estado == OCUPADO;
}

bool buscar_vacio_clave_igual(hash_t* hash, size_t pos_ini, const char* clave) {
  size_t i = pos_ini;
  do {
    campo_t* campo_act = hash->tabla[i];
    if (posicion_vacia(hash, i) ||
        (posicion_ocupado(hash, i) && strcmp(clave, campo_act->clave) == 0)) {
      return i;
    }

    i = (i + 1) % hash->capacidad;
  } while (i != pos_ini);
  return i;
}

bool hash_guardar(hash_t* hash, const char* clave, void* dato) {
  size_t pos_inicial = funcion_hash(clave, hash->capacidad);

  size_t pos = pos_inicial;

  do {
    campo_t* campo = campo = hash->tabla[pos];
    if (posicion_vacia(hash, pos)) {
      campo_guardar(campo, clave, dato);
      hash->cantidad++;
      hash->cantidad_total++;
      if (factor_redimension(hash)) {
        return hash_redimensionar(hash);
      }

      return true;
    } else if (posicion_ocupado(hash, pos) &&
               strcmp(clave, campo->clave) == 0) {
      if (hash->destructor) hash->destructor(campo->valor);

      campo->valor = dato;

      return true;
    }

    pos = (pos + 1) % hash->capacidad;
  } while (pos != pos_inicial);

  return false;
}

void* hash_borrar(hash_t* hash, const char* clave) {
  if (!hash_pertenece(hash, clave)) return NULL;

  size_t pos_inicial = funcion_hash(clave, hash->capacidad);

  size_t pos_act = pos_inicial;

  do {
    campo_t* actual = hash->tabla[pos_act];
    if (posicion_ocupado(hash, pos_act) && strcmp(actual->clave, clave) == 0) {
      void* dato = actual->valor;
      campo_vaciar(actual);
      hash->cantidad--;
      return dato;
    }

    pos_act = (pos_act + 1) % hash->capacidad;
  } while (pos_inicial != pos_act || posicion_vacia(hash, pos_act));

  return NULL;
}

void* hash_obtener(const hash_t* hash, const char* clave) {
  if (!hash_pertenece(hash, clave)) {
    return NULL;
  }

  size_t posicion = funcion_hash(clave, hash->capacidad);

  size_t i = posicion;

  do {
    campo_t* actual = hash->tabla[i];
    if (posicion_ocupado(hash, i) && strcmp(actual->clave, clave) == 0) {
      return actual->valor;
    }

    i = (i + 1) % hash->capacidad;
  } while (i != posicion || posicion_vacia(hash, i));

  return NULL;
}

size_t hash_cantidad(const hash_t* hash) { return hash->cantidad; }

bool hash_pertenece(const hash_t* hash, const char* clave) {
  size_t pos = funcion_hash(clave, hash->capacidad);
  campo_t* actual = hash->tabla[pos];

  while (!posicion_vacia(hash, pos)) {
    actual = hash->tabla[pos];
    if (posicion_ocupado(hash, pos) && strcmp(actual->clave, clave) == 0) {
      return true;
    }
    pos = (pos + 1) % hash->capacidad;
  }

  return false;
}

void hash_destruir(hash_t* hash) {
  hash_tabla_destruir(hash->tabla, hash->capacidad, hash->destructor);
  free(hash);
}

size_t buscar_posicion_ocupada(hash_iter_t* iter, size_t posicion) {
  size_t i = posicion;
  for (i = posicion; i < iter->hash->capacidad; i++) {
    if (posicion_ocupado(iter->hash, i)) return i;
  }

  return i++;
}

hash_iter_t* hash_iter_crear(const hash_t* hash) {
  hash_iter_t* iter = calloc(1, sizeof(hash_iter_t));

  if (!iter) return NULL;

  iter->hash = hash;

  iter->pos = buscar_posicion_ocupada(iter, 0);

  return iter;
}

bool hash_iter_avanzar(hash_iter_t* iter) {
  if (hash_iter_al_final(iter)) return false;

  iter->pos = buscar_posicion_ocupada(iter, iter->pos + 1);

  return true;
}

const char* hash_iter_ver_actual(const hash_iter_t* iter) {
  if (hash_iter_al_final(iter)) return NULL;

  return iter->hash->tabla[iter->pos]->clave;
}

bool hash_iter_al_final(const hash_iter_t* iter) {
  return iter->pos >= iter->hash->capacidad;
}

void hash_iter_destruir(hash_iter_t* iter) { free(iter); }