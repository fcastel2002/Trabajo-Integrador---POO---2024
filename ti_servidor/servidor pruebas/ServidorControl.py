
from xmlrpc.server import SimpleXMLRPCServer
from ControladorRobot import ControladorRobot  # Include ControladorRobot class
import sys

class ServidorControl:
    def __init__(self, consola, ip="127.0.0.1", puerto=9000):
        self.consola = consola
        self.ip = ip
        self.puerto = puerto
        self.running = True  # Control flag to manage the server loop
        self._iniciar_servidor()

    def _iniciar_servidor(self):
        self.server = SimpleXMLRPCServer((self.ip, self.puerto), allow_none=True)
        self._registrar_funciones()
        self.run()

    def _registrar_funciones(self):
        # Registering the functions from ControladorRobot with the XML-RPC server
        self.server.register_function(self.conectar_robot, "conectar")
        self.server.register_function(self.desconectar_robot, "desconectar")
        self.server.register_function(self.activar_motores, "activar_motores")
        self.server.register_function(self.desactivar_motores, "desactivar_motores")
        self.server.register_function(self.mover_efector, "mover_efector")
        self.server.register_function(self.realizar_homming,"homming")

       def run(self):
        print("Servidor iniciado. Presione Ctrl+C para detener.")
        
        def signal_handler(sig, frame):
            print("Interrupción detectada. Cerrando el servidor...")
            self.running = False
            self.server.shutdown()  # Shutdown the server immediately
            self.disconnect()
            sys.exit(0)

        signal.signal(signal.SIGINT, signal_handler)

        while self.running:
            self.server.handle_request()

    def disconnect(self):
        print("Iniciando cierre del servidor...")
        self.running = False  # Set the flag to stop the server loop
        self.server.server_close()  # Close the server socket
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
