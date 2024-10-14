class GestorDeArchivos():
    def __init__(self, nombre):
        self.nombre_archivo = nombre
        self.limpiar_archivo()
    
    def limpiar_archivo(self):
        with open(self.nombre_archivo, "w") as archivo:
            archivo.close()
    
    def guardar_linea(self, texto):
        with open(self.nombre_archivo, "a") as archivo:
            archivo.write(texto + "\n")
    
    def leer_contenido(self):
        with open(self.nombre_archivo, "r") as archivo:
            return archivo.read()
    
    def contar_lineas(self):
        with open(self.nombre_archivo, "r") as archivo:
            return len(archivo.readlines())
