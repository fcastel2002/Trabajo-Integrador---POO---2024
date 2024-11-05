import serial
import time
import json
from ManejadorErrores import ErrorDeConexion, ErrorDeParametros, ErrorDeEstado, ErrorArchivos
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
        self.tipo_movimiento = 'absoluto'
        self.homming_realizado = False  # Nueva variable para controlar el estado del homming
        self.archivo_ordenes_ejecutadas = GestorDeArchivos("ordenes_ejecutadas.txt")
        self.archivo_ordenes_solicitadas = GestorDeArchivos("ordenes_solicitadas.txt")
        self.archivo_aprendizaje = None
        self.inicio_actividad = None  # Almacenar cuando se inicia la actividad
        self.ordenes_totales = 0  # Contador de órdenes solicitadas
        self.errores = []  # Almacenar órdenes que produjeron errores

    def conectar(self):
        mensajes = []
        if self.estado_conexion == "conectado":
            raise ErrorDeEstado(3)  # Conexión ya activa
        try:
            self.serial_robot = serial.Serial(self.puerto_serial, self.baudios)
            self.serial_robot.timeout = 2
            self.estado_conexion = "conectado"

            # Limpiar el buffer de recepción al iniciar la conexión
            self.serial_robot.reset_input_buffer()
            self.serial_robot.reset_output_buffer()

            # Leer mensajes iniciales y descartarlos
            time.sleep(2)  # Esperar un poco para que lleguen los mensajes iniciales
            while self.serial_robot.in_waiting > 0:
                mensaje = self.serial_robot.readline().decode().strip()  # Descartar cualquier mensaje residual
                mensajes.append(mensaje)

            mensajes.append("Exito: Conexion establecida\n")
            return mensajes
        except Exception:
            raise ErrorDeConexion(1)

    def desconectar(self):
        mensajes = []
        if self.estado_conexion == "desconectado":
            raise ErrorDeEstado(4)  # Conexión ya inactiva
        self.serial_robot.close()
        self.estado_conexion = "desconectado"
        mensajes.append("Exito: Conexion terminada\n")
        return mensajes

    def activar_motores(self):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if self.motores_activos:
            raise ErrorDeEstado(2)  # Motores ya encendidos

        if self.inicio_actividad is None:
            self.inicio_actividad = time.time()

        self.motores_activos = True
        gcode = "M17"
        msjs = []
        try:
            respuestas = self._registrar_comando(gcode)
            for respuesta in respuestas:
                msjs.append(respuesta)
            return msjs
        except Exception as e:
            self.motores_activos = False
            raise e

    def desactivar_motores(self):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeEstado(1)  # Motores ya apagados

        self.motores_activos = False
        gcode = "M18"
        mensajes = []
        try:
            respuestas = self._registrar_comando(gcode)
            for respuesta in respuestas:
                mensajes.append(respuesta)
            return mensajes
        except Exception:
            self.motores_activos = True
            raise Exception

    def _registrar_comando(self, comando):
        self.ordenes_totales += 1
        comando_json = json.dumps({"comando": comando, "timestamp": time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())})
        self.archivo_ordenes_solicitadas.guardar_linea(comando_json)

        try:
            self.serial_robot.write((comando + "\r\n").encode())
        except Exception:
            raise ErrorDeConexion(1)

        # Limpiar el buffer de entrada antes de leer la respuesta para asegurarnos de no tener residuos de comandos anteriores
        self.serial_robot.reset_input_buffer()

        respuestas = []
        time.sleep(2)
        while self.serial_robot.in_waiting > 0:
            respuesta = self.serial_robot.readline().decode().strip()  # Descartar cualquier mensaje residual
            respuestas.append(respuesta)
        
        if not respuestas:
            self.errores.append(comando)
            respuestas.append("No se recibio respuesta del robot")
            return respuestas

        for respuesta in respuestas:
            if "ERROR" in respuesta.upper():
                self.errores.append(comando)
                respuestas.append(respuesta)
                return respuestas

            try:
                self.archivo_ordenes_ejecutadas.guardar_linea(f"{comando} -> {respuesta}")
            except Exception as e:
                self.errores.append(comando)
                return f"{e}"

        # Agregar el comando al archivo de aprendizaje si está activo
        if self.aprendiendo and self.archivo_aprendizaje:
            self.archivo_aprendizaje.guardar_linea(comando)

        return respuestas


    def reportar(self):
        reporte = f"Estado de conexion: {self.estado_conexion}\n"
        reporte += f"Estado de actividad: {'Activo' if self.motores_activos else 'Inactivo'}\n"
        if self.inicio_actividad:
            reporte += f"Actividad iniciada en: {time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(self.inicio_actividad))}\n"
        reporte += f"Numero de ordenes solicitadas: {self.ordenes_totales}\n"
        reporte += f"Numero de errores: {len(self.errores)}\n"
        if self.errores:
            reporte += f"Ordenes con errores: {self.errores}\n"
        reporte += f"Numero de ordenes ejecutadas: {self.archivo_ordenes_ejecutadas.contar_lineas()}\n"
        return reporte

    def aprender(self, nombre_archivo, activar):
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados
        mensajes = []
        if activar == "Iniciar":
            if self.aprendiendo:
                raise ErrorDeEstado(7)  # Ya está en modo aprendizaje
            self.aprendiendo = True
            self.archivo_aprendizaje = GestorDeArchivos(f"{nombre_archivo}.txt")
            mensajes.append("Modo aprendizaje activado\n")
            return mensajes
        else:
            if not self.aprendiendo:
                mensajes.append("El robot ya esta fuera del modo aprendizaje\n")
                return mensajes
            self.aprendiendo = False
            self.archivo_aprendizaje = None  # Eliminar la referencia
            mensajes.append("Modo aprendizaje desactivado\n")
            return mensajes

    def ejecutar_automatico(self, nombre_archivo, archivo = None):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)
        if not self.motores_activos:
            raise ErrorDeConexion(2)
        if self.aprendiendo:
            raise ErrorDeEstado(9)

        if archivo is not None:
            comandos = archivo
            archivo_control_automatico = GestorDeArchivos(f"archivoRecibido.txt")
        else:
            archivo_control_automatico = GestorDeArchivos(f"{nombre_archivo}.txt")
            try:
                comandos = archivo_control_automatico.leer_archivo()
            except Exception:
                raise ErrorArchivos(2,nombre_archivo)

        respuestas_usuario = []
        resultados = []
        for comando in comandos:
            comando = comando.strip()
            if archivo is not None:
                archivo_control_automatico.escribir_archivo(comando)
            respuestas = self._registrar_comando(comando)
            for respuesta in respuestas:
                resultados.append(respuesta)
                
        for i,resultado in enumerate(resultados):
            if "error" in resultado.lower():
                respuestas_usuario.append(f"Comando: {comandos[i]} - Error: {resultado}")
            else:
                respuestas_usuario.append(f"Comando: {comandos[i]} - Respuesta: {resultado}")

        respuestas_usuario.append("Ejecucion automatica completada\n")
        return respuestas_usuario

    def mover_efector(self, x, y, z, velocidad = None):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados
        if velocidad:
            gcode = f"G1 X{x} Y{y} Z{z} F{velocidad}"
            respuesta_servidor = f"Exito: Efector movido a (X={x}, Y={y}, Z={z}) con velocidad {velocidad}\n"
        else:
            gcode = f"G1 X{x} Y{y} Z{z}"
            respuesta_servidor = f"Exito: Efector movido a (X={x}, Y={y}, Z={z}) \n"
        resultados = self._registrar_comando(gcode)
        mensajes = []
        for resultado in resultados:
            if "error" in resultado.lower():
                mensajes.append(resultado)
                mensajes.append("Debe respetar los limites del espacio de trabajo del robot")
                return mensajes
        mensajes.append(respuesta_servidor)
        return mensajes

    def actuar_efector(self, accion):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados

        if accion.lower() == 'activar':
            if self.efector_estado == "activado":
                raise ErrorDeEstado(5)  # Efector ya activado
            gcode = "M3"
            self.efector_estado = "activado"
        elif accion.lower() == 'desactivar':
            if self.efector_estado == "desactivado":
                raise ErrorDeEstado(6)  # Efector ya desactivado
            gcode = "M5"
            self.efector_estado = "desactivado"
        else:
            raise ErrorDeParametros(3)  # Parámetros inválidos
        mensajes = []
        respuestas = self._registrar_comando(gcode)
        for respuesta in respuestas:
            mensajes.append(respuesta)
            if "error" in respuesta.lower():
                return mensajes
        mensajes.append(f"Exito: Efector {'activado' if accion == 'activar' else 'desactivado'}\n")
        
        return mensajes

    def homming(self):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados

        respuestas = self._registrar_comando("G28")
        mensajes = []
        for respuesta in respuestas:
            mensajes.append(respuesta)
            if "error" in respuesta:
                return mensajes
        
        self.homming_realizado = True  # Actualizamos el estado de homming
        mensajes.append("Exito: Homming realizado\n")
        return mensajes

    def mover_a_origen(self):
        """Mueve el efector a la posición de origen/descanso"""
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados

        resultado = self._registrar_comando("G28")  # Comando de retorno al origen
        if "error" in resultado:
            return resultado["error"]
        
        return "Exito: Efector movido a la posición de origen\n"

    def reportar_posicion(self):
        """Reporte la posición actual y el modo de coordenadas"""
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión

        mensajes = []
        respuestas = self._registrar_comando("M114")

        if "error" in respuestas:
            mensajes.append(respuestas)
            return mensajes
        
        for respuesta in respuestas:
            mensajes.append(respuesta)
        return mensajes

    def modo_absoluto(self):
        """Cambiar a modo de coordenadas absolutas"""
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión

        respuestas = self._registrar_comando("G90")
        mensajes = []
        for respuesta in respuestas:
            mensajes.append(respuesta)
            if "error" in respuesta:
                return mensajes
        mensajes.append("Modo de coordenadas absolutas activado\n")
        self.tipo_movimiento = 'absoluto'
        return mensajes

    def modo_relativo(self):
        """Cambiar a modo de coordenadas relativas"""
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        
        respuestas = self._registrar_comando("G91")
        mensajes = []
        for respuesta in respuestas:
            mensajes.append(respuesta)
            if "error" in respuesta:
                return mensajes
        mensajes.append("Modo relativo activado\n")
        self.tipo_movimiento = 'relativo'
        return mensajes
