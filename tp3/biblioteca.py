from collections import deque
from grafo import Grafo

import heapq
import sys

import re
import random

OPERACIONES = ['camino', 'clustering', 'diametro', 'rango', 'navegacion', 'mas_importantes']

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

def listar_operaciones():
    for operacion in OPERACIONES:
        print(operacion)

def _dfs(grafo, v, visitados, padres, origen):
    for w in grafo.adyacentes(v):
        if w not in visitados:
            visitados.add(w)
            padres[w] = v
            origen[w] = orden[v] + 1
            _dfs(grafo, w, visitados, padres, origen)

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
    for v in grafo.obtener_vertices():
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

def get_pagerank(grafo, origen, k):
    if (k == 50):
        return 1
    d = 0.75
    primer_termino = (1-d)/grafo.cantidad
    segundo_termino = 0
    vertices = grafo.obtener_vertices()
    for v in vertices:
        adyacentes = grafo.obtener_adyacentes(origen)
        if v in adyacentes:
            k += 1
            segundo_termino += get_pagerank(grafo, v, k)/len(adyacentes)
    print(primer_termino + segundo_termino)
    return primer_termino + segundo_termino

def mas_importantes(grafo, n):
    pageranks = {}
    heap = []
    vertices = grafo.obtener_vertices()
    for v in vertices:
        k = 0
        pagerank = get_pagerank(grafo, v, k)
        heapq.heappush(heap, pagerank)
        pageranks[pagerank] = v
    
    maximos = heapq.nlargest(n, heap)

    for valor in maximos:
        print(f'{pageranks[valor]},', end=" ")


