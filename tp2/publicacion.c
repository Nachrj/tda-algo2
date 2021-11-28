#include "usuario.h"
#include "./tdas_aux/abb.h"

struct publicacion {
    usuario_t* usuario_creador;
    abb_t* likes;
    char* texto;
    int id;
}