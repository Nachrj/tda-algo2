#include <stdbool.h>
#include <stddef.h>
#include "usuario.h"
#include "publicacion.h"
#include "hash.h"
#include "lista.h"
#include "heap.h"
struct algogram;

typedef struct algogram algogram_t;
algogram_t* algogram_crear(hash_t* usuarios);
bool postear_publicacion(algogram_t* algogram, char* texto_publicacion);
void algogram_ver_proximo(algogram_t* algogram);
bool likear_publicacion(algogram_t* algogram, int id);
bool algogram_login(algogram_t* algogram, usuario_t* usuario);
bool algogram_logout(algogram_t* algogram);
usuario_t* algogram_get_usuario_actual(algogram_t* algogram);
void algogram_destruir(algogram_t* algogram);