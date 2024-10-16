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
        return self._registrar_comando(gcode)

    def desactivar_motores(self):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeEstado(1)  # Motores ya apagados

        self.motores_activos = False
        gcode = "M18"
        return self._registrar_comando(gcode)

    def _registrar_comando(self, comando):
        self.ordenes_totales += 1  # Incrementar contador de órdenes
        comando_json = json.dumps({"comando": comando, "timestamp": time.time()})
        self.archivo_ordenes_solicitadas.guardar_linea(comando_json)
        
        # Enviar comando al robot
        self.serial_robot.write((comando + "\r\n").encode())
        
        # Leer la respuesta del robot
        respuesta = self.serial_robot.readline().decode().strip()
        
        if not respuesta:
            self.errores.append(comando)  # Registrar el comando que produjo un error
            return {"error": "No se recibió respuesta del robot"}

        # Verificar si la respuesta contiene un error
        if "ERROR" in respuesta.upper():
            self.errores.append(comando)  # Registrar el comando como erróneo
            return {"error": respuesta}

        try:
            self.archivo_ordenes_ejecutadas.guardar_linea(f"{comando} -> {respuesta}")
        except Exception as e:
            self.errores.append(comando)  # Registrar el comando que produjo un error
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
        resultado = self._registrar_comando(gcode)
        if "error" in resultado:
            return resultado["error"]
        
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
        resultado = self._registrar_comando(gcode)
        if "error" in resultado:
            return resultado["error"]
        
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

        resultado = self._registrar_comando(gcode)
        if "error" in resultado:
            return resultado["error"]
        
        return f"Éxito: Efector {'activado' if accion == '1' else 'desactivado'}\n"

    def homming(self):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados

        resultado = self._registrar_comando("G28")
        if "error" in resultado:
            return resultado["error"]
        
        return "Éxito: Homming realizado\n"

    def mover_a_origen(self):
        """Mueve el efector a la posición de origen/descanso"""
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados

        resultado = self._registrar_comando("G28")  # Comando de retorno al origen
        if "error" in resultado:
            return resultado["error"]
        
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
            resultado = self._registrar_comando(comando.strip())
            if "error" in resultado:
                return f"Fallo en ejecución automática: {resultado['error']}"

        return "Éxito: Ejecución automática completada\n"
