import json
import questionary
from ControladorRobot import ControladorRobot
from ServidorControl import ServidorControl
from GestorDeArchivos import GestorDeArchivos
import sys

class InterfazConsola:
    def __init__(self):
        self.robot = ControladorRobot('COM8', 115200)
        self.rpc_server = None
        self.archivo_configuracion = "configuracion_robot.json"  # Archivo para guardar la configuración
        self.archivo_usuarios = "usuarios.json"  # Archivo que contiene usuarios y contraseñas
        self.archivo_logs = "logs_trabajo.csv"  # Archivo donde se guardan los logs de trabajo
        self.modo_trabajo = "manual"  # Modo por defecto
        self.tipo_movimiento = "absoluto"  # Tipo de movimiento por defecto
        self.gestor_logs = GestorDeArchivos(self.archivo_logs)  # Instancia para manejar el archivo de logs

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

        Ejemplo:
        Para conectar el robot:
        conectar("admin", "clave123")
        """
        print(ayuda)

    def iniciar(self):
        while True:
            # Menú dinámico según el estado del robot y el servidor
            opciones_menu = []

            if self.robot.estado_conexion == "desconectado":
                opciones_menu.append("Conectar al Robot")
            
            if self.rpc_server is None:
                opciones_menu.append("Iniciar Servidor RPC")

            if self.robot.estado_conexion == "conectado":
                opciones_menu.append("Desconectar del Robot")
                opciones_menu.append("Listar Comandos Disponibles")
                opciones_menu.append("Mostrar/Editar Parámetros de Conexión")
                opciones_menu.append("Cambiar Modo de Trabajo (Actual: {})".format(self.modo_trabajo))
                opciones_menu.append("Cambiar Tipo de Movimiento (Actual: {})".format(self.tipo_movimiento))

                # Mostrar "Activar/Desactivar Motores"
                opciones_menu.append("Activar Motores" if not self.robot.motores_activos else "Desactivar Motores")

                if self.robot.motores_activos:
                    opciones_menu.append("Mover Efector Final (con Velocidad)")
                    opciones_menu.append("Mover Efector Final (solo Posición)")
                    opciones_menu.append("Movimiento Circular")
                    opciones_menu.append("Homming")
                    opciones_menu.append("Aprendizaje (On/Off)")
                    opciones_menu.append("Ejecución Automática")
                    opciones_menu.append("Reportar Estado")

            if self.rpc_server is not None:
                opciones_menu.append("Detener Servidor RPC")
            
            # Agregar la opción de mostrar las últimas 100 líneas del log (solo para admin)
            opciones_menu.append("Mostrar las últimas 100 líneas del Log (Admin)")

            opciones_menu.append("Mostrar Ayuda")  # Opción para mostrar ayuda
            opciones_menu.append("Salir")  # Siempre disponible

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
            elif choice.startswith("Cambiar Modo de Trabajo"):
                self.cambiar_modo_trabajo()
            elif choice.startswith("Cambiar Tipo de Movimiento"):
                self.cambiar_tipo_movimiento()
            elif choice == "Mover Efector Final (con Velocidad)":
                self.mover_efector()
            elif choice == "Mover Efector Final (solo Posición)":
                self.mover_efector_posicion()
            elif choice == "Movimiento Circular":
                self.mover_circular()
            elif choice == "Homming":
                self.realizar_homming()
            elif choice == "Aprendizaje (On/Off)":
                self.aprendizaje()
            elif choice == "Ejecución Automática":
                self.ejecutar_automatico()
            elif choice == "Reportar Estado":
                self.reportar_estado()
            elif choice == "Mostrar las últimas 100 líneas del Log (Admin)":
                self.mostrar_log_admin()
            elif choice == "Mostrar Ayuda":
                self.mostrar_ayuda()
            elif choice == "Salir":
                self.salir()

    def mostrar_log_admin(self):
        """Función para mostrar las últimas 100 líneas del log de trabajo (solo para admin)"""
        # Aquí podemos agregar validación si queremos restringirlo a ciertos usuarios
        print("Mostrando las últimas 100 líneas del log de trabajo:\n")
        ultimas_lineas = self.gestor_logs.leer_ultimas_lineas(100)
        for linea in ultimas_lineas:
            print(linea.strip())

    # Listar los comandos disponibles
    def listar_comandos(self):
        comandos = [
            "Conectar al Robot",
            "Desconectar del Robot",
            "Activar Motores",
            "Desactivar Motores",
            "Mover Efector Final (con Velocidad)",
            "Mover Efector Final (solo Posición)",
            "Movimiento Circular",
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
            nuevo_puerto = questionary.text("Ingrese el nuevo puerto (actual: COM8):", default=parametros['puerto_serial']).ask()
            nuevo_baudios = questionary.text("Ingrese la nueva tasa de baudios (actual: 115200):", default=str(parametros['baudios'])).ask()

            # Actualizar el archivo de configuración
            parametros['puerto_serial'] = nuevo_puerto
            parametros['baudios'] = int(nuevo_baudios)

            with open(self.archivo_configuracion, "w") as archivo:
                json.dump(parametros, archivo)

            print(f"Parámetros actualizados:\nPuerto: {nuevo_puerto}\nBaudios: {nuevo_baudios}")
        else:
            print("No se realizaron cambios en los parámetros.")

    def cambiar_modo_trabajo(self):
        self.modo_trabajo = "manual" if self.modo_trabajo == "automático" else "automático"
        print(f"Modo de trabajo cambiado a: {self.modo_trabajo}")

    def cambiar_tipo_movimiento(self):
        self.tipo_movimiento = "absoluto" if self.tipo_movimiento == "relativo" else "relativo"
        print(f"Tipo de movimiento cambiado a: {self.tipo_movimiento}")

    # Validar usuario y clave
    def validar_usuario(self, usuario, clave):
        try:
            with open(self.archivo_usuarios, "r") as archivo:
                usuarios = json.load(archivo)
                if usuario in usuarios and usuarios[usuario] == clave:
                    print(f"Validación exitosa para el usuario {usuario}")
                    return True
                else:
                    print(f"Usuario o clave incorrectos para el usuario {usuario}")
                    return False
        except FileNotFoundError:
            print("No se encontró el archivo de usuarios.")
            return False

    # Funciones para interactuar con el robot y servidor
    def conectar_robot(self):
        puerto = questionary.text("Ingrese el puerto COM (ejemplo: COM8):").ask()
        baudios = questionary.text("Ingrese la tasa de baudios (ejemplo: 115200):").ask()

        if puerto and baudios:
            self.robot.puerto_serial = puerto
            self.robot.baudios = int(baudios)
            mensaje = self.robot.conectar()
            print(mensaje)
        else:
            print("Error: Debe ingresar un puerto COM y la tasa de baudios.")

    def desconectar_robot(self):
        mensaje = self.robot.desconectar()
        print(mensaje)

    def iniciar_servidor_rpc(self):
        if self.rpc_server is None:
            self.rpc_server = ServidorControl(self)
            self.rpc_server.iniciar()  # Inicia el servidor en un hilo separado
        else:
            print("El servidor RPC ya está en ejecución\n")

    def detener_servidor_rpc(self):
        if self.rpc_server is not None:
            self.rpc_server.disconnect()
            self.rpc_server = None
            print("Servidor RPC detenido\n")
        else:
            print("No hay servidor en ejecución para detener\n")

    def activar_motores(self):
        mensaje = self.robot.activar_motores()
        print(mensaje)

    def desactivar_motores(self):
        mensaje = self.robot.desactivar_motores()
        print(mensaje)

    def mover_efector(self):
        x = questionary.text("Ingrese la coordenada X:").ask()
        y = questionary.text("Ingrese la coordenada Y:").ask()
        z = questionary.text("Ingrese la coordenada Z:").ask()
        velocidad = questionary.text("Ingrese la velocidad:").ask()
        mensaje = self.robot.mover_efector(float(x), float(y), float(z), float(velocidad))
        print(mensaje)

    def mover_efector_posicion(self):
        x = questionary.text("Ingrese la coordenada X:").ask()
        y = questionary.text("Ingrese la coordenada Y:").ask()
        z = questionary.text("Ingrese la coordenada Z:").ask()
        mensaje = self.robot.mover_efector_posicion(float(x), float(y), float(z))
        print(mensaje)

    def mover_circular(self):
        q1 = questionary.text("Ingrese ángulo q1:").ask()
        v1 = questionary.text("Ingrese velocidad v1:").ask()
        q2 = questionary.text("Ingrese ángulo q2:").ask()
        v2 = questionary.text("Ingrese velocidad v2:").ask()
        q3 = questionary.text("Ingrese ángulo q3:").ask()
        v3 = questionary.text("Ingrese velocidad v3:").ask()
        mensaje = self.robot.mover_circular(float(q1), float(v1), float(q2), float(v2), float(q3), float(v3))
        print(mensaje)

    def realizar_homming(self):
        mensaje = self.robot.homming()
        print(mensaje)

    def aprendizaje(self):
        nombre_archivo = questionary.text("Ingrese el nombre del archivo:").ask()
        activar = questionary.confirm("¿Desea activar el modo aprendizaje?").ask()
        mensaje = self.robot.aprender(nombre_archivo, activar)
        print(mensaje)

    def ejecutar_automatico(self):
        nombre_archivo = questionary.text("Ingrese el nombre del archivo G-code:").ask()
        mensaje = self.robot.ejecutar_automatico(nombre_archivo)
        print(mensaje)

    def reportar_estado(self):
        reporte = self.robot.reportar()
        print(reporte)

    def salir(self):
        print("Saliendo del sistema...")
        if self.rpc_server is not None:
            self.rpc_server.disconnect()
        sys.exit(0)