#include "abb.h"
#include "pila.h" 
#include <stdlib.h>
#include <string.h>

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
    nodo_t *nodo = calloc(1,sizeof(nodo_t));
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
    free((char*)nodo->clave);
    free(nodo);
}

// *** PRIMITIVAS DEL ABB ***
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = calloc(1,sizeof(abb_t));
    if(!abb) return NULL;
    abb->cmp = cmp;
    abb->destruir_dato = destruir_dato;
    abb->cantidad = 0;
    return abb;
}

bool _abb_guardar(abb_t *arbol, const char *clave, void *dato, nodo_t* actual){
    // Si el arbol no tiene raiz, el nodo a ingresar se convierte en su raiz
    if(!arbol->raiz){
        arbol->raiz = nodo_crear(clave,dato);
        arbol->cantidad++;
        return true;
    }

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
            actual->der = nodo_crear(clave,dato);
            arbol->cantidad++;
            return true;
        }
        return _abb_guardar(arbol,clave,dato,actual->der);
    }
    else{
        if(!actual->izq){
            actual->izq = nodo_crear(clave,dato);
            arbol->cantidad++;
            return true;
        }
        return _abb_guardar(arbol,clave,dato,actual->izq);
    }
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if (!_abb_guardar(arbol,clave,dato,arbol->raiz)) return false;
    return true;
}

nodo_t* sucesor(nodo_t* actual, nodo_t** extremo){
    //printf("%d",*(int*)actual->dato);
    if(!actual) return NULL;
    if(!actual->izq){
        *extremo = actual;
        return actual->der;
    }
    actual->izq = sucesor(actual->izq, extremo);
    return actual;
}

nodo_t *_abb_borrar(abb_t* arbol, const char* clave, nodo_t* actual, void** dato){
    if(!actual) return NULL;
    
    if(!arbol->cmp(clave,actual->clave)){
        *dato = actual->dato;
        // Caso 0 hijos
        if(!actual->izq && !actual->der){
            if(actual == arbol->raiz){
                free(arbol->raiz->clave);
                nodo_t* aux = arbol->raiz;
                arbol->raiz = NULL;
                free(aux);
            }
            return NULL;
        }
        // Casos 1 hijo
        if(!actual->izq && actual->der){
            if(actual == arbol->raiz){
                free(arbol->raiz->clave);
                nodo_t* aux = arbol->raiz;
                arbol->raiz = actual->der;
                free(aux);
                return actual;
            }
            return actual->der;
        }
        if(!actual->der && actual->izq){
            if(actual == arbol->raiz){
                free(arbol->raiz->clave);
                nodo_t* aux = arbol->raiz;
                arbol->raiz = actual->izq;
                free(aux);
                return actual;
            }
            return actual->izq;
        }

        // Caso 2 hijos
        if(actual->der && actual->izq){
            nodo_t* ext;
            actual->der = sucesor(actual->der , &ext);
            free(actual->clave);
            actual->clave = ext->clave;
            actual->dato = ext->dato;
            free(ext);
            return actual;
        }
        
    } else if(arbol->cmp(clave,actual->clave) > 0){
        nodo_t* aux = actual->der;
        actual->der = _abb_borrar(arbol,clave,actual->der,dato);
        if(aux){
            nodo_destruir(NULL,aux);
        }
        return actual;
    }
    nodo_t* aux = actual->der;
    actual->izq = _abb_borrar(arbol,clave,actual->izq,dato);
    if(aux){
        nodo_destruir(NULL,aux);
    }
    return actual;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    void* dato = NULL;
    if(!abb_pertenece(arbol, clave)) return NULL;
    _abb_borrar(arbol,clave,arbol->raiz, &dato);
    arbol->cantidad--;
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
void _abb_in_order(abb_t* arbol, nodo_t* actual, bool visitar(const char *, void *, void *), void *extra){
    if(!actual) return;
    _abb_in_order(arbol,actual->izq,visitar,extra);
     if (!visitar(actual->clave,actual->dato,extra)) return;
    _abb_in_order(arbol,actual->der,visitar,extra);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    _abb_in_order(arbol,arbol->raiz,visitar,extra);
}

// *** PRIMITIVAS DE ITERADOR EXTERNO ***
abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if(!iter) return NULL;
    pila_t* pila = pila_crear();
    iter->pila = pila;
    if(arbol->raiz){
        pila_apilar(iter->pila,arbol->raiz);
    }
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)) return false;
    nodo_t* actual = pila_desapilar(iter->pila);
    if(actual->der){
        pila_apilar(iter->pila,actual->der);
    }
    if(actual->izq){
        pila_apilar(iter->pila,actual->izq);
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

int main(void){
    
    abb_t* abb = abb_crear(strcmp,NULL);
    //abb_iter_t* iter = abb_iter_in_crear(abb);
    
    int datos[] = {36,64,1,27,58,59,50,79,15,25,32};
    abb_guardar(abb,"36",&datos[0]);
    abb_guardar(abb,"64",&datos[1]);
    abb_guardar(abb,"01",&datos[2]);
    abb_guardar(abb,"27",&datos[3]);
    abb_guardar(abb,"58",&datos[4]);
    abb_guardar(abb,"59",&datos[5]);
    abb_guardar(abb,"50",&datos[6]);
    abb_guardar(abb,"79",&datos[7]);
    abb_guardar(abb,"15",&datos[8]);
    abb_guardar(abb,"25",&datos[9]);
    abb_guardar(abb,"32",&datos[10]);
    abb_borrar(abb,"58");
    abb_borrar(abb,"64");
    abb_in_order(abb,print_,NULL);
    abb_destruir(abb);
    return 0;
}
