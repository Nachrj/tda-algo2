#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "publicacion.h"
#include <string.h>

struct publicacion {
    usuario_t* usuario_creador;
    abb_t* likes;
    int cant_likes;
    char* texto;
    int id;
};

// PRIMITIVAS PUBLICACIONES
publicacion_t* publicacion_crear(usuario_t* usuario_creador, char* texto, int id) {
    publicacion_t* publicacion = calloc(1, sizeof(publicacion_t));
    if (!publicacion) return NULL;
    publicacion->usuario_creador = usuario_creador;
    publicacion->likes = abb_crear(strcmp, NULL);
    publicacion->texto = strdup(texto);
    publicacion->id = id;
    return publicacion;
}

bool agregar_usuario_likes(publicacion_t* publicacion, usuario_t* usuario) {
    if (!publicacion || !usuario) return false;
    if(abb_pertenece(publicacion->likes, usuario_get_nombre(usuario))) return true;
    if (abb_guardar(publicacion->likes, usuario_get_nombre(usuario), usuario)) {
        publicacion->cant_likes++;
        return true;
    }
    return false;
}

bool imprimir_likes(const char* clave, void* dato, void* extra) {
    printf("\t%s\n", (char*)clave);
    return true;
}

void mostrar_likes(publicacion_t* publicacion) {
    printf("El post tiene %d likes:\n",publicacion->cant_likes);
    abb_in_order(publicacion->likes, imprimir_likes, NULL);
}

usuario_t* publicacion_get_usuario_creador(publicacion_t* publicacion) {
    return publicacion->usuario_creador;
}

int publicacion_get_id(publicacion_t* publicacion){
    return publicacion->id;
}

char* publicacion_get_texto(publicacion_t* publicacion){
    return publicacion->texto;
}

int publicacion_get_likes(publicacion_t* publicacion){
    return publicacion->cant_likes;
}

void publicacion_destruir(publicacion_t* publicacion) {
    if (!publicacion) return;
    free(publicacion->texto);
    abb_destruir(publicacion->likes);
    free(publicacion);
}

