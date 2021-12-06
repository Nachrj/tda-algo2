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
    publicacion_afinidad_t* publicacion_afinidad = calloc(1, sizeof(publicacion_afinidad_t));
    publicacion_afinidad->publicacion = publicacion;
    publicacion_afinidad->afinidad = afinidad;
    return publicacion_afinidad;
}

void publicacion_afinidad_destruir(publicacion_afinidad_t* publicacion_afinidad) {
    if (!publicacion_afinidad) return;
    free(publicacion_afinidad);
}

int afinidad_min(const void* a, const void* b){
    if (!(((publicacion_afinidad_t*)b)->afinidad - ((publicacion_afinidad_t*)a)->afinidad)) {
        return publicacion_get_id(((publicacion_afinidad_t*)b)->publicacion) - publicacion_get_id(((publicacion_afinidad_t*)a)->publicacion);
    }
    return ((publicacion_afinidad_t*)b)->afinidad - ((publicacion_afinidad_t*)a)->afinidad;
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
    //printf("%s\n", usuario_get_nombre(usuario));
    //printf("%ld\n", heap_cantidad(usuario->feed));
}

void* ver_proximo_post_feed(usuario_t* usuario) {
    if(heap_esta_vacio(usuario->feed)){
        printf("%s\n", "Usuario no loggeado o no hay mas posts para ver");
        return NULL;
    }
    publicacion_afinidad_t* publi = (publicacion_afinidad_t*)heap_desencolar(usuario->feed);
    void* publi_pointer = publi->publicacion;
    return publi_pointer;
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