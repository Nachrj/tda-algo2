#include "usuario_publi.h"
#include "usuario.h"

struct usuario {
    char* nombre;
    heap_t* feed;
    int id;
};

struct publicacion_afinidad {
    publicacion_t* publicacion;
    int afinidad;
};

// PRIMITIVAS PUBLICACION CON AFINIDAD
publicacion_afinidad_t* publicacion_afinidad_crear(publicacion_t* publicacion, int afinidad) {
    publicacion_afinidad_t* publicacion_afinidad = malloc(sizeof(publicacion_afinidad_t));
    publicacion_afinidad->publicacion = publicacion;
    publicacion_afinidad->afinidad = afinidad;
    return publicacion_afinidad;
}

void publicacion_afinidad_destruir(publicacion_afinidad_t* publicacion_afinidad) {
    if (!publicacion_afinidad) return;
    publicacion_destruir(publicacion_afinidad->publicacion);
    free(publicacion_afinidad);
}

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

void postear_al_feed(usuario_t* usuario, publicacion_t* publicacion, int afinidad){
    heap_encolar(usuario->feed, publicacion_afinidad_crear(publicacion, afinidad));
}

char* ver_proximo_post_feed(usuario_t* usuario) {
    publicacion_afinidad_t* publicacion = heap_desencolar(usuario->feed);
    return publicacion_get_texto(publicacion->publicacion);
}

char* usuario_get_nombre(usuario_t* usuario){
    return usuario->nombre;
}

int usuario_get_id(usuario_t* usuario){
    return usuario->id;
}

heap_t* usuario_get_feed(usuario_t* usuario){
    return usuario->feed;
}

void _publicacion_afinidad_destruir(void* publicacion){
    publicacion_afinidad_destruir(publicacion);
}
void usuario_destruir(usuario_t* usuario) {
    if (!usuario) return;
    free(usuario->nombre);
    heap_destruir(usuario->feed, _publicacion_afinidad_destruir);
    free(usuario);
}