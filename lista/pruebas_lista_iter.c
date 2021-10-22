#include "lista.h"
#include "testing.h"



static void pruebas_lista_vacia(){
    printf("--- INICIO DE PRUEBAS COLA VACIA ---\n");

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

    // Defino cantidad de veces a encolar
    size_t n = 1000;

    // Declaro las variables a utilizar
    lista_t *lista = lista_crear();
    size_t vector[n];

    // Pruebo encolar n cantidad de veces
    bool ok = true;
    for (size_t i = 0; i < n; i++) {
        // Si algún elemento no se pudo encolar, ok = false.
        vector[i] = i;
        ok &= lista_insertar_ultimo(lista, &vector[i]);
    }
    print_test("Se pudo insertar todas las veces", ok);

    // Pruebo desencolar todo
    ok = true;
    for (size_t j = 0; j < n; j++) {
        size_t *dato = lista_ver_primero(lista);
        ok &= (dato == lista_borrar_primero(lista) && dato == &vector[j]);
    }
    print_test("Se pudo borrar todos", ok);

    // Destrullo la cola creada
    lista_destruir(lista, NULL);
}

bool multiplicar_dato(void *dato, void* multiplicador) {
    if (!dato) return NULL;

    *(int*)dato *= *(int*)multiplicador;
    return true;
}

static void pruebas_iteradores_internos(){
    printf("\n--- INICIO DE PRUEBAS LISTA INSERTAR ---\n");

    // Declaro las variables a utilizar
    int multi = 2;
    int n = 5;
    int vector[n];
    lista_t *lista = lista_crear();

    for (int i = 0; i < n; i++) {
        vector[i] = i+1;
        lista_insertar_ultimo(lista, &vector[i]);
    }
    
    lista_iterar(lista, multiplicar_dato, &multi);

    bool ok = true;
    for (size_t j = 0; j < n; j++) {
        size_t dato = *(size_t *)lista_borrar_primero(lista);
        ok &= (dato == (vector[j]*multi));
    }
    print_test("Se pudo iterar y multiplicar todos los valores.", ok);

    // Destrullo la cola creada
    lista_destruir(lista, NULL);
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
    pruebas_lista_null();
    pruebas_lista_insertar();
    prueba_lista_insertar_volumen();
    pruebas_iteradores_internos();
}
int main(void){
    pruebas();  
    return 0;
}
