#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "hash.h"
#include "lista.h"
#include "heap.h"

/* Tipo utilizado para el usuario */
typedef struct usuario usuario_t;

/*
 * Crea un usuario recibiendo por parametro el Nombre y el ID.
 * Devuelve un puntero al usuario
 * que tiene que ser destruido con usuario_destruir()
 */
usuario_t* usuario_crear(char* nombre, int id);

/*
 * Dado un usuario, devuelve una publicacion de su feed
 * (CUIDADO: Devuelve del tipo void*)
 */
void* ver_proximo_post_feed(usuario_t* usuario);

/*
 * Devuelve el nombre del usuario
 * Pre: El usuario fue creado
 */
char* usuario_get_nombre(usuario_t* usuario);

/*
 * Devuelve el ID del usuario
 * Pre: El usuario fue creado
 */
int usuario_get_id(usuario_t* usuario);

/*
 * Destruye el usuario dado, incluyendo
 * sus contenidos.
 * Post: Se destruyo la feed del usuario y el usuario
 * mismo.
 */
void usuario_destruir(usuario_t* usuario);
