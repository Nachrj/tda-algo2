#include "heap.h"
#include "testing.h"
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.
#include <stdbool.h>

int intcmp(const void* a, const void* b){
    if(*(int*)a > *(int*)b) return 1;
    if(*(int*)a <*(int*)b) return -1;
    return 0;
}

static void prueba_crear_heap_vacio() {
    printf("\n--- INICIO DE PRUEBAS HEAP VACIO ---\n");
    heap_t* heap = heap_crear(NULL);

    print_test("Prueba heap crear abb vacio", heap);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("Prueba heap ver maximo, es NULL", !heap_ver_max(heap));
    print_test("Prueba heap desencolar clave A, es NULL, no existe", !heap_desencolar(heap));
    
    heap_destruir(heap, NULL);
}

static void prueba_heap_insertar() {
    printf("\n--- INICIO DE PRUEBAS HEAP INSERTAR ---\n");
    heap_t* heap = heap_crear(intcmp);

    int a = 3;
    int b = 4;
    int c = 2;
    int d = 6;
    int e = 7;
    int f = 9;
    int g = 5;
    int h = 10;
    int i = 8;
    int j = 1;
    int k = 11;
    int l = 12;

    void* claves[] = {&a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l};

    print_test("Prueba heap insertar clave 3", heap_encolar(heap, claves[0]));
    print_test("Prueba heap insertar clave 4", heap_encolar(heap, claves[1]));
    print_test("Prueba heap insertar clave 2", heap_encolar(heap, claves[2]));
    print_test("Prueba heap insertar clave 6", heap_encolar(heap, claves[3]));
    print_test("Prueba heap insertar clave 7", heap_encolar(heap, claves[4]));
    print_test("Prueba desencolar, devuelve 7", heap_desencolar(heap) == claves[4]);
    print_test("Prueba heap insertar clave 9", heap_encolar(heap, claves[5]));
    print_test("Prueba heap insertar clave 5", heap_encolar(heap, claves[6]));
    print_test("Prueba heap insertar clave 10", heap_encolar(heap, claves[7]));
    print_test("Prueba heap insertar clave 8", heap_encolar(heap, claves[8]));
    print_test("Prueba heap insertar clave 1", heap_encolar(heap, claves[9]));
    print_test("Prueba heap insertar clave 11", heap_encolar(heap, claves[10]));
    print_test("Prueba heap insertar clave 12", heap_encolar(heap, claves[11]));
    print_test("Prueba heap la cantidad de elementos es 11", heap_cantidad(heap) == 11);
    print_test("Prueba heap ver maximo es 12", heap_ver_max(heap) == claves[11]);

    heap_destruir(heap, NULL);
}

static void prueba_heap_desencolar(){
    printf("\n--- INICIO DE PRUEBAS HEAP DESENCOLAR ---\n");
    heap_t* heap = heap_crear(intcmp);
    
    int a = 3;
    int b = 4;
    int c = 2;
    int d = 6;
    int e = 7;
    int f = 9;
    int g = 5;
    int h = 10;
    int i = 8;
    int j = 1;
    int k = 11;
    int l = 12;
    void* claves[] = {&a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l};
    heap_encolar(heap, claves[0]);
    print_test("Desencolar heap de un elemento lo deja como uno vacio",heap_desencolar(heap) == claves[0] && heap_cantidad(heap) == 0);
    heap_encolar(heap, claves[0]);
    heap_encolar(heap, claves[1]);
    heap_encolar(heap, claves[2]);
    heap_encolar(heap, claves[3]);
    heap_encolar(heap, claves[4]);
    heap_encolar(heap, claves[5]);
    heap_encolar(heap, claves[6]);
    heap_encolar(heap, claves[7]);
    heap_encolar(heap, claves[8]);
    heap_encolar(heap, claves[9]);
    heap_encolar(heap, claves[10]);
    heap_encolar(heap, claves[11]);
    void* desencolado[] = {&l, &k, &h, &f, &i, &e, &d, &g, &b, &a, &c, &j};
    bool ok = true;
    for(int i = 0; i < 12; i++){
        void* dato = heap_desencolar(heap);
        ok &= (dato == desencolado[i]);
    }
    print_test("Pruebo insertar elementos, desencolarlos devuelve segun propiedad de heap",ok);
    print_test("Heap queda vacio despues de vaciarlo",heap_esta_vacio(heap));
    heap_destruir(heap, NULL);
}
static void prueba_heap_sort(){
    printf("\n--- INICIO DE PRUEBAS HEAPSORT ---\n");

    int a = 3;
    int b = 4;
    int c = 2;
    int d = 6;
    int e = 7;
    int f = 9;
    int g = 5;
    void* claves[] = {&a, &b, &c, &d, &e, &f, &g};
    void* claves_ordenado[] = {&c, &a, &b, &g, &d, &e, &f};
    int n = 7;
    heap_sort(claves, n, intcmp);
    bool ok = true;
    for(int i = 0; i < n; i++){
        ok &= (claves[i] == claves_ordenado[i]);
    }
    print_test("Heapsort ordena correctamente",ok);
}

static void prueba_crear_arr(){
    printf("\n--- INICIO DE PRUEBAS HEAP CREADO DESDE UN ARREGLO ---\n");
    heap_t* heap = heap_crear(intcmp);
    int a = 3;
    int b = 4;
    int c = 2;
    int d = 6;
    int e = 7;
    int f = 9;
    int g = 5;
    void* claves[] = {&a, &b, &c, &d, &e, &f, &g};
    int n = 7;
    heap_t* heap_arr = heap_crear_arr(claves,n,intcmp);
    heap_encolar(heap,claves[0]);
    heap_encolar(heap,claves[1]);
    heap_encolar(heap,claves[2]);
    heap_encolar(heap,claves[3]);
    heap_encolar(heap,claves[4]);
    heap_encolar(heap,claves[5]);
    heap_encolar(heap,claves[6]);
    bool ok = true;
    for(int i = 0; i < n; i++){
        ok &= (*(int*)heap_ver_max(heap) == *(int*)heap_ver_max(heap_arr));
        heap_desencolar(heap);
        heap_desencolar(heap_arr);
    }
    print_test("Heap desde arreglo se crea correctamente",ok);
    heap_destruir(heap_arr, NULL);
    heap_destruir(heap, NULL);
}

bool encolar_n_y_desencolar(heap_t* heap, size_t n){
    int array[n];
    // Encolo n elementos 
    for(size_t i = 0; i <= n;i++){
        array[i]=(int)i;
        if(!heap_encolar(heap,&array[i])){
            return false;
        }
    }
    size_t index = n;
    while(!heap_esta_vacio(heap)){
        if(heap_desencolar(heap) != &array[index]){
            return false;
        }
        index--;
    }
    return true;
}

static void prueba_heap_volumen(){
    printf("\n--- INICIO PRUEBAS VOLUMEN ---\n");

    /* Creo la cola*/
    heap_t* heap = heap_crear(intcmp);
    heap_t* nuevo_heap = heap_crear(intcmp);

    /* Pruebas volumen*/
    print_test("encolar 1000 elementos encola correctamente",encolar_n_y_desencolar(heap,1000));
    print_test("encolar 10000 elementos encola correctamente",encolar_n_y_desencolar(heap,10000));
    print_test("encolar 100000 elementos encola correctamente",encolar_n_y_desencolar(heap,100000));
    print_test("Desencolar hasta que quede vacio funciona correctamente",heap_esta_vacio(heap));
    print_test("Heap vaciado actua de la misma manera que heap recien creado",heap_esta_vacio(heap) == heap_esta_vacio(nuevo_heap) && heap_ver_max(heap)== heap_ver_max(nuevo_heap));
    print_test("Funcion heap_ver_max() con el heap vaciado devuelve NULL", heap_ver_max(heap)==NULL);
    print_test("Funcion heap_desencolar() con el heap vaciado devuelve NULL", heap_desencolar(heap) == NULL);
    heap_destruir(heap,NULL);
    heap_destruir(nuevo_heap,NULL);
}

static void _lista_destruir(void* lista){
    lista_destruir(lista,NULL);
}

int comparar(const void* a, const void* b){
    return strcmp((char*)a,(char*)b);
}
static void prueba_lista_en_heap(){
    printf("\n--- INICIO PRUEBAS PILA EN COLA ---\n");

    //Creo elementos de prueba 
    heap_t* heap = heap_crear(comparar);
    lista_t* lista = lista_crear();
    
    
    print_test("heap_encolar() lista encola correctamente", heap_encolar(heap, lista));
    print_test("Maximo del heap es la primer lista encolada", heap_ver_max(heap) == lista);
    print_test("heap_desencolar() lista desencola correctamente", heap_desencolar(heap) == lista);
    lista_destruir(lista,NULL);
    heap_destruir(heap,_lista_destruir);
}

static void prueba_destruir_dinamico(){
    heap_t* heap = heap_crear(NULL);
    int* elemento = malloc(sizeof(int));
    heap_encolar(heap,elemento);
    heap_destruir(heap,free);
}


void pruebas_heap_estudiante()
{
    /* Ejecuta todas las pruebas unitarias. */
    prueba_crear_heap_vacio();
    prueba_heap_insertar();
    prueba_heap_desencolar();
    prueba_heap_sort();
    prueba_crear_arr();
    prueba_destruir_dinamico();
    prueba_heap_volumen();
    prueba_lista_en_heap();
}

#ifndef CORRECTOR
int main(void){
    pruebas_heap_estudiante();
    return 0;
}
#endif