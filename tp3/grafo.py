class Grafo:
    def __init__(self):
        self.vertices = {}
        self.cantidad = 0

    def agregar_vertice(self, v):
        self.vertices[v] = {}
        self.cantidad += 1

    def borrar_vertice(self, v):
        self.vertices.pop(v)

    def agregar_arista(self, v, w):
        """
        Agrega una arista entre 2 vertices.
        Si no existe alguno de los 2 vertices devuelve false.
        Si ya existe la arista devuelve false.
        """
        if v not in self.vertices or w not in self.vertices:
            return False
        if self.estan_unidos(v, w):
            return False
        self.vertices[v][w] = 1
        self.vertices[w][v] = 1

    def borrar_arista(self, v, w):
        """
        Borra la arista entre 2 vertices.
        Si no existe alguno de los 2 vertices devuelve false.
        
        """
        if not self.estan_unidos(v,w):
            return False
        self.vertices[v].pop(w)
        self.vertices[w].pop(v)

    def obtener_vertices(self):
        """
        Devuelve una lista con todos los vertices del grafo
        """
        lista = []
        for v in self.vertices:
            lista.append(v)
        return lista
    
    def obtener_adyacentes(self, v):
        """
        Devuelve todos los adyacentes de un vertice.
        Si no existe el vertice devuelve None
        """
        if not self.vertices[v]:
            return None
        return self.vertices[v]

    def estan_unidos(self, v, w):
        """
        Devuelve True si 2 vertices estan unidos.
        Si no existe alguno de los 2 vertices devuelve false.
        """
        if v not in self.vertices or w not in self.vertices:
            return False
        return v in self.vertices[w] and w in self.vertices[v]
    
