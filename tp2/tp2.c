#define _POSIX_C_SOURCE 200809L

#include "tdas_aux/hash.h"
#include "algogram.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void _usuario_destruir(void* usuario){
    usuario_destruir(usuario);
}

hash_t* leer_archivo_usuarios(FILE *stream) {
    hash_t* hash_usuarios = hash_crear(_usuario_destruir);

    char* linea = NULL;
    size_t tam;

    int i = 0;
    while ((getline(&linea, &tam, stream)) != EOF) {
        linea[strlen(linea)-1] = '\0';
        usuario_t* usuario = usuario_crear(linea, i);
        printf("%s\n", usuario_get_nombre(usuario));
        hash_guardar(hash_usuarios, usuario_get_nombre(usuario), usuario);
        i++;
    }
    free(linea);

    return hash_usuarios;
}

bool login_usuario(algogram_t* algogram, hash_t* hash_usuarios, char* nombre_usuario) {
    if (algogram_get_usuario_actual(algogram)) {
        fprintf(stderr, "Error: Ya habia un usuario loggeado\n");
        return false;    
    }

    printf("%s", nombre_usuario);
    printf("%d", hash_pertenece(hash_usuarios, "Echi"));
    usuario_t* usuario = hash_obtener(hash_usuarios, nombre_usuario);
    if (usuario == NULL) {
        fprintf(stderr, "Error: usuario no existente\n");
        return false;
    }

    algogram_login(algogram, usuario);
    return true;
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
    hash_t* hash_usuarios = leer_archivo_usuarios(mi_archivo);
    algogram_t* algogram = algogram_crear(hash_usuarios);

    while(true){
        char* input = NULL;
        size_t len = 0;
        getline(&input, &len, stdin);
        input[strlen(input)-1] = '\0';
        if(!strcmp(input , "login")){
            getline(&input, &len, stdin);
            input[strlen(input)-1] = '\0';
            login_usuario(algogram, hash_usuarios, input);
        }
    }
    return 0;
}


