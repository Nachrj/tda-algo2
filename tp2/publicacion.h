#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "usuario.h"
struct publicacion;
typedef struct publicacion publicacion_t;

//PRIMITIVAS PUBLICACIONES
publicacion_t* publicacion_crear(usuario_t* usuario_creador, char* texto, int id);
bool agregar_usuario_likes(publicacion_t* publicacion, usuario_t* usuario);
int publicacion_cantidad_likes(publicacion_t* publicacion);
bool imprimir_likes(const char* clave, void* dato, void* extra);
void mostrar_likes(publicacion_t* publicacion);
int publicacion_get_id(publicacion_t* publicacion);
void publicacion_destruir(publicacion_t* publicacion);