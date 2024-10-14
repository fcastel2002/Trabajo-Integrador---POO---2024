import questionary
from ControladorRobot import ControladorRobot
from ServidorControl import ServidorControl
import sys


class InterfazConsola:
    def __init__(self):
        self.robot = ControladorRobot('COM8', 115200)
        self.rpc_server = None

    def iniciar(self):
        while True:
            # Menú dinámico según el estado del robot
            opciones_menu = []

            if self.robot.estado_conexion == "desconectado":
                opciones_menu.append("Conectar al Robot")
            else:
                opciones_menu.append("Desconectar del Robot")
                opciones_menu.append("Iniciar/Detener Servidor RPC")
                if not self.robot.motores_activos:
                    opciones_menu.append("Activar Motores")
                else:
                    opciones_menu.append("Desactivar Motores")
                    opciones_menu.append("Mover Efector Final")
                    opciones_menu.append("Movimiento Circular")
                    opciones_menu.append("Homming")
                    opciones_menu.append("Aprendizaje (On/Off)")
                    opciones_menu.append("Ejecución Automática")
                    opciones_menu.append("Reportar Estado")

            opciones_menu.append("Salir")  # Siempre debe estar disponible

            choice = questionary.select(
                "Seleccione una opción:",
                choices=opciones_menu
            ).ask()

            if choice == "Conectar al Robot":
                self.conectar_robot()
            elif choice == "Desconectar del Robot":
                self.desconectar_robot()
            elif choice == "Iniciar/Detener Servidor RPC":
                self.gestionar_servidor()
            elif choice == "Activar Motores":
                self.activar_motores()
            elif choice == "Desactivar Motores":
                self.desactivar_motores()
            elif choice == "Mover Efector Final":
                self.mover_efector()
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
            elif choice == "Salir":
                self.salir()

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

    def gestionar_servidor(self):
        value = questionary.confirm("¿Desea iniciar el servidor RPC?").ask()
        if value:
            if self.rpc_server is None:
                self.rpc_server = ServidorControl(self)
                print("Servidor iniciado\n")
            else:
                print("Servidor ya está en ejecución\n")
        else:
            if self.rpc_server is not None:
                self.rpc_server.shutdown()
                self.rpc_server = None
                print("Servidor detenido\n")
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
        sys.exit(0)