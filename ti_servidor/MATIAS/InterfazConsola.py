import questionary
from ControladorRobot import ControladorRobot

class InterfazConsola:
    def __init__(self):
        self.robot = ControladorRobot("/dev/ttyUSB0", 9600)

    def menu_principal(self):
        while True:
            opcion = questionary.select(
                "Seleccione una opción:",
                choices=[
                    "Conectar al Robot", "Desconectar del Robot",
                    "Activar Motores", "Desactivar Motores",
                    "Mover Efector", "Salir"
                ]
            ).ask()

            if opcion == "Conectar al Robot":
                print(self.robot.conectar())
            elif opcion == "Desconectar del Robot":
                print(self.robot.desconectar())
            elif opcion == "Activar Motores":
                print(self.robot.activar_motores())
            elif opcion == "Desactivar Motores":
                print(self.robot.desactivar_motores())
            elif opcion == "Mover Efector":
                x = float(questionary.text("Ingrese X:").ask())
                y = float(questionary.text("Ingrese Y:").ask())
                z = float(questionary.text("Ingrese Z:").ask())
                v = float(questionary.text("Ingrese Velocidad:").ask())
                print(self.robot.mover_efector(x, y, z, v))
            elif opcion == "Salir":
                print("Saliendo del sistema.")
                break

if __name__ == "__main__":
    interfaz = InterfazConsola()
    interfaz.menu_principal()
