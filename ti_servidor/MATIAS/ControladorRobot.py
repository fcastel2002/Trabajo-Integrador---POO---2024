import serial, time
from ManejadorErrores import ErrorDeConexion, ErrorDeParametros, ErrorDeEstado
from GestionArchivos import GestorDeArchivos

class ControladorRobot:
    def __init__(self, puerto, baudios):
        self.puerto = puerto
        self.baudios = baudios
        self.estado_conexion = "desconectado"
        self.motores_activos = False
        self.archivo_ordenes = GestorDeArchivos("ordenes.txt")

    def conectar(self):
        if self.estado_conexion == "conectado":
            raise ErrorDeEstado(3)
        try:
            self.serial_robot = serial.Serial(self.puerto, self.baudios)
            self.estado_conexion = "conectado"
            return "Conexión exitosa."
        except:
            return "Error: No se pudo conectar. Verifique los cables."

    def desconectar(self):
        if self.estado_conexion == "desconectado":
            raise ErrorDeEstado(4)
        self.serial_robot.close()
        self.estado_conexion = "desconectado"
        return "Desconexión completada."

    def activar_motores(self):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)
        if self.motores_activos:
            raise ErrorDeEstado(2)
        
        self.motores_activos = True
        comando = "M17"
        self._enviar_comando(comando)
        return "Motores encendidos."

    def desactivar_motores(self):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)
        if not self.motores_activos:
            raise ErrorDeEstado(1)
        
        self.motores_activos = False
        comando = "M18"
        self._enviar_comando(comando)
        return "Motores apagados."

    def mover_efector(self, x, y, z, velocidad):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)
        comando = f"G1 X{x} Y{y} Z{z} F{velocidad}"
        self._enviar_comando(comando)
        return f"Moviendo efector a X:{x}, Y:{y}, Z:{z} con velocidad {velocidad}."

    def _enviar_comando(self, comando):
        self.archivo_ordenes.guardar_linea(comando)
        self.serial_robot.write((comando + "\n").encode())
        respuesta = self.serial_robot.readline().decode().strip()
        self.archivo_ordenes.guardar_linea("Respuesta: " + respuesta)
