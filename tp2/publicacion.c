#include "usuario.h"
#include "./tdas_aux/abb.h"

struct publicacion {
    usuario_t* usuario_creador;
    abb_t* likes;
    char* texto;
    int id;
}

publicacion_t* publicacion_crear(usuario_t* usuario_creador, char* texto, int id) {
    publicacion_t* publicacion = malloc(sizeof(publicacion_t));
    if (!publicacion) return NULL;
    publicacion->usuario_creador = usuario_creador;
    publicacion->likes = abb_crear(strcmp, NULL);
    publicacion->texto = strdup(texto);
    publicacion->id = id;
    return publicacion;
}