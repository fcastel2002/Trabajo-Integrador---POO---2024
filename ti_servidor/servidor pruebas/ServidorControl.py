import signal
from xmlrpc.server import SimpleXMLRPCServer
from ControladorRobot import ControladorRobot
import sys
import threading
import json

class ServidorControl:
    def __init__(self, consola, ip="127.0.0.1", puerto=9000):
        self.consola = consola
        self.ip = ip
        self.puerto = puerto
        self.server_thread = None
        self.server = None

    def iniciar(self):
        # Crear el servidor pero no ejecutarlo aún
        self.server = SimpleXMLRPCServer((self.ip, self.puerto), allow_none=True)
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
        self.server.register_function(self._serializar_comando(self.conectar_robot), "conectar")
        self.server.register_function(self._serializar_comando(self.desconectar_robot), "desconectar")
        self.server.register_function(self._serializar_comando(self.activar_motores), "activar_motores")
        self.server.register_function(self._serializar_comando(self.desactivar_motores), "desactivar_motores")
        self.server.register_function(self._serializar_comando(self.mover_efector), "mover_efector")
        self.server.register_function(self._serializar_comando(self.realizar_homming), "homming")

    def _serializar_comando(self, funcion):
        def wrapper(*args, **kwargs):
            # Serializamos el comando recibido
            comando_serializado = json.dumps({"args": args, "kwargs": kwargs})
            print(f"Comando recibido: {comando_serializado}")
            resultado = funcion(*args, **kwargs)
            return json.dumps(resultado)  # Devolvemos la respuesta como JSON
        return wrapper

    def disconnect(self):
        # Detener el servidor
        if self.server:
            print("Iniciando cierre del servidor...")
            self.server.shutdown()
            self.server.server_close()
            self.server_thread.join()  # Asegurarse de que el hilo termine
            print("Servidor cerrado correctamente.")

    # Robot control functions, leveraging the ControladorRobot instance
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
