#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "usuario.h"
#include "hash.h"
#include "lista.h"
#include "heap.h"

/* Tipo utilizado para la publicacion*/
typedef struct publicacion publicacion_t;


//PRIMITIVAS PUBLICACIONES

/*
 * Crea una publicacion recibiendo un USUARIO (ver usuario.h),
 * el texto de la publicacion y el ID. Devuelve un puntero
 * a la publicacion que tiene que ser destruido con publicacion_destruir()
 */
publicacion_t* publicacion_crear(usuario_t* usuario_creador, char* texto, int id);

/*
 * Agrega un like del usuario dado a la publicacion dada.
 * Si el usuario ya likeo el post devuelve TRUE
 */
bool agregar_usuario_likes(publicacion_t* publicacion, usuario_t* usuario);

/*
 * Dada una publicacion, muestra cuantos likes tiene
 * y quien lo likeo.
 */
void mostrar_likes(publicacion_t* publicacion);

/*
 * Devuelve la cantidad de likes que tiene
 * la publicacion dada.
 * Pre: La publicacion fue creada.
 */
int publicacion_get_likes(publicacion_t* publicacion);

/*
 * Devuelve el ID de
 * la publicacion dada.
 * Pre: La publicacion fue creada.
 */
int publicacion_get_id(publicacion_t* publicacion);

/*
 * Devuelve un puntero de tipo usuario (ver usuario.h), creador de
 * la publicacion dada.
 * Pre: La publicacion fue creada.
 */
usuario_t* publicacion_get_usuario_creador(publicacion_t* publicacion);
/*
 * Devuelve el texto de
 * la publicacion dada.
 * Pre: La publicacion fue creada.
 */
char* publicacion_get_texto(publicacion_t* publicacion);

/*
 * Destruye la publicacion dada, incluyendo
 * sus contenidos.
 * Post: Se destruyeron los likes de la publicacion
 * NOTA: NO destruye el usuario creador, en caso de querer
 * destruir un usuario ver usuario.h
 */
void publicacion_destruir(publicacion_t* publicacion);