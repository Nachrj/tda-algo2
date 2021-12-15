from grafo import Grafo
import re
OPERACIONES = ['camino','mas_importantes']
def crear_red(ruta):
    g = Grafo()
    with open(ruta, encoding='utf-8') as archivo:
        for linea in archivo:
            linea = linea.rstrip()
            recortado = re.split(r'\t+', linea)
            for valor in recortado:
                g.agregar_vertice(valor)
                if valor == recortado[0]:
                    continue
                g.agregar_arista(recortado[0], valor)
    return g


def camino(grafo, origen, destino):
    pass

def mas_importantes(grafo, n):
    pass
