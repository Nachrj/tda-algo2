#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "./tdas_aux/heap.h"

struct usuario;
typedef struct usuario usuario_t;

struct publicacion_afinidad;
typedef struct publicacion_afinidad publicacion_afinidad_t;

usuario_t* usuario_crear(char* nombre, int id);
void* ver_proximo_post_feed(usuario_t* usuario);
void usuario_destruir(usuario_t* usuario);
char* usuario_get_nombre(usuario_t* usuario);
int usuario_get_id(usuario_t* usuario);
heap_t* usuario_get_feed(usuario_t* usuario);