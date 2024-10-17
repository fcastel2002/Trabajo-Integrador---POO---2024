import json
import csv

class GestorDeArchivos:
    def __init__(self, nombre):
        self.nombre_archivo = nombre

    def limpiar_archivo(self):
        """Limpia el contenido del archivo, dejándolo vacío."""
        with open(self.nombre_archivo, "w") as archivo:
            archivo.close()

    def guardar_linea(self, objeto_json):
        """Guarda una línea en el archivo."""
        with open(self.nombre_archivo, "a") as archivo:
            archivo.write(objeto_json + "\n")

    def leer_contenido(self):
        """Lee y deserializa el contenido JSON del archivo."""
        with open(self.nombre_archivo, "r") as archivo:
            contenido = archivo.readlines()
            return [json.loads(linea.strip()) for linea in contenido]

    def contar_lineas(self):
        """Cuenta el número de líneas en el archivo."""
        with open(self.nombre_archivo, "r") as archivo:
            return len(archivo.readlines())

    def leer_ultimas_lineas(self, n):
        """Lee las últimas 'n' líneas del archivo."""
        with open(self.nombre_archivo, "r") as archivo:
            lineas = archivo.readlines()
            return lineas[-n:] if len(lineas) >= n else lineas

    def leer_csv(self):
        """Lee el contenido de un archivo CSV y lo devuelve como una lista de listas."""
        with open(self.nombre_archivo, newline='') as archivo_csv:
            lector_csv = csv.reader(archivo_csv)
            return list(lector_csv)