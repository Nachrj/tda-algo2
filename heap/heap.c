#include "heap.h"
#include <string.h> //strcmp
#include <stdio.h>
#include <stdlib.h>

#define CAPACIDAD 10

struct heap{
    void** lista;
    size_t cantidad;
    size_t capacidad;
    cmp_func_t cmp;
};

void swap(void** lista, int a, int b){
    void* aux = lista[a];
    lista[a] = lista[b];
    lista[b] = aux;
}

bool heap_redimensionar(heap_t* heap, size_t tam_nuevo) {
    void** datos_nuevo = realloc(heap->lista, tam_nuevo * sizeof(void*));

    // Cuando tam_nuevo es 0, es correcto si se devuelve NULL.
    // En toda otra situación significa que falló el realloc.
    if (tam_nuevo > 0 && datos_nuevo == NULL) {
        return false;
    }
    heap->lista = datos_nuevo;
    heap->capacidad = tam_nuevo;
    return true;
}


void upheap(heap_t* heap, int actual){
    if(actual-1 < 0) return;
    int padre = (actual-1)/2;
    if(heap->cmp(heap->lista[padre],heap->lista[actual]) > 0){
        return;
    }
    if(heap->cmp(heap->lista[padre],heap->lista[actual]) <= 0){
        swap(heap->lista,actual,padre);
        return upheap(heap,padre);
    }
    return;
}

void downheap(heap_t* heap, int actual){
    int hijo_izq = (actual*2)+1;
    int hijo_der = (actual*2)+2;
    if(hijo_izq > heap_cantidad(heap)-1 || hijo_der > heap_cantidad(heap)-1){
        return;
    }
    int hijo_max = heap->cmp(heap->lista[hijo_izq],heap->lista[hijo_der]) > 0 ? hijo_izq : hijo_der;
    if(heap->cmp(heap->lista[hijo_max],heap->lista[actual]) <= 0){
        return;
    }
    if(heap->cmp(heap->lista[hijo_max],heap->lista[actual]) > 0){
        swap(heap->lista,actual,hijo_max);
        return downheap(heap,hijo_max);
    }
    return;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    heap_t* heap_aux = heap_crear(cmp);
    for(int i = 0; i < cant; i++){
        heap_encolar(heap_aux,elementos[i]);
    }
    for(int i = 0; i < cant; i++){
        elementos[i] = heap_desencolar(heap_aux);
    }
    heap_destruir(heap_aux,NULL);
}

heap_t *heap_crear(cmp_func_t cmp){
    heap_t* heap = calloc(1,sizeof(heap_t));
    void** lista = calloc(CAPACIDAD,sizeof(void*));
    heap->capacidad = CAPACIDAD;
    heap->lista = lista;
    heap->cmp = cmp;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t* heap = calloc(1,sizeof(heap_t));
    heap->lista = arreglo;
    heap->capacidad = CAPACIDAD;
    downheap(heap,0); // Debería ser el ultimo al primero, pero no funciona así downheap
    heap->cmp = cmp;
    return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
    if(destruir_elemento) {
        for (int i = 0; i < heap->cantidad; i++) {
            destruir_elemento(heap->lista[i]);
        }
    }
    free(heap->lista);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
    return !heap->cantidad;
}

bool heap_encolar(heap_t *heap, void *elem){
    int i = (int)heap_cantidad(heap);
    heap->lista[i] = elem;
    heap->cantidad++;
    upheap(heap,i);
    if(heap->cantidad == heap->capacidad){
        if(!heap_redimensionar(heap, heap->capacidad*2)){
            return false;
        }
    }
    return true;
}

void *heap_ver_max(const heap_t *heap){
    return heap->lista[0];
}

void *heap_desencolar(heap_t *heap){
    void* dato = heap->lista[0];
    heap->lista[0] = heap->lista[heap_cantidad(heap)-1];
    heap->lista[heap_cantidad(heap)-1] = NULL;
    heap->cantidad--;
    downheap(heap,0);
    if(heap->cantidad <= heap->capacidad/2){
        if(!heap_redimensionar(heap, heap->capacidad/2)){
            return false;
        }
    }
    return dato;
}

void pruebas_heap_estudiante(void){

}

int intcmp(const void* a, const void* b){
    if(*(int*)a > *(int*)b) return 1;
    if(*(int*)a <*(int*)b) return -1;
    return 0;
}

int main(void){
    heap_t* heap = heap_crear(intcmp);
    int a = 1;
    int b = 3;
    int c = 2;
    int d = 5;
    void** arreglo = calloc(4,sizeof(void*));
    arreglo[0] = &a;
    arreglo[1] = &b;
    arreglo[2] = &c;
    arreglo[3] = &d;
    heap_crear_arr(arreglo,4,intcmp);
    /*heap_encolar(heap,&a);
    heap_encolar(heap,&b);
    heap_encolar(heap,&c);
    heap_encolar(heap,&d);
    heap_desencolar(heap);*/
    printf("%d",*(int*)heap->lista[0]);
    heap_destruir(heap,NULL);
    return 0;
}
