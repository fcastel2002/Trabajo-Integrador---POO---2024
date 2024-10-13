from xmlrpc.server import SimpleXMLRPCServer
import threading

# Crear una clase personalizada para desactivar el logging
class CustomXMLRPCServer(SimpleXMLRPCServer):
    def log_request(self, code='-', size='-'):
        # Sobrescribimos este método para no mostrar los logs
        pass

# Definir las funciones que se van a registrar en el servidor
usuarios = {}

def generarNumero(cota_inferior, cota_superior, alias, ip):
    if alias not in usuarios:
        usuarios[alias] = {"ip": ip, "secuencias": 0, "numeros": 0}
    usuarios[alias]["numeros"] += 1
    import random
    numero = random.uniform(cota_inferior, cota_superior)
    from datetime import datetime
    tiempo = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    return numero, tiempo

def iniciarSecuencia(alias, ip):
    if alias not in usuarios:
        usuarios[alias] = {"ip": ip, "secuencias": 0, "numeros": 0}
    usuarios[alias]["secuencias"] += 1
    return "Secuencia registrada exitosamente."

def obtenerEstadisticas(alias):
    if alias in usuarios:
        return usuarios[alias]
    else:
        return "Usuario no encontrado."

def mostrarUsuarios():
    return [f"Alias: {alias}, IP: {datos['ip']}, Secuencias: {datos['secuencias']}, Números: {datos['numeros']}" for alias, datos in usuarios.items()]

# Iniciar el servidor
server = CustomXMLRPCServer(("localhost", 8080), allow_none=True)
print("Servidor Python ejecutándose en el puerto 8080...")

# Registrar las funciones en el servidor
server.register_function(generarNumero, "generarNumero")
server.register_function(iniciarSecuencia, "registrarSecuencia")
server.register_function(obtenerEstadisticas, "obtenerEstadisticas")
server.register_function(mostrarUsuarios, "mostrarUsuarios")

# Ejecutar el servidor en un hilo separado para permitir el uso del menú
def iniciar_servidor():
    server.serve_forever()

hilo_servidor = threading.Thread(target=iniciar_servidor)
hilo_servidor.start()

# Menú de opciones del lado del servidor
def mostrar_menu():
    while True:
        print("\nMenu del servidor:")
        print("1. Mostrar cantidad de usuarios")
        print("2. Mostrar estadisticas de un usuario")
        print("3. Apagar servidor")
        opcion = input("Ingrese una opcion: ")

        if opcion == "1":
            usuarios_list = mostrarUsuarios()
            for user in usuarios_list:
                print(user)
        elif opcion == "2":
            alias = input("Ingrese el alias del usuario: ")
            estadisticas = obtenerEstadisticas(alias)
            print(estadisticas)
        elif opcion == "3":
            print("Apagando el servidor...")
            server.shutdown()
            hilo_servidor.join()
            break
        else:
            print("Opcion no valida. Intente nuevamente.")

# Mostrar el menú de opciones del servidor
mostrar_menu()

