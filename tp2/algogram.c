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
    return abs(usuario_get_id(usuario_creador) - usuario_get_id(usuario_actual));
}

bool postear_publicacion(algogram_t* algogram, char* texto_publicacion) {
    if (!texto_publicacion || !algogram) return false;
    if (!algogram->usuario_actual) {
        printf("%s\n", "Error: no habia usuario loggeado");
        return false;
    }
    publicacion_t* publicacion = publicacion_crear(algogram->usuario_actual, texto_publicacion, (int)hash_cantidad(algogram->hash_publicaciones));

    // Agregar la publicacion a la lista de publicaciones totales
    hash_guardar(algogram->hash_publicaciones, publicacion_get_id(publicacion), publicacion);

    // Agregar la publicacion a la feed de cada usuario
    hash_iter_t* iter = hash_iter_crear(algogram->usuarios);
    
    while (!hash_iter_al_final(iter)) {
        usuario_t* usuario = (usuario_t*)hash_obtener(algogram->usuarios, hash_iter_ver_actual(iter));
        // Encuentro primero la afinidad entre los usuarios
        if (!strcmp(usuario_get_nombre(usuario), usuario_get_nombre(algogram->usuario_actual))) {
            hash_iter_avanzar(iter);
            continue;
        }
        int afinidad = encontrar_afinidad(algogram->usuario_actual, usuario);

        if (strcmp(usuario_get_nombre(usuario), usuario_get_nombre(algogram->usuario_actual)) != 0) {
            // Encolo la publicación con la afinidad entre usuarios (publicacion_afinidad) para compararlo.
            postear_al_feed(usuario, publicacion, afinidad);
        }
        hash_iter_avanzar(iter);
    }

    hash_iter_destruir(iter);
    return true;
}

void algogram_ver_proximo(algogram_t* algogram) {
    if (!algogram) return;
    if (!algogram->usuario_actual) {
        printf("%s\n", "Error: no habia usuario loggeado");
        return;
    }
    printf("%s\n", ver_proximo_post_feed(algogram->usuario_actual));
}

bool likear_publicacion(algogram_t* algogram, int id) {
    if (!id || !algogram) return false;

    publicacion_t* publi = hash_obtener(algogram->hash_publicaciones, publicacion_get_id(publicacion));
    if (!publi || !algogram->usuario_actual) {
        printf("Error: Usuario no loggeado o Post inexistente\n");
        return false;
    }
    printf("Post likeado\n");
    return agregar_usuario_likes(publi, algogram->usuario_actual);
}

bool algogram_login(algogram_t* algogram, usuario_t* usuario) {
    if (!algogram || !usuario) return false;
    algogram->usuario_actual = usuario;
    printf("Hola %s\n", usuario_get_nombre(usuario));
    return true;
}

bool algogram_logout(algogram_t* algogram) {
    if (!algogram->usuario_actual) return false;
    algogram->usuario_actual = NULL;
    printf("Adios\n");
    return true;
}

usuario_t* algogram_get_usuario_actual(algogram_t* algogram) {
    if (!algogram) return NULL;
    return algogram->usuario_actual;
}

void algogram_destruir(algogram_t* algogram) {
    if (!algogram) return;
    hash_destruir(algogram->usuarios);
    usuario_destruir(algogram->usuario_actual);
    hash_destruir(algogram->hash_publicaciones);
    free(algogram);
}