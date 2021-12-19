#!/usr/bin/python3
from grafo import Grafo
from biblioteca import *
RUTA_WIKI = 'wiki-reducido-75000.tsv'
ES_DIRIGIDO = True
def main():
    netstats = crear_red(RUTA_WIKI, ES_DIRIGIDO)
    #print("Francia" in netstats.obtener_adyacentes("Argentina"))
    while True:
        entrada = input()
        entrada = entrada.split(" ", 1)
        comando = entrada[0]
        if len(entrada) > 1:
            parametros = entrada[1]
        if comando == 'camino':
            links = parametros.split(',')
            if len(links) != 2:
                continue
            origen = links[0]
            destino = links[1]
            camino(netstats, origen, destino)
        if comando == 'diametro':
            diametro(netstats)
        if comando == 'rango':
            links = parametros.split(',')
            if len(links) != 2:
                continue
            pagina = links[0]
            saltos = links[1]
            print(rango(netstats, pagina, saltos))
        if comando == 'listar_operaciones':
            listar_operaciones()
        if comando == 'clustering':
            if len(entrada) != 1:
                print(clustering(netstats, parametros))
            else:
                print(clustering(netstats))
        if comando == 'navegacion':
            navegacion(netstats, parametros)
        if comando == 'mas_importantes':
            links = parametros.split(',')
            if len(links) != 1:
                continue
            n = links[0]
            mas_importantes(netstats, int(n))
        if comando == 'ciclo':
            links = parametros.split(',')
            ciclo(netstats, links[0], int(links[1]))
main()
