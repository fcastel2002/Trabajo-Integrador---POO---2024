import json
import csv

class GestorDeArchivos:
    def __init__(self, nombre):
        self.nombre_archivo = nombre
        self.limpiar_archivo()

    def limpiar_archivo(self):
        """Limpia el contenido del archivo, dejándolo vacío."""
        with open(self.nombre_archivo, "w") as archivo:
            archivo.close()

    def guardar_linea(self, objeto_json):
        """Guarda una línea en formato JSON en el archivo."""
        with open(self.nombre_archivo, "a") as archivo:
            archivo.write(objeto_json + "\n")

    def leer_contenido(self):
        """Lee y deserializa el contenido JSON del archivo."""
        with open(self.nombre_archivo, "r") as archivo:
            contenido = archivo.readlines()
            return [json.loads(linea.strip()) for linea in contenido]  # Deserializamos cada línea

    def contar_lineas(self):
        """Cuenta el número de líneas en el archivo."""
        with open(self.nombre_archivo, "r") as archivo:
            return len(archivo.readlines())

    def leer_ultimas_lineas(self, n):
        """Lee las últimas 'n' líneas del archivo."""
        with open(self.nombre_archivo, "r") as archivo:
            lineas = archivo.readlines()
            return lineas[-n:] if len(lineas) >= n else lineas

    def guardar_csv(self, datos):
        """Guarda los datos en formato CSV en el archivo."""
        with open(self.nombre_archivo, "w", newline='') as archivo_csv:
            escritor_csv = csv.writer(archivo_csv)
            escritor_csv.writerows(datos)

    def leer_csv(self):
        """Lee el contenido de un archivo CSV y lo devuelve como una lista de listas."""
        with open(self.nombre_archivo, newline='') as archivo_csv:
            lector_csv = csv.reader(archivo_csv)
            return list(lector_csv)

    def guardar_en_csv(self, datos, encabezados=None):
        """Guarda una lista de diccionarios en formato CSV con encabezados."""
        with open(self.nombre_archivo, "w", newline='') as archivo_csv:
            escritor_csv = csv.DictWriter(archivo_csv, fieldnames=encabezados)
            escritor_csv.writeheader()
            escritor_csv.writerows(datos)

    def leer_csv_con_encabezados(self):
        """Lee un archivo CSV con encabezados y devuelve una lista de diccionarios."""
        with open(self.nombre_archivo, newline='') as archivo_csv:
            lector_csv = csv.DictReader(archivo_csv)
            return list(lector_csv)