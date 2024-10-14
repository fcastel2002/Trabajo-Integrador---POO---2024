from xmlrpc.server import SimpleXMLRPCServer
from threading import Thread
from ControladorRobot import ControladorRobot

class ServidorControl:
    def __init__(self, consola, ip="localhost", puerto=9000):
        self.consola = consola
        self.ip = ip
        self.puerto = puerto
        self._iniciar_servidor()

    def _iniciar_servidor(self):
        self.server = SimpleXMLRPCServer((self.ip, self.puerto), allow_none=True)
        self._registrar_funciones()

    def _registrar_funciones(self):
        self.server.register_function(self.conectar_robot, "conectar")
        self.server.register_function(self.desconectar_robot, "desconectar")
        self.server.register_function(self.activar_motores, "activar_motores")
        self.server.register_function(self.desactivar_motores, "desactivar_motores")
        self.server.register_function(self.mover_efector, "mover_efector")
    
    def disconnect(self):
        self.server.shutdown()
        self.server.socket.close()
    
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

    def run(self):
        self.server.serve_forever()
