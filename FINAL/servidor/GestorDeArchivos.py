import json
import csv
from ManejadorErrores import ErrorArchivos

class GestorDeArchivos:
    def __init__(self, nombre):
        self.nombre_archivo = nombre

    def limpiar_archivo(self):
        """Limpia el contenido del archivo, dejándolo vacío."""
        try:
            with open(self.nombre_archivo, "w") as archivo:
                archivo.close()
        except IOError:
            raise ErrorArchivos(1, self.nombre_archivo)

    def guardar_linea(self, objeto_json):
        """Guarda una línea en el archivo."""
        try:
            with open(self.nombre_archivo, "a") as archivo:
                archivo.write(objeto_json + "\n")
        except IOError:
            raise ErrorArchivos(1, self.nombre_archivo)

    def leer_contenido(self):
        """Lee y deserializa el contenido JSON del archivo."""
        try:
            with open(self.nombre_archivo, "r") as archivo:
                contenido = archivo.readlines()
                return [json.loads(linea.strip()) for linea in contenido]
        except FileNotFoundError:
            raise ErrorArchivos(2, self.nombre_archivo)
        except PermissionError:
            raise ErrorArchivos(3, self.nombre_archivo)
        except json.JSONDecodeError:
            raise ErrorArchivos(4)

    def contar_lineas(self):
        """Cuenta el número de líneas en el archivo."""
        try:
            with open(self.nombre_archivo, "r") as archivo:
                return len(archivo.readlines())
        except FileNotFoundError:
            raise ErrorArchivos(2, self.nombre_archivo)
        except PermissionError:
            raise ErrorArchivos(3, self.nombre_archivo)

    def leer_ultimas_lineas(self, n):
        """Lee las últimas 'n' líneas del archivo."""
        try:
            with open(self.nombre_archivo, "r") as archivo:
                lineas = archivo.readlines()
                return lineas[-n:] if len(lineas) >= n else lineas
        except FileNotFoundError:
            raise ErrorArchivos(2, self.nombre_archivo)
        except PermissionError:
            raise ErrorArchivos(3, self.nombre_archivo)

    def leer_csv(self):
        """Lee el contenido de un archivo CSV y lo devuelve como una lista de listas."""
        try:
            with open(self.nombre_archivo, newline='') as archivo_csv:
                lector_csv = csv.reader(archivo_csv)
                return list(lector_csv)
        except FileNotFoundError:
            raise ErrorArchivos(2, self.nombre_archivo)
        except PermissionError:
            raise ErrorArchivos(3, self.nombre_archivo)
        except csv.Error:
            raise ErrorArchivos(4)
    
    def leer_archivo(self):
        """Lee todas las líneas del archivo de texto."""
        try:
            with open(self.nombre_archivo, "r") as archivo:
                lineas = archivo.readlines()
                return lineas
        except FileNotFoundError:
            raise ErrorArchivos(2, self.nombre_archivo)
        except PermissionError:
            raise ErrorArchivos(3, self.nombre_archivo)
        
    def escribir_archivo(self, linea):
        """Escribe una línea en el archivo."""
        try:
            with open(self.nombre_archivo, "a") as archivo:
                archivo.write(linea + "\n")
        except IOError:
            raise ErrorArchivos(1, self.nombre_archivo)