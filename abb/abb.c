#include "abb.h"

typedef struct nodo nodo_t;

struct nodo {
    void* dato; // Apunta al dato almacenado
    const char* clave;
    nodo_t* izq; // Apunta al nodo izquierdo
    nodo_t* der; // Apunta al nodo derecho
};

// *** PRIMITIVAS DE NODO ***
nodo_t *nodo_crear(const char* clave, void* dato) {
    nodo_t *nodo = calloc(1,sizeof(nodo_t));
    if (!nodo) return NULL;

    nodo->dato = dato;
    nodo->clave = clave;
    nodo->izq = NULL;
    nodo->der = NULL;

    return nodo;
}

void nodo_destruir(nodo_t* nodo){
    free(nodo);
}

struct abb{
    nodo_t* raiz;
    size_t cantidad;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
    
};

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
        return true;
    }

    if(!actual) return false;
    
    // Si la clave ya existe, se sobreescribe el dato
    if(!arbol->cmp(clave,actual->clave)){
        actual->dato = dato;
        return true;
    }

    // Caso general
    if(arbol->cmp(clave,actual->clave)>0){
        if(!actual->der){
            actual->der = nodo_crear(clave,dato);
            return true;
        }
        return _abb_guardar(arbol,clave,dato,actual->der);
    }
    else{
        if(!actual->izq){
            actual->izq = nodo_crear(clave,dato);
            return true;
        }
        return _abb_guardar(arbol,clave,dato,actual->izq);
    }
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if (!_abb_guardar(arbol,clave,dato,arbol->raiz)) return false;
    arbol->cantidad++;
    return true;
}
nodo_t* mas_derecha(nodo_t* actual, nodo_t** extremo){
    //printf("%d",*(int*)actual->dato);
    if(!actual) return NULL;
    if(!actual->izq){
        *extremo = actual;
        return actual->der;
    }
    actual->izq = mas_derecha(actual->izq, extremo);
    return actual;
}
nodo_t *_abb_borrar(abb_t* arbol, const char* clave, nodo_t* actual, void** dato){
    if(!actual) return NULL;
    if(!abb_pertenece(arbol,clave)) return NULL;
    
    if(!arbol->cmp(clave,actual->clave)){
        *dato = actual->dato;
        
        // Caso 0 hijos
        if(!actual->izq && !actual->der){
            return NULL;
        }
        // Casos 1 hijo
        if(!actual->izq && actual->der){
          return actual->der;
        }
        if(!actual->der && actual->izq){
            return actual->izq;
        }
        // Caso 2 hijos
        if(actual->der && actual->izq){
            nodo_t* ext;
            mas_derecha(actual->der , &ext);
            //free cosas de actual
            actual->clave = ext->clave;
            actual->dato = ext->dato;
            //free ext
            return actual;
        }
    } else if(arbol->cmp(clave,actual->clave) > 0){
        actual->der = _abb_borrar(arbol,clave,actual->der,dato);
        return actual;
    } else {
        actual->izq = _abb_borrar(arbol,clave,actual->izq,dato);
        return actual;
    }
}

void *abb_borrar(abb_t *arbol, const char *clave){
    void* dato = NULL;
    if (_abb_borrar(arbol,clave,arbol->raiz, &dato)){
        arbol->cantidad--;
    }
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

bool _abb_pertenece(const abb_t *arbol, const char *clave, nodo_t* actual){
    if(!actual || !arbol->raiz) return false;

    if(!arbol->cmp(clave,actual->clave)){
        return true;
    }

    if(arbol->cmp(clave,actual->clave)>0){
        return _abb_pertenece(arbol,clave,actual->der);
    }
    return _abb_pertenece(arbol,clave,actual->izq);
}
bool abb_pertenece(const abb_t *arbol, const char *clave){
    return _abb_pertenece(arbol,clave,arbol->raiz);
}

size_t abb_cantidad(const abb_t *arbol){
    return arbol->cantidad;
}

void abb_destruir(abb_t *arbol){
    return;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    return;
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    return NULL;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    return false;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    return NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return false;
}

void abb_iter_in_destruir(abb_iter_t* iter){
    return;
}

int main(void){
    abb_t* abb = abb_crear(strcmp,NULL);
    int datos[] = {10,5,21,7,15,13,19,17,20,18,16};
    abb_guardar(abb,"i",&datos[0]);
    abb_guardar(abb,"b",&datos[1]);
    abb_guardar(abb,"z",&datos[2]);
    abb_guardar(abb,"d",&datos[3]);
    abb_guardar(abb,"o",&datos[4]);
    abb_guardar(abb,"n",&datos[5]);
    abb_guardar(abb,"w",&datos[6]);
    abb_guardar(abb,"u",&datos[7]);
    abb_guardar(abb,"y",&datos[8]);
    abb_guardar(abb,"v",&datos[9]);
    abb_guardar(abb,"t",&datos[10]);
    void* dato = abb_borrar(abb,"o");
    printf("%d\n",*(int*)abb->raiz->der->izq->der->izq->der->dato);
    return 0;
}
