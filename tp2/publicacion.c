#include "usuario.h"
#include "./tdas_aux/abb.h"

struct publicacion {
    usuario_t* usuario_creador;
    abb_t* likes;
    int cant_likes;
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

int publicacion_cantidad_likes(publicacion_t* publicacion) {
    return publicacion->cant_likes;
}

// A CABIAR ESTA FUNCION PARA QUE PRINTEE LOS NOMBRES
bool imprimir_likes(const char* clave, void* dato, void* extra) {
    printf("%d\n", *(char*)clave);
    return true;
}

void mostrar_likes(publicacion_t* publicacion) {
    printf("El post tiene %d likes:\n",publicacion->cant_likes);
    abb_in_order(publicacion->likes, imprimir_likes, NULL);
    printf("\n");
}
