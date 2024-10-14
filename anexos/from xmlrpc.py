from xmlrpc.server import SimpleXMLRPCServer
import threading

# Create a custom server class to disable logging
class CustomXMLRPCServer(SimpleXMLRPCServer):
    def log_request(self, code='-', size='-'):
        # Override this method to disable logging
        pass

# Define a function to confirm the connection
def confirmar_conexion():
    print("Client has confirmed the connection.")
    return "Conexión exitosa"

# Start the server
server = CustomXMLRPCServer(("localhost", 8080), allow_none=True)
print("Servidor Python ejecutándose en el puerto 8080...")

# Register the confirmation function with the server
server.register_function(confirmar_conexion, "confirmar_conexion")

# Run the server in a separate thread
def iniciar_servidor():
    server.serve_forever()

hilo_servidor = threading.Thread(target=iniciar_servidor)
hilo_servidor.start()

# Keep the main thread alive to keep the server running
try:
    while True:
        pass
except KeyboardInterrupt:
    print("Apagando el servidor...")
    server.shutdown()
    hilo_servidor.join()

