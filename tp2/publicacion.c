#include "usuario.h"
#include "./tdas_aux/abb.h"
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
    return abb_guardar(publicacion->likes, strdup(usuario_get_nombre(usuario)), usuario);
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

int publicacion_get_id(publicacion_t* publicacion){
    return publicacion->id;
}

void publicacion_destruir(publicacion_t* publicacion) {
    if (!publicacion) return;
    free(publicacion->texto);
    abb_destruir(publicacion->likes);
    free(publicacion);
}

