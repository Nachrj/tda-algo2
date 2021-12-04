#include "algogram.h"
#include "usuario_publi.h"
#include "hash.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct algogram {
    hash_t* usuarios;
    usuario_t* usuario_actual;
    hash_t* hash_publicaciones;
};

void _publicacion_destruir(void* publicacion){
    publicacion_destruir(publicacion);
}
// PRIMITIVAS DE ALGOGRAM
algogram_t* algogram_crear(hash_t* usuarios) {
    algogram_t* algogram = malloc(sizeof(algogram_t));
    if (!algogram) return NULL;
    algogram->usuarios = usuarios;
    algogram->usuario_actual = NULL;
    algogram->hash_publicaciones = hash_crear(_publicacion_destruir);
    return algogram;
}

int encontrar_afinidad(usuario_t* usuario_creador, usuario_t* usuario_actual) {
    return 1/(abs(usuario_get_id(usuario_creador) - usuario_get_id(usuario_actual)));
}

bool postear_publicacion(algogram_t* algogram, usuario_t* usuario_creador, publicacion_t* publicacion) {
    if (!usuario_creador || !publicacion || !algogram) return false;
    
    // Agregar la publicacion a la lista de publicaciones totales
    hash_guardar(algogram->hash_publicaciones, publicacion_get_texto(publicacion), publicacion);

    // Agregar la publicacion a la feed de cada usuario
    hash_iter_t* iter = hash_iter_crear(algogram->usuarios);

    while (!hash_iter_al_final(iter)) {
        usuario_t* usuario_actual = (usuario_t*)hash_iter_ver_actual(iter);
        // Encuentro primero la afinidad entre los usuarios
        int afinidad = encontrar_afinidad(usuario_creador, usuario_actual);

        if (strcmp(usuario_get_nombre(usuario_actual), usuario_get_nombre(usuario_creador)) != 0) {
            // Encolo la publicación con la afinidad entre usuarios (publicacion_afinidad) para compararlo.
            postear_al_feed(usuario_creador, publicacion, afinidad);
        }
        hash_iter_avanzar(iter);
    }

    hash_iter_destruir(iter);
    return true;
}




bool likear_publicacion(algogram_t* algogram, usuario_t* usuario, publicacion_t* publicacion) {
    if (!usuario || !publicacion || !algogram) return false;

    
    /* Conseguimos el post y lo likeamos
    lista_iter_t* iter = lista_iter_crear(algogram->lista_publicaciones);

    while (!lista_iter_al_final(iter)) {
        publicacion_t* publicacion = lista_iter_ver_actual(iter);
        if (publicacion->id == id_publicacion) {
            publicacion->cant_likes++;
            printf("Post likeado\n");
            return agregar_usuario_likes(publicacion, usuario);
        }
    }*/

    // En el caso de que no guarde el usuario, devolvemos false.
    printf("Error: Usuario no loggeado o Post inexistente\n");
    return false;
}

void algogram_destruir(algogram_t* algogram) {
    if (!algogram) return;
    hash_destruir(algogram->usuarios);
    usuario_destruir(algogram->usuario_actual);
    hash_destruir(algogram->hash_publicaciones);
    free(algogram);
}