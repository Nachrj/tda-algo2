#include "lista.h"
#include "testing.h"



static void pruebas_lista_vacia(){
    lista_t* lista = lista_crear();
    print_test("Creo la lista exitosamente",lista != NULL && lista_ver_primero(lista) == NULL && lista_ver_ultimo(lista) == NULL);
    print_test("Al crear la lista esta vacia", lista_esta_vacia(lista));

    lista_destruir(lista,NULL);
}
/*
static void pruebas_basicas_iteradores_externos(){
    lista_t* lista = lista_crear();
    lista_iter_t* iter = lista_iter_crear(lista);
    int a = 1;
    int b = 2;
    int c = 3;
    lista_insertar_ultimo(lista,&a);
    lista_insertar_ultimo(lista,&b);
    lista_insertar_ultimo(lista,&c);
    

    lista_destruir(lista,NULL);
    lista_iter_destruir(iter);
}
*/

static void pruebas(){
    pruebas_lista_vacia();
}
int main(void){
    pruebas();  
    return 0;
}
