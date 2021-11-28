#include "publicacion.h"
#include "./tda_aux/heap.h"

struct usuario {
    char* nombre;
    heap_t* feed;
    int id;
};

usuario_t* usuario_crear(char* nombre, int id) {
    usuario_t* usuario = malloc(sizeof(usuario_t));
    if (!usuario) return NULL;
    usuario->nombre = strdup(nombre);
    usuario->feed = heap_crear(/* METODO DE COMPARACION */);
    usuario->id = id;
    return usuario;
}