#include "algogram.h"
#include "usuario.h"
#include "./tdas_aux/hash.h"

struct algogram {
    hash_t* usuarios;
    usuario_t* usuario_actual;
};

// PRIMITIVAS DE ALGOGRAM
algogram_t* algogram_crear(hash_t* usuarios) {
    algogram_t* algogram = malloc(sizeof(algogram_t));
    if (!algogram) return NULL;
    algogram->usuarios = usuarios;
    algorgram->usuario_actual = NULL;
    return algogram;
}