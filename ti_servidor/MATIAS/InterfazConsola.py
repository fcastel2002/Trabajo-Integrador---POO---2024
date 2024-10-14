import questionary
from ControladorRobot import ControladorRobot
from ServidorControl import ServidorControl
import sys

class InterfazConsola:
    def __init__(self):
        self.robot = ControladorRobot('COM8', 115200)
        self.servidor_rpc = None

    def iniciar(self):
        while True:
            choice = questionary.select(
                "Seleccione una opción:",
                choices=[
                    "Conectar al Robot",
                    "Desconectar del Robot",
                    "Iniciar/Detener Servidor RPC",
                    "Activar Motores",
                    "Desactivar Motores",
                    "Mover Efector Final",
                    "Movimiento Circular",
                    "Homming",
                    "Aprendizaje (On/Off)",
                    "Ejecución Automática",
                    "Reportar Estado",
                    "Salir"
                ]
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
        mensaje = self.robot.conectar()
        print(mensaje)

    def desconectar_robot(self):
        mensaje = self.robot.desconectar()
        print(mensaje)

    def gestionar_servidor(self):
        value = questionary.confirm("¿Desea iniciar el servidor RPC?").ask()
        if value:
            if self.servidor_rpc is None:
                self.servidor_rpc = ServidorControl(self)
                self.servidor_rpc.run()
                print("Servidor iniciado\n")
            else:
                print("Servidor ya está en ejecución\n")
        else:
            if self.servidor_rpc is not None:
                self.servidor_rpc.shutdown()
                self.servidor_rpc = None
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