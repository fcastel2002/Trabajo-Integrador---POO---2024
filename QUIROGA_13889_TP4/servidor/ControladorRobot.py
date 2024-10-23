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
        if self.estado_conexion == "desconectado":
            raise ErrorDeEstado(4)  # Conexión ya inactiva
        self.serial_robot.close()
        self.estado_conexion = "desconectado"
        return "Exito: Conexion terminada\n"

    def activar_motores(self):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if self.motores_activos:
            raise ErrorDeEstado(2)  # Motores ya encendidos

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
        self.ordenes_totales += 1
        comando_json = json.dumps({"comando": comando, "timestamp": time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())})
        self.archivo_ordenes_solicitadas.guardar_linea(comando_json)

        self.serial_robot.write((comando + "\r\n").encode())

        # Limpiar el buffer de entrada antes de leer la respuesta para asegurarnos de no tener residuos de comandos anteriores
        self.serial_robot.reset_input_buffer()

        respuestas = []
        time.sleep(2)
        while self.serial_robot.in_waiting > 0:
            respuesta = self.serial_robot.readline().decode().strip()  # Descartar cualquier mensaje residual
            respuestas.append(respuesta)
        
        if not respuestas:
            self.errores.append(comando)
            return {"error": "No se recibio respuesta del robot"}

        for respuesta in respuestas:
            if "ERROR" in respuesta.upper():
                self.errores.append(comando)
                return {"error": respuesta}

            try:
                self.archivo_ordenes_ejecutadas.guardar_linea(f"{comando} -> {respuesta}")
            except Exception as e:
                self.errores.append(comando)
                return {"error": f"{e}"}

        # Agregar el comando al archivo de aprendizaje si está activo
        if self.aprendiendo and self.archivo_aprendizaje:
            self.archivo_aprendizaje.guardar_linea(comando)

        return {"respuesta_robot": respuestas}


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

        if activar:
            if self.aprendiendo:
                raise ErrorDeEstado(7)  # Ya está en modo aprendizaje
            self.aprendiendo = True
            self.archivo_aprendizaje = GestorDeArchivos(f"{nombre_archivo}.txt")
            return "Modo aprendizaje activado\n"
        else:
            if not self.aprendiendo:
                return "El robot ya esta fuera del modo aprendizaje\n"
            self.aprendiendo = False
            self.archivo_aprendizaje = None  # Eliminar la referencia
            return "Modo aprendizaje desactivado\n"

    def ejecutar_automatico(self, nombre_archivo):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)
        if not self.motores_activos:
            raise ErrorDeConexion(2)
        if self.aprendiendo:
            raise ErrorDeEstado(9)

        archivo_control_automatico = GestorDeArchivos(f"{nombre_archivo}.txt")
        try:
            comandos = archivo_control_automatico.leer_archivo()
        except Exception:
            raise ErrorArchivos(2,nombre_archivo)

        resultados = []
        for comando in comandos:
            comando = comando.strip()
            respuestas = self._registrar_comando(comando)
            if "error" in respuestas:
                resultados.append(f"Comando: {comando} - Error: {respuestas['error']}")
            else:
                for respuesta in respuestas['respuesta_robot']:
                    resultados.append(f"Comando: {comando} - Respuesta: {respuesta}")

        resultados.append("Ejecucion automatica completada\n")
        return "\n".join(resultados)

    def mover_efector(self, x, y, z, velocidad):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados

        gcode = f"G1 X{x} Y{y} Z{z} F{velocidad}"
        resultado = self._registrar_comando(gcode)
        mensajes = []
        if "error" in resultado:
            mensajes.append(resultado["error"])
            return mensajes
        mensajes.append(f"Exito: Efector movido a (X={x}, Y={y}, Z={z}) con velocidad {velocidad}\n")
        return mensajes
    
    def mover_efector_posicion(self, x, y, z):
        """Mueve el efector a una posición especifica, sin pedir velocidad"""
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados

        # Usar una velocidad por defecto, por ejemplo, 1000
        velocidad_default = 1000
        gcode = f"G1 X{x} Y{y} Z{z} F{velocidad_default}"
        resultado = self._registrar_comando(gcode)
        mensajes = []
        if "error" in resultado:
            mensajes.append(resultado["error"])
            return mensajes
        mensajes.append(f"Exito: Efector movido a (X={x}, Y={y}, Z={z}) con velocidad {velocidad_default}\n")
        return mensajes

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
        
        return f"Exito: Efector {'activado' if accion == '1' else 'desactivado'}\n"

    def homming(self):
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        if not self.motores_activos:
            raise ErrorDeConexion(2)  # Motores apagados

        resultado = self._registrar_comando("G28")
        mensajes = []
        if "error" in resultado:
            mensajes.append(resultado["error"])
            return mensajes
        
        self.homming_realizado = True  # Actualizamos el estado de homming
        mensajes.append("Éxito: Homming realizado\n")
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
        
        return "Éxito: Efector movido a la posición de origen\n"

    def reportar_posicion(self):
        """Reporte la posición actual y el modo de coordenadas"""
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión

        mensajes = []
        respuestas = self._registrar_comando("M114")

        if "error" in respuestas:
            mensajes.append(respuestas["error"])
            return mensajes
        
        for respuesta in respuestas['respuesta_robot']:
            mensajes.append(respuesta)
        return mensajes

    def modo_absoluto(self):
        """Cambiar a modo de coordenadas absolutas"""
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión

        resultado = self._registrar_comando("G90")
        mensajes = []
        if "error" in resultado:
            mensajes.append(resultado["error"])
            return mensajes
        mensajes.append(resultado)
        mensajes.append("Modo de coordenadas absolutas activado\n")
        return mensajes

    def modo_relativo(self):
        """Cambiar a modo de coordenadas relativas"""
        if self.estado_conexion == "desconectado":
            raise ErrorDeConexion(1)  # No hay conexión
        
        resultado = self._registrar_comando("G91")
        mensajes = []
        if "error" in resultado:
            mensajes.append(resultado["error"])
            return mensajes
        mensajes.append(resultado)
        mensajes.append("Modo relativo activado\n")
        return mensajes
