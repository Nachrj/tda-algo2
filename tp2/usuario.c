#include "publicacion.h"
#include "./tda_aux/heap.h"

struct usuario {
    char* nombre;
    heap_t* feed;
    int id;
};