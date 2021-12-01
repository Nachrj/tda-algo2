#include "usuario.h"
#include "./tdas_aux/abb.h"

typedef struct publicacion_afinidad {
    publicacion_t* publicacion;
    int afinidad;
} publicacion_afinidad_t;

struct publicacion {
    usuario_t* usuario_creador;
    abb_t* likes;
    int cant_likes;
    char* texto;
    int id;
}

// PRIMITIVAS PUBLICACION CON AFINIDAD
publicacion_afinidad_t* publicacion_afinidad_crear(publicacion_t* publicacion, int afinidad) {
    publicacion_afinidad_t* publicacion_afinidad = malloc(sizeof(publicacion_afinidad_t));
    publicacion_afinidad->publicacion = publicacion;
    publicacion_afinidad->afinidad = afinidad;
    return publicacion_afinidad
}

void publicacion_afinidad_destruir(publicacion_afinidad_t* publicacion_afinidad) {
    if (!publicacion_afinidad) return;
    publicacion_destruir(publicacion_afinidad->publicacion);
    free(publicacion_afinidad);
}

// PRIMITIVAS PUBLICACIONES
publicacion_t* publicacion_crear(usuario_t* usuario_creador, char* texto, int id) {
    publicacion_t* publicacion = malloc(sizeof(publicacion_t));
    if (!publicacion) return NULL;
    publicacion->usuario_creador = usuario_creador;
    publicacion->likes = abb_crear(strcmp, free);
    publicacion->texto = strdup(texto);
    publicacion->id = id;
    return publicacion;
}

bool agregar_usuario_likes(publicacion_t* publicacion, usuario_t* usuario) {
    if (!publicacion || !usuario) return false;
    return abb_guardar(publicacion->likes, strdup(usuario->nombre), usuario);
}

int publicacion_cantidad_likes(publicacion_t* publicacion) {
    return publicacion->cant_likes;
}

bool imprimir_likes(const char* clave, void* dato, void* extra) {
    printf("%s\n", (char*)clave);
    return true;
}

void mostrar_likes(publicacion_t* publicacion) {
    printf("El post tiene %d likes:\n",publicacion->cant_likes);
    abb_in_order(publicacion->likes, imprimir_likes, NULL);
}

void publicacion_destruir(publicacion_t* publicacion) {
    if (!publicacion) return;
    free(publicacion->texto);
    abb_destruir(publicacion->likes);
    free(publicacion);
}

