#define _POSIX_C_SOURCE 200809L

#include "hash.h"
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
        if (linea[strlen(linea)-1] == '\n'){
            linea[strlen(linea)-1] = '\0';
        }
        usuario_t* usuario = usuario_crear(linea, i);
        hash_guardar(hash_usuarios, linea, usuario);
        i++;
    }
    free(linea);

    return hash_usuarios;
}

bool login_usuario(algogram_t* algogram, hash_t* hash_usuarios, char* nombre_usuario) {
    if (algogram_get_usuario_actual(algogram)) {
        printf("%s", "Error: Ya habia un usuario loggeado\n");
        return false;    
    }

    usuario_t* usuario = hash_obtener(hash_usuarios, nombre_usuario);
    if (usuario == NULL) {
        printf("%s", "Error: usuario no existente\n");
        return false;
    }

    algogram_login(algogram, usuario);
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("%s", "Error: Cantidad erronea de parametros\n");
        return -1;
    }
    FILE *mi_archivo = fopen(argv[1], "r");
    if (mi_archivo == NULL) {
        printf("%s", "Error: archivo fuente inaccesible\n");
        return -1;
    }
    hash_t* hash_usuarios = leer_archivo_usuarios(mi_archivo);
    algogram_t* algogram = algogram_crear(hash_usuarios);

    char* input = NULL;
    size_t len = 0;
    while(getline(&input, &len, stdin) != EOF){
        
        input[strlen(input)-1] = '\0';
        if(!strcmp(input , "login")){
            getline(&input, &len, stdin);
            input[strlen(input)-1] = '\0';
            login_usuario(algogram, hash_usuarios, input);
        }
        if(!strcmp(input, "logout")){
            if (!algogram_logout(algogram)) {
                printf("%s", "Error: no habia usuario loggeado\n");
            }
        }
        if(!strcmp(input, "publicar")){
            getline(&input, &len, stdin);
            input[strlen(input)-1] = '\0';
            postear_publicacion(algogram, input);
        }
        if(!strcmp(input, "ver_siguiente_feed")) {
            algogram_ver_proximo(algogram);
        }
        if(!strcmp(input, "likear_post")){
            getline(&input, &len, stdin);
            input[strlen(input)-1] = '\0';
            algogram_likear_publicacion(algogram, atoi(input));
        }
        if(!strcmp(input,"mostrar_likes")){
            getline(&input, &len, stdin);
            input[strlen(input)-1] = '\0';
            algogram_mostrar_likes(algogram, atoi(input));
        }
    }
    free(input);
    algogram_destruir(algogram);
    fclose(mi_archivo);
    return 0;
}


