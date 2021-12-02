#define _POSIX_C_SOURCE 200809L

#include "./tdas_aux/hash.h"
#include "usuario.h"
#include "algogram.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

hash_t* leer_archivo_usuarios(FILE *stream) {
    hash_t* hash_usuarios = hash_crear(usuario_destruir);

    char* linea = NULL;
    size_t tam;

    int i = 0;
    while ((getline(&linea, &tam, stream)) != EOF) {
        usuario_t* usuario = usuario_crear(linea, i);
        hash_guardar(hash_usuarios, usuario_get_nombre(usuario), usuario);
        i++;
    }

    return hash_usuarios;
}

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
    algogram_t* algogram = algogram_crear(leer_archivo_usuarios(mi_archivo));
    while(true){
        char* input;
        fgets(input, 50, stdin);
    }
    return 0;
}


