#include <stdbool.h>
#include <stddef.h>
#include "usuario.h"
#include "publicacion.h"
#include "./tdas_aux/hash.h"
struct algogram;

typedef struct algogram algogram_t;
algogram_t* algogram_crear(hash_t* usuarios);
bool postear_publicacion(algogram_t* algogram, usuario_t* usuario_creador, publicacion_t* publicacion);
bool likear_publicacion(algogram_t* algogram, usuario_t* usuario, publicacion_t* publicacion);
void algogram_destruir(algogram_t* algogram);