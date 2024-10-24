import threading
from InterfazConsola2 import InterfazConsola
from ControladorRobot import ControladorRobot
from ServidorControl4 import ServidorControl
import time
import sys

def iniciar_interfaz(consola):
    consola.iniciar()

def iniciar_servidor(servidor):
    servidor.iniciar()

def main():
    # Crear objeto de controlador del robot
    robot = ControladorRobot('COM8', 115200)
    consola = InterfazConsola(robot)
    servidor = ServidorControl(robot)

    # Control de los hilos
    hilo_consola = threading.Thread(target=iniciar_interfaz, args=(consola,))
    hilo_consola.start()
    hilo_servidor = None

    try:
        while not consola.evento_finalizacion.is_set():
            # Gestionar la creación del servidor cuando sea necesario
            if consola and consola.evento_creacion_Servidor.is_set():
                hilo_servidor = threading.Thread(target=iniciar_servidor, args=(servidor,))
                hilo_servidor.start()
                consola.evento_creacion_Servidor.clear()

            # Detener el servidor si se selecciona esa opción
            if consola and consola.evento_cierre_Servidor.is_set():
                if hilo_servidor:
                    servidor.disconnect()
                    hilo_servidor.join(timeout=4)
                consola.evento_cierre_Servidor.clear()

            # Actualización de la consola
            if servidor and servidor.evento_actualizacion.is_set():
                print("Actualizando consola...")
                
                # Aquí verificamos si el hilo de consola sigue activo
                if hilo_consola.is_alive():
                    # Forzamos la finalización del hilo de consola anterior
                    consola.evento_finalizacion.set()
                    hilo_consola.join(timeout=4)

                # Crear una nueva consola e iniciar un nuevo hilo
                consola = InterfazConsola(robot)
                hilo_consola = threading.Thread(target=iniciar_interfaz, args=(consola,))
                hilo_consola.start()
                
                # Limpiar el evento de actualización
                servidor.evento_actualizacion.clear()

            time.sleep(1)
    except KeyboardInterrupt:
        print("Saliendo...")

    # Asegurarse de que todos los hilos finalicen correctamente
    if hilo_servidor:
        servidor.disconnect()
        hilo_servidor.join(timeout=4)

    # Finalizar el hilo de la consola
    if hilo_consola.is_alive():
        consola.evento_finalizacion.set()
        hilo_consola.join(timeout=4)
    
    print("Programa finalizado.")

if __name__ == "__main__":
    main()
