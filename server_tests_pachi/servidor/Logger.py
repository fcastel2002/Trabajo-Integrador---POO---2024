import csv
import time

class Logger:
    def __init__(self, nombre_archivo="log_trabajo.csv"):
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
            with open(self.nombre_archivo, "a", newline='', encoding='utf-8') as archivo_log:
                escritor_csv = csv.DictWriter(archivo_log, fieldnames=["peticion", "ip", "usuario", "resultado", "timestamp"])

                archivo_log.seek(0, 2)
                if archivo_log.tell() == 0:
                    escritor_csv.writeheader()

                escritor_csv.writerow(log_entrada)
        except IOError as e:
            print(f"Error al abrir o escribir en el {self.nombre_archivo}: {e}")
        except Exception as e:
            print(f"Error general al registrar en el log: {e}")
