#include <stdbool.h>
#include <stddef.h>
#include "usuario_publi.h"
#include "hash.h"
#include "lista.h"
#include "heap.h"

/* Tipo utilizado para Algogram*/
typedef struct algogram algogram_t;

/*
 * Crea una red algogram dado un hash de usuarios (ver usuario.h)
 * Devuelve un puntero a la red que tiene que ser destruido por
 * algogram_destruir()
 */
algogram_t* algogram_crear(hash_t* usuarios);

/*
 * Loguea al usuario dado (ver usuario.h) en la red
 * si el usuario no fue "registrado" devuelve FALSE.
 * Post: El usuario fue logueado y ahora interactua con la red
 */
bool algogram_login(algogram_t* algogram, usuario_t* usuario);

/*
 * Desloguea al usuario de la red.
 * Pre: Hay un usuario logueado
 */
bool algogram_logout(algogram_t* algogram);

/*
 * Postea una publicacion en la red segun el usuario
 * que este logueado. 
 * Pre: Hay un usuario logueado
 */
bool postear_publicacion(algogram_t* algogram, char* texto_publicacion);

/*
 * Muestra el siguiente post en la feed del usuario logueado.
 * Pre: Hay un usuario logueado.
 */
void algogram_ver_proximo(algogram_t* algogram);

/*
 * Dado el ID de una publicacion (ver publicacion.h) 
 * le otorga un like del usuario logueado
 * Pre: Hay un usuario logueado y existe el post
 */
bool algogram_likear_publicacion(algogram_t* algogram, int id);

/*
 * Dado el ID de una publicacion muestra los likes que tiene
 * y quienes la likearon.
 * Pre: Hay un usuario logueado y existe el post.
 */
bool algogram_mostrar_likes(algogram_t* algogram, int id);

/*
 * Devuelve un puntero del tipo usuario del usuario que
 * esta logueado actualmente.
 */
usuario_t* algogram_get_usuario_actual(algogram_t* algogram);

/*
 * Destruye la red incluyendo todas las publicaciones existentes
 * y todos sus usuarios.
 * Post: Se destruyo la red.
 */
void algogram_destruir(algogram_t* algogram);