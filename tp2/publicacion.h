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
struct publicacion;
typedef struct publicacion publicacion_t;


//PRIMITIVAS PUBLICACIONES
publicacion_t* publicacion_crear(usuario_t* usuario_creador, char* texto, char* id);
bool agregar_usuario_likes(publicacion_t* publicacion, usuario_t* usuario);
int publicacion_cantidad_likes(publicacion_t* publicacion);
bool imprimir_likes(const char* clave, void* dato, void* extra);
void mostrar_likes(publicacion_t* publicacion);
char* publicacion_get_id(publicacion_t* publicacion);
char* publicacion_get_texto(publicacion_t* publicacion);
void publicacion_destruir(publicacion_t* publicacion);