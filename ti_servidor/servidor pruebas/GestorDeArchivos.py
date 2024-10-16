import json

class GestorDeArchivos:
    def __init__(self, nombre):
        self.nombre_archivo = nombre
        self.limpiar_archivo()
    
    def limpiar_archivo(self):
        with open(self.nombre_archivo, "w") as archivo:
            archivo.close()
    
    def guardar_linea(self, objeto_json):
        with open(self.nombre_archivo, "a") as archivo:
            archivo.write(objeto_json + "\n")
    
    def leer_contenido(self):
        with open(self.nombre_archivo, "r") as archivo:
            contenido = archivo.readlines()
            return [json.loads(linea.strip()) for linea in contenido]  # Deserializamos cada línea
    
    def contar_lineas(self):
        with open(self.nombre_archivo, "r") as archivo:
            return len(archivo.readlines())
