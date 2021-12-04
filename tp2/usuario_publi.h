#include "publicacion.h"
#include "usuario.h"
struct publicacion_afinidad;
typedef struct publicacion_afinidad publicacion_afinidad_t;

publicacion_afinidad_t* publicacion_afinidad_crear(publicacion_t* publicacion, int afinidad);
void publicacion_afinidad_destruir(publicacion_afinidad_t* publicacion_afinidad);
void postear_al_feed(usuario_t* usuario, publicacion_t* publicacion, int afinidad);