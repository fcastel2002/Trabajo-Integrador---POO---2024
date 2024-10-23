import threading
import queue
import time
import socket
import questionary
from datetime import datetime

# Cola para las instrucciones del cliente
cola_instrucciones = queue.Queue()

# Función para mostrar el día
def mostrar_dia():
    dia = datetime.now().strftime("%Y-%m-%d")
    print(f"\n[Mostrar Día] Hoy es: {dia}")

# Función para mostrar la hora
def mostrar_hora():
    hora = datetime.now().strftime("%H:%M:%S")
    print(f"\n[Mostrar Hora] La hora actual es: {hora}")

# Interfaz del servidor usando questionary
def manejar_interfaz():
    while True:
        # Revisar si hay instrucciones en la cola
        while not cola_instrucciones.empty():
            instruccion, args = cola_instrucciones.get()
            instruccion(*args)  # Ejecutar la función enviada (Mostrar hora en este caso)

        # Menu interactivo con questionary
        opcion = questionary.select(
            "Elige una opción:",
            choices=[
                "Mostrar Día",
                "Mostrar Hora",
                "Salir"
            ]
        ).ask()

        if opcion == "Mostrar Día":
            mostrar_dia()
        elif opcion == "Mostrar Hora":
            mostrar_hora()
        elif opcion == "Salir":
            print("Saliendo del servidor...")
            break

# Hilo que recibe solicitudes del cliente
def manejar_cliente():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as servidor:
        servidor.bind(('localhost', 9999))
        servidor.listen()
        print("Servidor escuchando en el puerto 9999...")

        while True:
            cliente, direccion = servidor.accept()
            with cliente:
                print(f"\nCliente conectado d.esde {direccion}")
                # Al recibir la conexión del cliente, agregar la instrucción de mostrar la hora
                cola_instrucciones.put((mostrar_hora, []))

# Crear e iniciar los hilos del servidor
hilo_interfaz = threading.Thread(target=manejar_interfaz)
hilo_cliente = threading.Thread(target=manejar_cliente)

hilo_interfaz.start()
hilo_cliente.start()

hilo_interfaz.join()
hilo_cliente.join()
