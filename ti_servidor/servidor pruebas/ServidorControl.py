import signal
from xmlrpc.server import SimpleXMLRPCServer
from ControladorRobot import ControladorRobot
import sys
import threading
import json
import logging
import time

class ServidorControl:
    def __init__(self, consola, ip="127.0.0.1", puerto=9000):
        self.consola = consola
        self.ip = ip
        self.puerto = puerto
        self.server_thread = None
        self.server = None
        self.usuarios_autorizados = self._cargar_usuarios()  # Cargar usuarios desde archivo
        self.logs = []  # Lista para almacenar los logs de trabajo
        self.modo_trabajo = "manual"  # Modo por defecto

    def _cargar_usuarios(self):
        # Cargamos los usuarios desde un archivo JSON
        try:
            with open("usuarios.json", "r") as archivo:
                return json.load(archivo)
        except FileNotFoundError:
            print("No se encontró el archivo de usuarios.")
            return {}

    def _validar_usuario(self, usuario, clave):
        # Validamos que el usuario y la clave sean correctos
        if usuario in self.usuarios_autorizados and self.usuarios_autorizados[usuario] == clave:
            return True
        return False

    def _registrar_log(self, peticion, ip, usuario, exito):
        # Registra una entrada en el log del servidor
        log_entrada = {
            "peticion": peticion,
            "ip": ip,
            "usuario": usuario,
            "resultado": "exitoso" if exito else "fallido",
            "timestamp": time.time()
        }
        self.logs.append(log_entrada)

    def iniciar(self):
        # Crear el servidor pero no ejecutarlo aún
        self.server = SimpleXMLRPCServer((self.ip, self.puerto), allow_none=True, logRequests=False)
        self._registrar_funciones()
        self.server_thread = threading.Thread(target=self._iniciar_servidor)
        self.server_thread.daemon = True
        self.server_thread.start()
        print(f"Servidor RPC iniciado en {self.ip}:{self.puerto}\n")

    def _iniciar_servidor(self):
        try:
            self.server.serve_forever()
        except KeyboardInterrupt:
            self.server.shutdown()

    def _registrar_funciones(self):
        # Registrar las funciones con validación y serialización
        self.server.register_function(self._serializar_comando(self.conectar_robot), "conectar")
        self.server.register_function(self._serializar_comando(self.desconectar_robot), "desconectar")
        self.server.register_function(self._serializar_comando(self.activar_motores), "activar_motores")
        self.server.register_function(self._serializar_comando(self.desactivar_motores), "desactivar_motores")
        self.server.register_function(self._serializar_comando(self.mover_efector), "mover_efector")
        self.server.register_function(self._serializar_comando(self.realizar_homming), "homming")
        self.server.register_function(self.listar_comandos, "listar_comandos")
        self.server.register_function(self.mostrar_ayuda, "mostrar_ayuda")
        self.server.register_function(self.reportar_estado_servidor, "reportar_estado")
        self.server.register_function(self.reportar_log_trabajo, "reportar_log")

    def _serializar_comando(self, funcion):
        def wrapper(usuario, clave, *args, **kwargs):
            # Validar usuario y clave antes de ejecutar el comando
            if not self._validar_usuario(usuario, clave):
                return json.dumps({"error": "Acceso denegado: Usuario o clave incorrectos"})

            # Registrar el log de la petición
            self._registrar_log(funcion.__name__, "127.0.0.1", usuario, True)

            comando_serializado = json.dumps({"args": args, "kwargs": kwargs})
            print(f"Comando recibido de {usuario}: {comando_serializado}")
            resultado = funcion(*args, **kwargs)
            return json.dumps(resultado)  # Serializar la respuesta
        return wrapper

    def disconnect(self):
        # Detener el servidor
        if self.server:
            print("Iniciando cierre del servidor...")
            self.server.shutdown()
            self.server.server_close()
            self.server_thread.join()  # Asegurarse de que el hilo termine
            print("Servidor cerrado correctamente.")

    # Funciones del robot que se llamarán desde el servidor RPC
    def conectar_robot(self):
        return self.consola.conectar_robot()

    def desconectar_robot(self):
        return self.consola.desconectar_robot()

    def activar_motores(self):
        return self.consola.activar_motores()

    def desactivar_motores(self):
        return self.consola.desactivar_motores()

    def mover_efector(self, x, y, z, velocidad):
        return self.consola.mover_efector(x, y, z, velocidad)

    def realizar_homming(self):
        return self.consola.realizar_homming()

    # Listar los comandos disponibles
    def listar_comandos(self):
        comandos = [
            "conectar(usuario, clave)",
            "desconectar(usuario, clave)",
            "activar_motores(usuario, clave)",
            "desactivar_motores(usuario, clave)",
            "mover_efector(usuario, clave, x, y, z, velocidad)",
            "homming(usuario, clave)",
            "listar_comandos(usuario, clave)",
            "mostrar_ayuda(usuario, clave)",
            "reportar_estado(usuario, clave)",
            "reportar_log(usuario, clave)"
        ]
        return json.dumps({"comandos_disponibles": comandos})

    # Mostrar ayuda con ejemplos de sintaxis
    def mostrar_ayuda(self):
        ayuda = """
        Comandos disponibles y su sintaxis:

        - Conectar al Robot: conectar(usuario, clave)
        - Desconectar del Robot: desconectar(usuario, clave)
        - Activar Motores: activar_motores(usuario, clave)
        - Desactivar Motores: desactivar_motores(usuario, clave)
        - Mover Efector Final (con Velocidad): mover_efector(usuario, clave, x, y, z, velocidad)
        - Homming: homming(usuario, clave)
        - Reportar Estado del Servidor: reportar_estado(usuario, clave)
        - Listar Comandos Disponibles: listar_comandos(usuario, clave)
        - Reportar Log de Trabajo: reportar_log(usuario, clave)

        Ejemplo:
        Para conectar el robot:
        conectar("admin", "clave123")
        """
        return json.dumps({"ayuda": ayuda})

    # Reportar estado general del servidor (conexión, actividad, etc.)
    def reportar_estado_servidor(self):
        return self.consola.reportar_estado()

    # Reportar el log de trabajo del servidor (peticiones, IP, usuario, etc.)
    def reportar_log_trabajo(self):
        return json.dumps({"log_trabajo": self.logs})

    # Cambiar el modo de trabajo
    def cambiar_modo_trabajo(self, usuario, clave, nuevo_modo):
        if not self._validar_usuario(usuario, clave):
            return json.dumps({"error": "Acceso denegado: Usuario o clave incorrectos"})
        
        if nuevo_modo not in ["manual", "automatico"]:
            return json.dumps({"error": "Modo inválido. Debe ser 'manual' o 'automatico'."})
        
        self.modo_trabajo = nuevo_modo
        return json.dumps({"exito": f"Modo de trabajo cambiado a {self.modo_trabajo}"})