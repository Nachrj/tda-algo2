class Grafo:
    def __init__(self, vertices):
        self.V = vertices
        self.grafo = []

    def add_aresta(self, u, v, peso):
        self.grafo.append([u, v, peso])

    def get_grafo(self):
        return self.grafo

    def get_vertices(self):
        return self.V

    def get_arestas(self):
        return self.grafo