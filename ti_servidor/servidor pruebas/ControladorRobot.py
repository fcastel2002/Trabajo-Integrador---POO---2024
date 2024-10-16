import serial
import time
import json
from ManejadorErrores import ErrorDeConexion, ErrorDeParametros, ErrorDeEstado
from GestorDeArchivos import GestorDeArchivos

class ControladorRobot:
    def __init__(self, puerto_serial, baudios):
        self.puerto_serial = puerto_serial
        self.baudios = baudios
        self.estado_conexion = "desconectado"
        self.motores_activos = False
        self.efector_estado = "desactivado"
        self.aprendiendo = False
        self.actividad = "inactivo"
        self.archivo_ordenes_ejecutadas = GestorDeArchivos("ordenes_ejecutadas.txt")
        self.archivo_ordenes_solicitadas = GestorDeArchivos("ordenes_solicitadas.txt")
        self.inicio_actividad = None  # Almacenar cuando se inicia la actividad
        self.ordenes_totales = 0  # Contador de órdenes solicitadas
        self.errores = []  # Almacenar órdenes que produjeron errores

    def conectar(self):
        if self.estado_conexion == "conectado":
            raise ErrorDeEstado(3)  # Conexión ya activa
        try:
            self.serial_robot = serial.Serial(self.puerto_serial, self.baudios)
            self.estado_conexion = "conectado"
            self.serial_robot.timeout = 2
            return "Éxito: Conexión establecida\n"
        except Exception:
            return "Error: No se pudo conectar. Verifique la conexión física con el robot.\n"

    def desconectar(self):
        if self.estado_conexion == "desconectado":
            raise ErrorDeEstado(4)  # Conexión ya inactiva
        self.serial_robot.close()
        self.estado_conexion = "desconectado"
        return "Éxito: Conexión terminada\n"

    def activar_motores(self):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if self.motores_activos:
            raise ErrorDeEstado(2)  # Motores ya encendidos

        # Registro de inicio de actividad
        if self.inicio_actividad is None:
            self.inicio_actividad = time.time()

        self.motores_activos = True
        gcode = "M17"
        self._registrar_comando(gcode)
        return "Éxito: Motores activados\n"

    def desactivar_motores(self):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeEstado(1)  # Motores ya apagados

        self.motores_activos = False
        gcode = "M18"
        self._registrar_comando(gcode)
        return "Éxito: Motores desactivados\n"

    def _registrar_comando(self, comando):
        self.ordenes_totales += 1  # Incrementar contador de órdenes
        comando_json = json.dumps({"comando": comando, "timestamp": time.time()})
        self.archivo_ordenes_solicitadas.guardar_linea(comando_json)
        self.serial_robot.write((comando + "\r\n").encode())
        
        respuesta = self.serial_robot.readline().decode().strip()
        
        if not respuesta:
            self.errores.append(comando)  # Registrar el comando que produjo un error
            print("Error: Respuesta vacía recibida del robot.")
            return {"error": "No se recibió respuesta del robot"}

        try:
            self.archivo_ordenes_ejecutadas.guardar_linea(f"{comando} -> {respuesta}")
        except Exception as e:
            self.errores.append(comando)  # Registrar el comando que produjo un error
            print(f"Error al guardar en ordenes ejecutadas: {e}")
            return {"error": f"No se pudo guardar la respuesta: {e}"}

        return {"respuesta_robot": respuesta}

    def reportar(self):
        reporte = f"Estado de conexión: {self.estado_conexion}\n"
        reporte += f"Estado de actividad: {'Activo' if self.motores_activos else 'Inactivo'}\n"
        if self.inicio_actividad:
            reporte += f"Actividad iniciada en: {time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(self.inicio_actividad))}\n"
        reporte += f"Número de órdenes solicitadas: {self.ordenes_totales}\n"
        reporte += f"Número de errores: {len(self.errores)}\n"
        if self.errores:
            reporte += f"Órdenes con errores: {self.errores}\n"
        reporte += f"Número de órdenes ejecutadas: {self.archivo_ordenes_ejecutadas.contar_lineas()}\n"
        return reporte

    def mover_efector(self, x, y, z, velocidad):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados

        gcode = f"G1 X{x} Y{y} Z{z} F{velocidad}"
        self._registrar_comando(gcode)
        return f"Éxito: Efector movido a (X={x}, Y={y}, Z={z}) con velocidad {velocidad}\n"
    
    def mover_efector_posicion(self, x, y, z):
        """Mueve el efector a una posición específica, sin pedir velocidad"""
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados

        # Usar una velocidad por defecto, por ejemplo, 1000
        velocidad_default = 1000
        gcode = f"G1 X{x} Y{y} Z{z} F{velocidad_default}"
        self._registrar_comando(gcode)
        return f"Éxito: Efector movido a (X={x}, Y={y}, Z={z}) con velocidad {velocidad_default}\n"

    def actuar_efector(self, accion):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados

        if accion == '1':
            if self.efector_estado == "activado":
                raise ErrorDeEstado(5)  # Efector ya activado
            gcode = "M3"
            self.efector_estado = "activado"
        elif accion == '0':
            if self.efector_estado == "desactivado":
                raise ErrorDeEstado(6)  # Efector ya desactivado
            gcode = "M5"
            self.efector_estado = "desactivado"
        else:
            raise ErrorDeParametros(3)  # Parámetros inválidos

        self._registrar_comando(gcode)
        return f"Éxito: Efector {'activado' if accion == '1' else 'desactivado'}\n"

    def homming(self):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados

        gcode = "G28"
        self._registrar_comando(gcode)
        return "Éxito: Homming realizado\n"

    def mover_a_origen(self):
        """Mueve el efector a la posición de origen/descanso"""
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados

        gcode = "G28"  # Comando de retorno al origen
        self._registrar_comando(gcode)
        return "Éxito: Efector movido a la posición de origen\n"

    def aprender(self, nombre_archivo, activar):
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados

        if activar:
            if self.aprendiendo:
                raise ErrorDeEstado(7)  # Ya está en modo aprendizaje
            self.aprendiendo = True
            self.archivo_aprendizaje = GestorDeArchivos(f"{nombre_archivo}.txt")
            return "Modo aprendizaje activado\n"
        else:
            if not self.aprendiendo:
                raise ErrorDeEstado(8)  # No está en modo aprendizaje
            self.aprendiendo = False
            return "Modo aprendizaje desactivado\n"

    def ejecutar_automatico(self, nombre_archivo):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados
        if self.aprendiendo:
            raise ErrorDeEstado(9)  # Está en modo aprendizaje

        with open(f"{nombre_archivo}.txt", "r") as archivo:
            comandos = archivo.readlines()

        for comando in comandos:
            self._registrar_comando(comando.strip())

        return "Éxito: Ejecución automática completada\n"