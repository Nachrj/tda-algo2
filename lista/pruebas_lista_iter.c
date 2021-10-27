#include "lista.h"
#include "testing.h"
#include "pila.h"
#include <stdio.h>

static void pruebas_lista_vacia(){
    printf("--- INICIO DE PRUEBAS lista VACIA ---\n");

    // Declaro variables a utilizar
    lista_t* lista = lista_crear();

    // Inicio pruebas
    print_test("Creo la lista exitosamente",lista != NULL && lista_ver_primero(lista) == NULL && lista_ver_ultimo(lista) == NULL);
    print_test("Al crear la lista, esta vacia", lista_esta_vacia(lista));

    // Destrullo la lista creada
    lista_destruir(lista,NULL);
}

static void pruebas_lista_null() {
    printf("\n--- INICIO DE PRUEBAS LISTA NULL ---\n");

    // Declaro las variables a utilizar
    lista_t* lista = lista_crear();

    // Inicio pruebas
    print_test("Se inserta NULL a lista vacia, devuelve true", lista_insertar_ultimo(lista, NULL));
    print_test("Se pregunta si la lista esta vacia, devuelve false", !lista_esta_vacia(lista));
    print_test("Se inserta NULL a lista con un elemento, devuelve true", lista_insertar_ultimo(lista, NULL));
    print_test("Se inserta un NULL al final de una lista, devuelve true", lista_insertar_ultimo(lista, NULL));
    print_test("Se pregunta cual es el primero en la lista, devuelve NULL", !lista_ver_primero(lista));
    lista_borrar_primero(lista);
    lista_borrar_primero(lista);
    lista_borrar_primero(lista);
    print_test("Lista esta vacia, devuelve true", lista_esta_vacia(lista));

    // Destrullo la lista creada
    lista_destruir(lista,NULL);
}

static void pruebas_lista_insertar(void) {
    printf("\n--- INICIO DE PRUEBAS LISTA INSERTAR ---\n");

    // Declaro las variables a utilizar
    lista_t *lista = lista_crear();

    size_t num = 3;
    size_t num2 = 10;
    char letra = 'a';
    bool boolean = true;
    double pi = 3.14;

    // Test de insertar
    print_test("Se inserta un booleano, devuelve true", lista_insertar_ultimo(lista, &boolean));
    print_test("Se inserta un char, devuelve true", lista_insertar_ultimo(lista, &letra));
    print_test("Se inserta un numero, devuelve true", lista_insertar_ultimo(lista, &num));
    
    print_test("Se borra primero, deuvele booleano true", *(bool*)lista_borrar_primero(lista));

    print_test("Se inserta otro numero, devuelve true", lista_insertar_ultimo(lista, &num2));

    print_test("Se borra primero, devuelve el char", *(char*)lista_borrar_primero(lista) == letra);

    print_test("Se pregunta si esta vacia, devuelve false", !lista_esta_vacia(lista));

    print_test("Se pregunta cual es el primero, devuelve la primera letra", *(size_t*)lista_ver_primero(lista) == num);

    print_test("Se inserta un double, devuelve true", lista_insertar_ultimo(lista, &pi));

    print_test("Se borra primero, devuelve el primer numero", *(size_t*)lista_borrar_primero(lista) == num);
    print_test("Se borra primero, devuelve el ultimo numero", *(size_t*)lista_borrar_primero(lista) == num2);
    print_test("Se borra primero, devuelve el double pi", *(double*)lista_borrar_primero(lista) == pi);

    print_test("Se pregunta si la lista esta vacia, devuelve true", lista_esta_vacia(lista));
    print_test("Se intenta borrar primero, devuelve NULL", !lista_borrar_primero(lista));
    print_test("Se intenta ver primero, devuelve NULL", !lista_ver_primero(lista));

    // Destrullo la lista creada
    lista_destruir(lista,NULL);

}

static void prueba_lista_insertar_volumen(void) {
    printf("\n--- INICIO PRUEBA DE VOLUMEN ---\n");

    // Defino cantidad de veces a insertar
    size_t n = 1000;

    // Declaro las variables a utilizar
    lista_t *lista = lista_crear();
    size_t vector[n];

    // Pruebo insertar n cantidad de veces
    bool ok = true;
    for (size_t i = 0; i < n; i++) {
        // Si algún elemento no se pudo insertar, ok = false.
        vector[i] = i;
        ok &= lista_insertar_ultimo(lista, &vector[i]);
    }
    print_test("Se pudo insertar todas las veces", ok);

    // Pruebo borrar todo
    ok = true;
    for (size_t j = 0; j < n; j++) {
        size_t *dato = lista_ver_primero(lista);
        ok &= (dato == lista_borrar_primero(lista) && dato == &vector[j]);
    }
    print_test("Se pudo borrar todos", ok);

    // Destrullo la lista creada
    lista_destruir(lista, NULL);
}

void _pila_destruir(void *pila) {
    pila_destruir(pila);
}

static void prueba_pila_dentro_de_lista(void) {
    printf("\n--- INICIO PRUEBA DE PILA DENTRO DE lista ---\n");

    // Declaro las variables a utilizar
    pila_t *pila = pila_crear();
    pila_t *pila2 = pila_crear();
    lista_t *lista = lista_crear();

    size_t num = 20;
    char letra = 'f'; 

    // Inicio pruebas pila dentro de lista
    print_test("Pruebo insertar pila, devuelve true", lista_insertar_ultimo(lista, pila));
    print_test("Pruebo insertar pila, devuelve true", lista_insertar_ultimo(lista, pila2));

    print_test("Pruebo borrar, devuelve pila", lista_borrar_primero(lista) == pila);
    pila_destruir(pila);
    print_test("Veo si esta vacia con pila adentro, devuelve false", !lista_esta_vacia(lista));
    lista_destruir(lista, _pila_destruir);

    lista = lista_crear();

    // Inicio pruebas
    print_test("Pregunto si esta vacia, devuelve true", lista_esta_vacia(lista));
    print_test("Pruebo insertar numero, devuelve true", lista_insertar_ultimo(lista, &num));
    print_test("Pruebo insertar letra, devuelve true", lista_insertar_ultimo(lista, &letra));
    lista_destruir(lista, NULL);
}

bool multiplicar_dato(void *dato, void* multiplicador) {
    if (!dato) return NULL;

    *(int*)dato *= *(int*)multiplicador;
    return true;
}

bool mayor_que_2(void* dato, void* extra){
    if (!dato) return NULL;
    if(*(int*)dato > 2){
        *(int*)extra = *(int*)dato;
        return false;
    }
    return true;
}

static void pruebas_iteradores_internos(){
    printf("\n--- INICIO DE PRUEBAS ITERAR INTERNAMENTE ---\n");

    // Declaro las variables a utilizar
    int multi = 2;
    int n = 5;
    int vector[n];
    int vector_correcto[n];
    int num_mayor_que_2;
    lista_t *lista = lista_crear();

    // Creo el vector correcto y la lista

    for (int i = 0; i < n; i++) {
        vector[i] = i+1;
        vector_correcto[i] = (i+1)*multi;
        lista_insertar_ultimo(lista, &vector[i]);
    }

    // Iterar y cortar

    lista_iterar(lista, mayor_que_2, &num_mayor_que_2);
    print_test("Se pudo iterar y cortar en condicion",num_mayor_que_2 > 2 && num_mayor_que_2 == vector[2]);
    
    // Iterar y multiplicar, chequeo con el vector correcto

    lista_iterar(lista, multiplicar_dato, &multi);
    
    bool ok = true;
    for (int j = 0; j < n; j++) {
        ok &= (*(int*)lista_borrar_primero(lista) == (vector_correcto[j]));
    }

    print_test("Se pudo iterar y multiplicar todos los valores.", ok);


    // Destrullo la lista creada
    lista_destruir(lista, NULL);
}


static void pruebas_iter_externos_lista_vacia(){
    printf("\n--- INICIO DE PRUEBAS ITERADORES EXTERNOS LISTA VACIA ---\n");

    // Declaro variables a utilizar
    lista_t* lista = lista_crear();
    lista_iter_t* iter = lista_iter_crear(lista);

    print_test("Ver actual en lista vacia devuelve NULL", lista_iter_ver_actual(iter) == NULL);
    print_test("Avanzar devuelve false",!lista_iter_avanzar(iter));
    print_test("Borrar con lista vacia devuelve NULL",!lista_iter_borrar(iter));
    print_test("Esta al final cuando esta vacia",lista_iter_al_final(iter));
   
    
    lista_destruir(lista,NULL);
    lista_iter_destruir(iter);
}

static void pruebas_iter_externos_funciones_basicas(){
    printf("\n--- INICIO DE PRUEBAS ITERADORES EXTERNOS LISTA COMUN ---\n");

    // Declaro variables a utilizar
    lista_t* lista = lista_crear();
    lista_iter_t* iter = lista_iter_crear(lista);
    int dato1 = 1;
    int dato2 = 2;
    int dato3 = 3;
    int dato4 = 4;
    
    // []
    print_test("Insertar en lista vacia, efectivamente inserta en el primero y ultimo", lista_iter_insertar(iter,&dato1) && lista_ver_ultimo(lista) == &dato1 && lista_ver_primero(lista) == &dato1);
    // [ 1 ]
    print_test("Insertar cuando iterador esta al final inserta en el final de la lista efectivamente", lista_iter_avanzar(iter) && lista_iter_insertar(iter,&dato2) && lista_ver_ultimo(lista) == &dato2);
    // [ 1 , 2 ]
    lista_iter_avanzar(iter);
    lista_iter_insertar(iter,&dato3);
    lista_iter_destruir(iter);
    // [ 1 , 2 , 3 ]
    lista_iter_t* iter2 = lista_iter_crear(lista);
    lista_iter_avanzar(iter2); // 2 es el actual
    print_test("Insertar en el medio lo hace en la posicion correcta", lista_iter_insertar(iter2,&dato4) && lista_iter_ver_actual(iter2) == &dato4 && lista_iter_avanzar(iter2) && lista_iter_ver_actual(iter2) == &dato2);
    // [ 1 , 4 , 2 , 3]
    print_test("Borrar en el medio lo borra de la lista correctamente", lista_iter_borrar(iter2) == &dato2 && lista_iter_ver_actual(iter2) == &dato3);
    // [ 1 , 4 , 3 ]
    lista_iter_destruir(iter2);
    lista_iter_t* iter3 = lista_iter_crear(lista);
    print_test("Borrar el primero actualiza bien la lista", lista_iter_borrar(iter3) == &dato1 && lista_ver_primero(lista) == &dato4);
    // [ 4 , 3]
    lista_iter_avanzar(iter3); // 3 es el actual
    print_test("Borrar el ultimo actualiza bien la lista", lista_iter_borrar(iter3) == &dato3 && lista_ver_ultimo(lista) == &dato4);
    // [ 4 ]
    lista_iter_destruir(iter3);
    lista_destruir(lista,NULL);
}

static void pruebas_iter_externos_volumen(){
    printf("\n--- INICIO DE PRUEBAS ITERADORES EXTERNOS VOLUMEN ---\n");

    // Declaro las variables a utilizar

    size_t n = 1000;
    lista_t *lista = lista_crear();
    size_t vector[n];

    for (size_t i = 0; i < n; i++) {
        vector[i] = i;
        lista_insertar_ultimo(lista, &vector[i]);
    }

    // Creo el iterador
    lista_iter_t* iter = lista_iter_crear(lista);
    size_t index = 0;
    bool ok = true;
    while(!lista_iter_al_final(iter)){
        ok &= (*(size_t*)lista_iter_ver_actual(iter) == vector[index]);
        lista_iter_avanzar(iter);
        index++;
    }
    print_test("Iterador recorre la lista correctamente", ok);


    lista_iter_t* iter2 = lista_iter_crear(lista);
    while(!lista_iter_al_final(iter2)){
        lista_iter_borrar(iter2);
    }
    print_test("Borro toda una lista y queda vacia", lista_iter_al_final(iter2) && lista_esta_vacia(lista) && lista_ver_ultimo(lista) == NULL && lista_ver_primero(lista) == NULL);
    lista_iter_destruir(iter);
    lista_iter_destruir(iter2);
    lista_destruir(lista,NULL);
}

void pruebas_lista_estudiante(){
    pruebas_lista_vacia();
    pruebas_lista_null();
    pruebas_lista_insertar();
    prueba_lista_insertar_volumen();
    pruebas_iteradores_internos();
    pruebas_iter_externos_lista_vacia();
    pruebas_iter_externos_funciones_basicas();
    pruebas_iter_externos_volumen();
    prueba_pila_dentro_de_lista();
}
#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.
int main(void){
    pruebas_lista_estudiante();  
    return failure_count() > 0;
}
#endif
