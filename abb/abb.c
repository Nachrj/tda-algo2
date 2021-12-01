#include "abb.h"
#include "pila.h" 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
typedef struct nodo nodo_t;

struct nodo {
    void* dato; // Apunta al dato almacenado
    char* clave;
    nodo_t* izq; // Apunta al nodo izquierdo
    nodo_t* der; // Apunta al nodo derecho
};

struct abb{
    nodo_t* raiz;
    size_t cantidad;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
    
};

struct abb_iter{
    pila_t* pila;
};

// *** PRIMITIVAS DE NODO ***
char* copia_clave(const char *clave){
	size_t len = strlen(clave);
	char* copia = malloc((len+1)*sizeof(char));
	strcpy((char*)copia,clave);
	return copia;
}

nodo_t *nodo_crear(const char* clave, void* dato) {
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;

    nodo->dato = dato;
    nodo->clave = copia_clave(clave);
    nodo->izq = NULL;
    nodo->der = NULL;

    return nodo;
}

void nodo_destruir(abb_destruir_dato_t destruir_dato, nodo_t* nodo){
    if(destruir_dato){
        destruir_dato(nodo->dato);
    }
    free(nodo->clave);
    free(nodo);
}

// *** PRIMITIVAS DEL ABB ***
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if(!abb) return NULL;
    abb->raiz = NULL;
    abb->cmp = cmp;
    abb->destruir_dato = destruir_dato;
    abb->cantidad = 0;
    return abb;
}

bool _abb_guardar(abb_t *arbol, const char *clave, void *dato, nodo_t* actual){
    if(!actual) return false;
    
    // Si la clave ya existe, se sobreescribe el dato
    if(!arbol->cmp(clave,actual->clave)){
        if(arbol->destruir_dato){
            arbol->destruir_dato(actual->dato);
        }
        actual->dato = dato;
        return true;
    }

    // Caso general
    if(arbol->cmp(clave,actual->clave)>0){
        if(!actual->der){
            actual->der = nodo_crear(clave, dato);
            if(!actual->der) return false;
            arbol->cantidad++;
            return true;
        }
        return _abb_guardar(arbol,clave,dato,actual->der);
    }
    else{
        if(!actual->izq){
            actual->izq = nodo_crear(clave,dato);
            if (!actual->izq) return false;
            arbol->cantidad++;
            return true;
        }
        return _abb_guardar(arbol,clave,dato,actual->izq);
    }
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    // Si el arbol no tiene raiz, el nodo a ingresar se convierte en su raiz
    if(!arbol->raiz){
        arbol->raiz = nodo_crear(clave, dato);
        if (!arbol->raiz) return false;
        arbol->cantidad++;
        return true;
    }
    
    if (!_abb_guardar(arbol,clave,dato,arbol->raiz)) return false;
    return true;
}

nodo_t* sucesor(nodo_t* actual){
    //printf("%d",*(int*)actual->dato);
    if(!actual) return NULL;
    if(!actual->izq){
        return actual;
    }
    return sucesor(actual->izq);
}

nodo_t *_abb_borrar(abb_t* arbol, const char* clave, nodo_t* actual, void** dato){
    if(!actual) return NULL;
    if(!arbol->cmp(clave,actual->clave)){
        *dato = actual->dato;
        if(actual->der && actual->izq){
            nodo_t* ext = sucesor(actual->der);
            char* clave_sucesor = copia_clave(ext->clave);
            void* dato_sucesor = ext->dato;
            actual->der = _abb_borrar(arbol,clave_sucesor,actual->der,&dato_sucesor);
            free(actual->clave);
            actual->clave = clave_sucesor;
            actual->dato = dato_sucesor;
            return actual;
        }
        else{
            nodo_t* hijo = actual->izq ? actual->izq : actual->der;
            if(arbol->raiz == actual){
                arbol->raiz = hijo;
            }
            nodo_destruir(NULL,actual);
            arbol->cantidad--;
            return hijo;
        }
    } else if(arbol->cmp(clave,actual->clave) > 0){
        actual->der = _abb_borrar(arbol,clave,actual->der,dato);
    } else {
        actual->izq = _abb_borrar(arbol,clave,actual->izq,dato);
    }
    return actual;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    void* dato = NULL;
    arbol->raiz = _abb_borrar(arbol,clave,arbol->raiz, &dato);
    return dato;
}

void *_abb_obtener(const abb_t *arbol, const char *clave, nodo_t* actual){
    if(!actual || !arbol->raiz) return NULL;

    if(!arbol->cmp(clave,actual->clave)){
        return actual->dato;
    }

    if(arbol->cmp(clave,actual->clave)>0){
        return _abb_obtener(arbol,clave,actual->der);
    }
    return _abb_obtener(arbol,clave,actual->izq);
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    return _abb_obtener(arbol,clave,arbol->raiz);
}

bool _abb_pertenece(abb_comparar_clave_t cmp, const char *clave, nodo_t* actual){
    if(!actual) return false;

    if(!cmp(clave,actual->clave)){
        return true;
    }
    if(cmp(clave,actual->clave)>0){
        return _abb_pertenece(cmp,clave,actual->der);
    }
    return _abb_pertenece(cmp,clave,actual->izq);
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    return _abb_pertenece(arbol->cmp,clave,arbol->raiz);
}

size_t abb_cantidad(const abb_t *arbol){
    return arbol->cantidad;
}

void _abb_destruir(abb_t* arbol, nodo_t* actual){
    if(!actual) return;
    _abb_destruir(arbol,actual->izq);
    _abb_destruir(arbol,actual->der);
    nodo_destruir(arbol->destruir_dato,actual);
    

}

void abb_destruir(abb_t *arbol){
    _abb_destruir(arbol,arbol->raiz);
    free(arbol);
}

// *** PRIMITIVAS DE ITERADOR INTERNO ***
void _abb_in_order(abb_t* arbol, nodo_t* actual, bool visitar(const char *, void *, void *), void *extra, bool* fin){
    if(!actual) return;
    if(*fin) return;
    _abb_in_order(arbol,actual->izq,visitar,extra,fin);
    if(*fin) return;
    if (!visitar(actual->clave,actual->dato,extra)){
        *fin = true;
        return;
    }
    _abb_in_order(arbol,actual->der,visitar,extra,fin);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    bool fin = false;
    _abb_in_order(arbol,arbol->raiz,visitar,extra,&fin);
}

// *** PRIMITIVAS DE ITERADOR EXTERNO ***
bool apilar_izquierdos(nodo_t* actual, pila_t* pila){
    while(actual){
        if (!pila_apilar(pila,actual)) return false;
        actual = actual->izq;
    }
    return true;
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if(!iter) return NULL;
    pila_t* pila = pila_crear();
    if (!pila) return NULL;
    iter->pila = pila;
    if(arbol->raiz){
        apilar_izquierdos(arbol->raiz,iter->pila);
    }
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)) return false;
    nodo_t* actual = pila_desapilar(iter->pila);
    if(actual->der){
        return apilar_izquierdos(actual->der,iter->pila);
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)) return NULL;
    nodo_t* tope = pila_ver_tope(iter->pila);
    return tope->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}
bool print_(const char* clave, void* dato, void* extra) {
    printf("%s\n", (char*)clave);
    return true;
}
