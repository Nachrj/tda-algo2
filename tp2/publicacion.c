#define _POSIX_C_SOURCE 200809L
#include "usuario.h"
#include "abb.h"
#include "publicacion.h"
#include <string.h>

struct publicacion {
    usuario_t* usuario_creador;
    abb_t* likes;
    int cant_likes;
    char* texto;
    char* id;
};

// PRIMITIVAS PUBLICACIONES
publicacion_t* publicacion_crear(usuario_t* usuario_creador, char* texto, char* id) {
    publicacion_t* publicacion = malloc(sizeof(publicacion_t));
    if (!publicacion) return NULL;
    publicacion->usuario_creador = usuario_creador;
    publicacion->likes = abb_crear(strcmp, free);
    publicacion->texto = strdup(texto);
    publicacion->id = strdup(id);
    return publicacion;
}

bool agregar_usuario_likes(publicacion_t* publicacion, usuario_t* usuario) {
    if (!publicacion || !usuario) return false;
    if (abb_guardar(publicacion->likes, strdup(usuario_get_nombre(usuario)), usuario)) {
        publicacion->cant_likes++;
        return true;
    }
    return false;
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

char* publicacion_get_id(publicacion_t* publicacion){
    return publicacion->id;
}

char* publicacion_get_texto(publicacion_t* publicacion){
    return publicacion->texto;
}
void publicacion_destruir(publicacion_t* publicacion) {
    if (!publicacion) return;
    free(publicacion->texto);
    abb_destruir(publicacion->likes);
    free(publicacion->id);
    free(publicacion);
}

