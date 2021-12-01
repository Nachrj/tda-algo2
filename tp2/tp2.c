#define _POSIX_C_SOURCE 200809L

#include "./tdas_aux/hash.h"
#include "usuario.h"
#include "algogram.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error: Cantidad erronea de parametros\n");
        return -1;
    }
    FILE *mi_archivo = fopen(argv[1], "r");
    if (mi_archivo == NULL) {
        fprintf(stderr, "Error: archivo fuente inaccesible\n");
        return -1;
    }
    algogram_t* algogram = algogram_crear(leer_arrchivo_usuarios(mi_archivo);
}

hash_t* leer_arrchivo_usuarios(FILE *stream) {
    hash_t* hash_usuarios = hash_crear(usuario, usuario_destruir);

    char* linea = NULL;
    size_t tam;

    int i = 0;
    while ((getline(&linea, &linea, stream)) != EOF) {
        hash_guardar(hash_usuarios, usuario_crear(linea, i));
        i++;
    }

    return hash_usuarios
}
