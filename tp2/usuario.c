#include "publicacion.h"
#include "./tdas_aux/heap.h"

struct usuario {
    char* nombre;
    heap_t* feed;
    int id;
};

int afinidad_min(const void* a, const void* b){
    return ((publicacion_afinidad_t*)a)->afinidad - ((publicacion_afinidad_t*)b)->afinidad;
}


// PRIMITIVAS DE USUARIO
usuario_t* usuario_crear(char* nombre, int id) {
    usuario_t* usuario = malloc(sizeof(usuario_t));
    if (!usuario) return NULL;
    usuario->nombre = strdup(nombre);
    usuario->feed = heap_crear(afinidad_min);
    usuario->id = id;
    return usuario;
}
    
void* ver_proximo_post_feed(usuario_t* usuario) {
    return heap_desencolar(usuario->feed);
}