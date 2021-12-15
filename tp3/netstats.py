from grafo import Grafo
from biblioteca import *
RUTA_WIKI = 'wiki-reducido-75000.tsv'

def main():
    netstats = crear_red(RUTA_WIKI)
    while True:
        entrada = input()
        entrada = entrada.split(" ", 1)
        comando = entrada[0]
        parametros = entrada[1]
        if comando == 'camino':
            links = parametros.split(',')
            if len(links) != 2:
                continue
            origen = links[0]
            destino = links[1]
            camino(netstats, origen, destino)
main()
