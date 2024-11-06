import sys
import time
from GestorDeArchivos import GestorDeArchivos

class Logger:
    def __init__(self, nombre_archivo="log_trabajo.csv"):
        self.gestor_archivos = GestorDeArchivos(nombre_archivo)
        self.nombre_archivo = nombre_archivo

    def registrar_log(self, peticion, ip, usuario, exito):
        log_entrada = {
            "peticion": peticion,
            "ip": ip,
            "usuario": usuario,
            "resultado": "exitoso" if exito else "fallido",
            "timestamp": time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())
        }

        try:
            # Verificar si el archivo está vacío y escribir encabezados si es necesario
            try:
                contenido = self.gestor_archivos.leer_archivo()
                if not contenido:
                    raise Exception("Archivo vacío")
            except Exception:
                # Si el archivo no existe o está vacío, crear y escribir los encabezados
                encabezados = "peticion,ip,usuario,resultado,timestamp"
                self.gestor_archivos.escribir_archivo(encabezados)

            # Convertir el diccionario en una línea CSV y guardarla
            linea_csv = f'{log_entrada["peticion"]},{log_entrada["ip"]},{log_entrada["usuario"]},{log_entrada["resultado"]},{log_entrada["timestamp"]}'
            self.gestor_archivos.escribir_archivo(linea_csv)

        except IOError as e:
            print(f"Error al abrir o escribir en el {self.nombre_archivo}: {e}")
            sys.exit(1)
        except Exception as e:
            print(f"Error general al registrar en el log: {e}")
            sys.exit(1)