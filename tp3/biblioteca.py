from collections import deque
from grafo import Grafo

import heapq
import re
import time
def crear_red(ruta, es_dirigido):
    g = Grafo(es_dirigido)
    with open(ruta, encoding='utf-8') as archivo:
        for linea in archivo:
            linea = linea.rstrip()
            recortado = re.split(r'\t', linea)
            for valor in recortado:
                g.agregar_vertice(valor)
                g.agregar_arista(recortado[0], valor)
    return g



def _dfs(grafo, v, visitados, padres, orden):
    for w in grafo.adyacentes(v):
        if w not in visitados:
            visitados.add(w)
            padres[w] = v
            orden[w] = orden[v] + 1
            _dfs(grafo, w, visitados, padres, orden)

def dfs(grafo, origen):
    padres = {}
    orden = {}
    visitados = set()
    padres[origen] = None
    orden[origen] = 0
    visitados.add(origen)
    _dfs(grafo, origen, visitados, padres, orden)
    return padres, orden

def bfs(grafo, origen):
    visitados = set()
    padres = {}
    orden = {}
    padres[origen] = None
    orden[origen] = 0
    visitados.add(origen)
    q = deque()
    q.append(origen)
    while not len(q) == 0:
        v = q.popleft()
        for w in grafo.obtener_adyacentes(v):
            if w not in visitados:
                padres[w] = v
                orden[w] = orden[v] + 1
                visitados.add(w)
                q.append(w)
    return padres, orden

def bfs_con_maximo_y_terminacion(grafo, v):
    visitados = set()
    padres = {}
    orden = {}
    padres[v] = None
    orden[v] = 0
    maximo = -1
    visitados.add(v)
    q = deque()
    q.append(v)
    while not len(q) == 0:
        v = q.popleft()
        adyacentes = grafo.obtener_adyacentes(v)
        for w in adyacentes:
            if w not in visitados:
                padres[w] = v
                orden[w] = orden[v] + 1
                if orden[w] > maximo:
                    maximo = orden[w]
                visitados.add(w)
                q.append(w)
    return maximo, v

def diametro(grafo):
    #TODO: Chequear si hacer el bfs del final no cambia la complejidad.
    maximo = 0
    origen = None
    destino = None
    vertices = grafo.obtener_vertices()
    for v in vertices:
        dist, fin_camino = bfs_con_maximo_y_terminacion(grafo, v)
        if dist > maximo:
            maximo = dist
            origen = v
            destino = fin_camino
    camino(grafo,origen,destino)

def camino(grafo, origen, destino):
    padres, orden = bfs(grafo, origen)
    if destino not in orden:
        print("No se encontro recorrido")
        return
    actual = destino
    recorrido = []
    recorrido.append(actual)
    while actual != origen:
        actual = padres[actual]
        recorrido.append(actual)
    while len(recorrido) > 0:
        print(f'{recorrido.pop()}', end=" ")
        if len(recorrido) != 0:
            print('->', end=" ")
    print(f'\nCosto: {orden[destino]}')
    return orden[destino]

def rango(grafo, pagina, n):
    #O(P + L) + P = O(P + L)
    _, orden = bfs(grafo, pagina)
    count = 0
    for v in orden:
        if orden[v] == int(n):
            count += 1
    return count

def clustering(grafo, pagina=None):
    #TODO: Chequear si la complejidad esta bien
    if(pagina == None):
        vertices = grafo.obtener_vertices()
        total = 0
        for v in vertices: #O(P)
            total += clustering(grafo, v)
        return round(float(total/len(vertices)),4)

    coeficiente = 0.0
    adyacentes = grafo.obtener_adyacentes(pagina)
    aristas = 0
    if len(adyacentes) < 2:
        return coeficiente
    for v in adyacentes:       #O(P)
        for w in adyacentes:   #O(P)
            if w == v:
                continue
            if grafo.es_adyacente(v,w):
                aristas += 1
    coeficiente = float(aristas/(len(adyacentes)*(len(adyacentes)-1)))
    return round(coeficiente, 4)


def _navegacion(grafo, origen, cont):
    adyacentes = list(grafo.obtener_adyacentes(origen).keys())
    if len(adyacentes) == 0 or cont == 20:
        return origen
    print(f'-> {adyacentes[0]}', end=" ")
    return _navegacion(grafo, adyacentes[0], cont+1)

def navegacion(grafo, origen):
    #TODO: Prints, chequear si son estrictos y arreglar
    print(f'{origen}', end=" ")
    return _navegacion(grafo, origen, 0)

def tiene_adyacente_a_v(grafo):
    dic = {}
    for v in grafo.obtener_vertices():
        for w in grafo.obtener_adyacentes(v):
            dic[w] = dic.get(w, [])
            dic[w].append(v)
    return dic

""""""
def get_pagerank(grafo, pageranks, pageranks_anterior, k):
    dic = tiene_adyacente_a_v(grafo)
    for _ in range(k):   
        vertices = grafo.obtener_vertices()
        for v in vertices:
            
            for a in dic[v]:
                pageranks_anterior[v] = pageranks_anterior.get(v, 0) + pageranks[a]/len(grafo.obtener_adyacentes(a)) + (1-0.75)/len(grafo.obtener_vertices())
                
        for v in vertices:
            pageranks[v] += pageranks_anterior[v] * 0.75
    return pageranks

def mas_importantes(grafo, n):
    pageranks = {}
    pageranks_anterior = {}
    heap = []
    vertices = grafo.obtener_vertices()
    
    # Inicializamos los pageranks
    for v in vertices:
        pageranks[v] = (1-0.75)/len(grafo.obtener_vertices())
    k = 50
    pageranks = get_pagerank(grafo, pageranks, pageranks_anterior, k)
    
    maximos = heapq.nlargest(n, vertices, key=lambda v : pageranks[v])
    print(f'{maximos[0]}', end = "")
    for valor in maximos[1:]:
       print(f', {valor}', end="")
    print()
    return maximos
    

def es_valido_ciclo(recorrido, n):
    return len(recorrido) < n
        
def _ciclo(grafo, pagina_actual, pagina_origen, visitados, n, recorrido):
    if len(recorrido) == n and pagina_actual == pagina_origen:
        return True
    adyacentes = grafo.obtener_adyacentes(pagina_actual)
    for w in adyacentes:
        if w not in visitados:
            recorrido.append(w)
            visitados.add(w)
            if not es_valido_ciclo(recorrido, n+1):
                recorrido.pop()
                visitados.remove(w)
                continue
            if(_ciclo(grafo, w, pagina_origen, visitados, n, recorrido)):
                return True
    if len(visitados) > 0 and len(recorrido) > 0:
        visitados.remove(pagina_actual)
        recorrido.pop()
    return False

def ciclo(grafo, pagina, n):
    recorrido = []
    visitados = set()
    if not _ciclo(grafo, pagina, pagina, visitados, n, recorrido):
        print("No se encontro recorrido")
        return
    recorrido.insert(0, pagina)
    print(recorrido[0],end=" ")
    for v in recorrido[1:]:
        print(f'-> {v}',end=" ")
    print()
