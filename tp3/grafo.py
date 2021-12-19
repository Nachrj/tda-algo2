class Grafo:
    def __init__(self, es_dirigido):
        self.vertices = {}
        self.cantidad = 0
        self.es_dirigido = es_dirigido

    def agregar_vertice(self, v):
        if v in self.vertices:
            return False
        self.vertices[v] = {}
        self.cantidad += 1

    def borrar_vertice(self, v):
        vertices = self.obtener_vertices()
        for w in vertices:
            if v in self.obtener_adyacentes(w):
                self.borrar_arista(v, w)
        self.vertices.pop(v)
        self.cantidad -= 1

    def agregar_arista(self, v, w, peso=1):
        """
        Agrega una arista entre 2 vertices.
        Si no existe alguno de los 2 vertices devuelve false.
        Si ya existe la arista devuelve false.
        """
        if v not in self.vertices or w not in self.vertices or v == w:
            return False
        if self.es_adyacente(v, w):
            return False
        
        self.vertices[v][w] = peso
        if not self.es_dirigido:
            self.vertices[w][v] = peso

    def borrar_arista(self, v, w):
        """
        Borra la arista entre 2 vertices.
        Si no existe alguno de los 2 vertices devuelve false.
        
        """
        if not self.es_adyacente(v,w):
            return False
        self.vertices[v].pop(w)
        if(not self.es_dirigido):
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
        if v not in self.vertices:
            return {}
        return self.vertices[v]

    def es_adyacente(self, v, w):
        """
        Devuelve True si 2 vertices estan unidos.
        Si no existe alguno de los 2 vertices devuelve false.
        """
        if v not in self.vertices or w not in self.vertices:
            return False
        if self.es_dirigido:
            return w in self.vertices[v]
        return w in self.vertices[v] and v in self.vertices[w]
