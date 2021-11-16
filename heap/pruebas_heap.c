#include "heap.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.

int intcmp(const void* a, const void* b){
    if(*(int*)a > *(int*)b) return 1;
    if(*(int*)a <*(int*)b) return -1;
    return 0;
}

static void prueba_crear_heap_vacio() {
    heap_t* heap = heap_crear(NULL);

    print_test("Prueba heap crear abb vacio", heap);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("Prueba heap ver maximo, es NULL", !heap_ver_max(heap));
    print_test("Prueba heap desencolar clave A, es NULL, no existe", !heap_desencolar(heap));
    
    heap_destruir(heap, NULL);
}

static void prueba_heap_insertar() {
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

void pruebas_abb_estudiante()
{
    /* Ejecuta todas las pruebas unitarias. */
    prueba_crear_heap_vacio();
    prueba_heap_insertar();
}

#ifndef CORRECTOR
int main(void){
    pruebas_abb_estudiante();
    return 0;
}
#endif