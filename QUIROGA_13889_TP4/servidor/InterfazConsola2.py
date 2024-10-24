import json
import questionary
from ControladorRobot import ControladorRobot
from ServidorControl4 import ServidorControl
from GestorDeArchivos import GestorDeArchivos
from ManejadorErrores import ErrorDeConexion, ErrorDeParametros, ErrorDeEstado
from Logger import Logger
import sys
import threading
import time

class InterfazConsola:
    def __init__(self, robot=None):
        self.robot = ControladorRobot('COM8', 115200) if robot == None else robot
        self.servidor_activado = False
        self.archivo_configuracion = "configuracion_robot.json"
        self.archivo_usuarios = "usuarios.json"
        self.archivo_logs = "log_trabajo.csv"
        self.modo_trabajo = "manual"
        self.tipo_movimiento = "absoluto"
        self.gestor_logs = GestorDeArchivos(self.archivo_logs)
        self.logger = Logger()
        self.evento_finalizacion = threading.Event()
        self.evento_creacion_Servidor = threading.Event()
        self.evento_cierre_Servidor = threading.Event()

    def mostrar_ayuda(self):
        ayuda = """
        Comandos disponibles y su sintaxis:

        - Conectar al Robot: conectar(usuario, clave)
        - Desconectar del Robot: desconectar(usuario, clave)
        - Activar Motores: activar_motores(usuario, clave)
        - Desactivar Motores: desactivar_motores(usuario, clave)
        - Mover Efector Final (con Velocidad): mover_efector(usuario, clave, x, y, z, velocidad)
        - Mover Efector Final (solo Posición): mover_efector_posicion(usuario, clave, x, y, z)
        - Homming: homming(usuario, clave)
        - Ejecución Automática: ejecutar_automatico(usuario, clave, nombre_archivo)
        - Reportar Posición Actual: reportar_posicion(usuario, clave)
        - Cambiar a Modo Absoluto: modo_absoluto(usuario, clave)
        - Cambiar a Modo Relativo: modo_relativo(usuario, clave)
        - Activar Efector: activar_efector(usuario, clave)
        - Desactivar Efector: desactivar_efector(usuario, clave)

        Ejemplo:
        Para conectar el robot:
        conectar("admin", "clave123")
        """
        print(ayuda)

    def iniciar(self):
        while True:
            opciones_menu = []

            if self.robot.estado_conexion == "desconectado":
                opciones_menu.append("Conectar al Robot")
            
            if self.servidor_activado is False:
                opciones_menu.append("Iniciar Servidor RPC")

            if self.robot.estado_conexion == "conectado":
                opciones_menu.append("Desconectar del Robot")
                opciones_menu.append("Listar Comandos Disponibles")
                opciones_menu.append("Mostrar/Editar Parámetros de Conexión")
                opciones_menu.append(f"Cambiar Modo de Trabajo (Actual: {self.modo_trabajo})")

                # Cambiar tipo de movimiento de acuerdo al estado actual
                if self.tipo_movimiento == "absoluto":
                    opciones_menu.append("Cambiar a Modo Relativo")
                else:
                    opciones_menu.append("Cambiar a Modo Absoluto")

                # Mostrar "Activar/Desactivar Motores"
                opciones_menu.append("Activar Motores" if not self.robot.motores_activos else "Desactivar Motores")

                if self.robot.motores_activos:
                    opciones_menu.append("Mover Efector Final (con Velocidad)")
                    opciones_menu.append("Mover Efector Final (solo Posición)")
                    opciones_menu.append("Homming")
                    opciones_menu.append("Aprendizaje (On/Off)")
                    opciones_menu.append("Ejecución Automática")
                    opciones_menu.append("Reportar Estado")
                    opciones_menu.append("Reportar Posición Actual")

                    # Mostrar solo la opción válida para el efector
                    if self.robot.efector_estado == "desactivado":
                        opciones_menu.append("Activar Efector")
                    else:
                        opciones_menu.append("Desactivar Efector")

            if self.servidor_activado is not False:
                opciones_menu.append("Detener Servidor RPC")
            
            opciones_menu.append("Mostrar las últimas 100 líneas del Log (Admin)")
            opciones_menu.append("Mostrar Ayuda")
            opciones_menu.append("Actualizar")
            opciones_menu.append("Salir")

            choice = questionary.select(
                "Seleccione una opción:",
                choices=opciones_menu
            ).ask()

            if choice == "Conectar al Robot":
                self.conectar_robot()
            elif choice == "Desconectar del Robot":
                self.desconectar_robot()
            elif choice == "Iniciar Servidor RPC":
                self.iniciar_servidor_rpc()
            elif choice == "Detener Servidor RPC":
                self.detener_servidor_rpc()
            elif choice == "Activar Motores":
                self.activar_motores()
            elif choice == "Desactivar Motores":
                self.desactivar_motores()
            elif choice == "Listar Comandos Disponibles":
                self.listar_comandos()
            elif choice == "Mostrar/Editar Parámetros de Conexión":
                self.mostrar_editar_parametros()
            elif choice == f"Cambiar Modo de Trabajo (Actual: {self.modo_trabajo})":
                self.cambiar_modo_trabajo()
            elif choice == "Cambiar a Modo Absoluto":
                self.modo_absoluto()
            elif choice == "Cambiar a Modo Relativo":
                self.modo_relativo()
            elif choice == "Mover Efector Final (con Velocidad)":
                self.mover_efector()
            elif choice == "Mover Efector Final (solo Posición)":
                self.mover_efector_posicion()
            elif choice == "Homming":
                self.realizar_homming()
            elif choice == "Aprendizaje (On/Off)":
                self.aprendizaje()
            elif choice == "Ejecución Automática":
                self.ejecutar_automatico()
            elif choice == "Reportar Estado":
                self.reportar_estado()
            elif choice == "Reportar Posición Actual":
                self.reportar_posicion()
            elif choice == "Activar Efector":
                self.activar_efector()
            elif choice == "Desactivar Efector":
                self.desactivar_efector()
            elif choice == "Mostrar las últimas 100 líneas del Log (Admin)":
                self.mostrar_log_admin()
            elif choice == "Mostrar Ayuda":
                self.mostrar_ayuda()
            elif choice == "Actualizar":
                continue
            elif choice == "Salir":
                self.salir()
                break
            else:
                break

    # Funciones para interactuar con el robot y servidor
    def conectar_robot(self):
        usuario = "consola_local"
        ip = "127.0.0.1"
        try:
            puerto = questionary.text("Ingrese el puerto COM (ejemplo: COM8):").ask()
            baudios = questionary.text("Ingrese la tasa de baudios (ejemplo: 115200):").ask()

            if puerto and baudios:
                self.robot.puerto_serial = puerto
                self.robot.baudios = int(baudios)
                mensajes = self.robot.conectar()
                for mensaje in mensajes:
                    print(mensaje)
                self.logger.registrar_log("conectar_robot", ip, usuario, True)
            else:
                print("Error: Debe ingresar un puerto COM y la tasa de baudios.")
                self.logger.registrar_log("conectar_robot", ip, usuario, False)
        except Exception as e:
            print(f"Error al conectar: {e}")
            self.logger.registrar_log("conectar_robot", ip, usuario, False)

    def desconectar_robot(self):
        usuario = "consola_local"
        ip = "127.0.0.1"
        try:
            mensaje = self.robot.desconectar()
            print(mensaje)
            self.logger.registrar_log("desconectar_robot", ip, usuario, True)
        except Exception as e:
            print(f"Error al desconectar: {e}")
            self.logger.registrar_log("desconectar_robot", ip, usuario, False)

    def iniciar_servidor_rpc(self):
        if self.servidor_activado is False:
            self.evento_creacion_Servidor.set()
            self.logger.registrar_log("iniciar_servidor_rpc", "127.0.0.1", "consola_local", True)
            self.servidor_activado = True
        else:
            print("El servidor RPC ya está en ejecución\n")

    def detener_servidor_rpc(self):
        if self.servidor_activado is not False:
            self.evento_cierre_Servidor.set()
            print("Servidor RPC detenido\n")
            self.logger.registrar_log("detener_servidor_rpc", "127.0.0.1", "consola_local", True)
            self.servidor_activado = False
        else:
            print("No hay servidor en ejecución para detener\n")

    def activar_motores(self):
        usuario = "consola_local"
        ip = "127.0.0.1"
        try:
            mensajes = self.robot.activar_motores()
            for mensaje in mensajes:
                print(mensaje)
            self.logger.registrar_log("activar_motores", ip, usuario, True)
        except Exception as e:
            print(f"Error al activar motores: {e}")
            self.logger.registrar_log("activar_motores", ip, usuario, False)

    def desactivar_motores(self):
        usuario = "consola_local"
        ip = "127.0.0.1"
        try:
            mensaje = self.robot.desactivar_motores()
            print(mensaje)
            self.logger.registrar_log("desactivar_motores", ip, usuario, True)
        except Exception as e:
            print(f"Error al desactivar motores: {e}")
            self.logger.registrar_log("desactivar_motores", ip, usuario, False)
            
    # Listar los comandos disponibles
    def listar_comandos(self):
        comandos = [
            "Conectar al Robot",
            "Desconectar del Robot",
            "Activar Motores",
            "Desactivar Motores",
            "Mover Efector Final (con Velocidad)",
            "Mover Efector Final (solo Posición)",
            "Homming",
            "Aprendizaje (On/Off)",
            "Ejecución Automática",
            "Reportar Estado"
        ]
        print("Comandos disponibles:")
        for comando in comandos:
            print(f"- {comando}")

    # Mostrar y editar los parámetros de conexión
    def mostrar_editar_parametros(self):
        # Cargar parámetros desde el archivo JSON
        try:
            with open(self.archivo_configuracion, "r") as archivo:
                parametros = json.load(archivo)
        except FileNotFoundError:
            print("Archivo de configuración no encontrado, creando uno nuevo.")
            parametros = {"puerto_serial": "COM8", "baudios": 115200}

        print(f"Parámetros actuales:\nPuerto: {parametros['puerto_serial']}\nBaudios: {parametros['baudios']}")
        editar = questionary.confirm("¿Desea editar los parámetros de conexión?").ask()

        if editar:
            nuevo_puerto = questionary.text("Ingrese el nuevo puerto (actual: {}):".format(parametros['puerto_serial']), default=parametros['puerto_serial']).ask()
            nuevo_baudios = questionary.text("Ingrese la nueva tasa de baudios (actual: {}):".format(parametros['baudios']), default=str(parametros['baudios'])).ask()

            # Actualizar el archivo de configuración
            parametros['puerto_serial'] = nuevo_puerto
            parametros['baudios'] = int(nuevo_baudios)

            with open(self.archivo_configuracion, "w") as archivo:
                json.dump(parametros, archivo)

            print(f"Parámetros actualizados:\nPuerto: {nuevo_puerto}\nBaudios: {nuevo_baudios}")
        else:
            print("No se realizaron cambios en los parámetros.")

    def modo_absoluto(self):
        usuario = "consola_local"
        ip = "127.0.0.1"
        try:
            mensajes = self.robot.modo_absoluto()
            for mensaje in mensajes:
                print(mensaje)
            for mensaje in mensajes:
                if "ERROR" in mensaje:
                    self.logger.registrar_log("modo_absoluto", ip, usuario, False)
                    break
            else: 
                self.logger.registrar_log("modo_absoluto", ip, usuario, True)
                self.tipo_movimiento = "absoluto"
        except Exception as e:
            print(f"Error al cambiar a modo absoluto: {e}")
            self.logger.registrar_log("modo_absoluto", ip, usuario, False)

    def modo_relativo(self):
        usuario = "consola_local"
        ip = "127.0.0.1"
        try:
            mensajes = self.robot.modo_relativo()
            for mensaje in mensajes:
                print(mensaje)
            
            for mensaje in mensajes:
                if "ERROR" in mensaje:
                    self.logger.registrar_log("modo_relativo", ip, usuario, False)
                    break
            else: 
                self.logger.registrar_log("modo_relativo", ip, usuario, True)
                self.tipo_movimiento = "relativo"
        except Exception as e:
            print(f"Error al cambiar a modo relativo: {e}")
            self.logger.registrar_log("modo_relativo", ip, usuario, False)

    def mover_efector(self):
        usuario = "consola_local"
        ip = "127.0.0.1"
        try:
            x = questionary.text("Ingrese la coordenada X:").ask()
            y = questionary.text("Ingrese la coordenada Y:").ask()
            z = questionary.text("Ingrese la coordenada Z:").ask()
            velocidad = questionary.text("Ingrese la velocidad:").ask()
            mensajes = self.robot.mover_efector(float(x), float(y), float(z), float(velocidad))
            for mensaje in mensajes:
                print(mensaje)
            for mensaje in mensajes:
                if "ERROR" in mensaje:
                    self.logger.registrar_log("mover_efector", ip, usuario, False)
                    break
            else: 
                self.logger.registrar_log("mover_efector", ip, usuario, True)
        except Exception as e:
            print(f"Error al mover el efector: {e}")
            self.logger.registrar_log("mover_efector", ip, usuario, False)

    def mover_efector_posicion(self):
        usuario = "consola_local"
        ip = "127.0.0.1"
        try:
            x = questionary.text("Ingrese la coordenada X:").ask()
            y = questionary.text("Ingrese la coordenada Y:").ask()
            z = questionary.text("Ingrese la coordenada Z:").ask()
            mensajes = self.robot.mover_efector_posicion(float(x), float(y), float(z))
            for mensaje in mensajes:
                print(mensaje)
            self.logger.registrar_log("mover_efector_posicion", ip, usuario, True)
        except Exception as e:
            print(f"Error al mover el efector: {e}")
            self.logger.registrar_log("mover_efector_posicion", ip, usuario, False)

    def realizar_homming(self):
        usuario = "consola_local"
        ip = "127.0.0.1"
        try:
            mensajes = self.robot.homming()
            for mensaje in mensajes:
                print(mensaje)
            self.logger.registrar_log("homming", ip, usuario, True)
        except Exception as e:
            print(f"Error al realizar homming: {e}")
            self.logger.registrar_log("homming", ip, usuario, False)
            
    def aprendizaje(self):
        usuario = "consola_local"
        ip = "127.0.0.1"
        try:
            nombre_archivo = questionary.text("Ingrese el nombre del archivo:").ask()
            activar = questionary.confirm("¿Desea activar el modo aprendizaje?").ask()
            mensaje = self.robot.aprender(nombre_archivo, activar)
            print(mensaje)
            self.logger.registrar_log("aprendizaje", ip, usuario, True)
        except Exception as e:
            print(f"Error en el modo aprendizaje: {e}")
            self.logger.registrar_log("aprendizaje", ip, usuario, False)

    def ejecutar_automatico(self):
        usuario = "consola_local"
        ip = "127.0.0.1"
        try:
            nombre_archivo = questionary.text("Ingrese el nombre del archivo G-code (sin extensión):").ask()
            mensaje = self.robot.ejecutar_automatico(nombre_archivo)
            if mensaje:
                print(mensaje)
            self.logger.registrar_log("ejecutar_automatico", ip, usuario, True)
        except ErrorDeConexion as e:
            print(f"Error de conexión: {e}")
            self.logger.registrar_log("ejecutar_automatico", ip, usuario, False)
        except ErrorDeEstado as e:
            print(f"Error de estado: {e}")
            self.logger.registrar_log("ejecutar_automatico", ip, usuario, False)
        except Exception as e:
            print(f"Error inesperado en la ejecución automática: {e}")
            self.logger.registrar_log("ejecutar_automatico", ip, usuario, False)

    def reportar_estado(self):
        try:
            reporte = self.robot.reportar()
            print(reporte)
            self.logger.registrar_log("reportar_estado", "127.0.0.1", "consola_local", True)
        except Exception as e:
            print(f"Error al reportar estado: {e}")
            self.logger.registrar_log("reportar_estado", "127.0.0.1", "consola_local", False)

    def reportar_posicion(self):
        usuario = "consola_local"
        ip = "127.0.0.1"
        try:
            mensajes = self.robot.reportar_posicion()
            for mensaje in mensajes:
                print(mensaje)
            self.logger.registrar_log("reportar_posicion", ip, usuario, True)
        except Exception as e:
            print(f"Error al reportar la posición: {e}")
            self.logger.registrar_log("reportar_posicion", ip, usuario, False)

    def activar_efector(self):
        usuario = "consola_local"
        ip = "127.0.0.1"
        try:
            mensaje = self.robot.actuar_efector('1')
            print(mensaje)
            self.logger.registrar_log("activar_efector", ip, usuario, True)
        except Exception as e:
            print(f"Error al activar el efector: {e}")
            self.logger.registrar_log("activar_efector", ip, usuario, False)

    def desactivar_efector(self):
        usuario = "consola_local"
        ip = "127.0.0.1"
        try:
            mensaje = self.robot.actuar_efector('0')
            print(mensaje)
            self.logger.registrar_log("desactivar_efector", ip, usuario, True)
        except Exception as e:
            print(f"Error al desactivar el efector: {e}")
            self.logger.registrar_log("desactivar_efector", ip, usuario, False)

    def mostrar_log_admin(self):
        """Función para mostrar las últimas 100 líneas del log de trabajo (solo para admin)"""
        # Aquí podemos agregar validación si queremos restringirlo a ciertos usuarios
        print("Mostrando las últimas 100 líneas del log de trabajo:\n")
        ultimas_lineas = self.gestor_logs.leer_ultimas_lineas(100)
        for linea in ultimas_lineas:
            print(linea.strip())

    def cambiar_modo_trabajo(self):
        self.modo_trabajo = "manual" if self.modo_trabajo == "automático" else "automático"
        print(f"Modo de trabajo cambiado a: {self.modo_trabajo}")

    def salir(self):
        # Desactivar el modo de aprendizaje si está activo
        if self.robot.aprendiendo:
            print("El modo de aprendizaje está activo, desactivándolo antes de salir.")
            self.robot.aprender("", False)  # Desactivar el modo de aprendizaje
        self.detener_servidor_rpc()

        print("Saliendo del sistema...")
        self.evento_finalizacion.set()