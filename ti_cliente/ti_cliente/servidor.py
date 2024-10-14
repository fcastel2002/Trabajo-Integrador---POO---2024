from xmlrpc.server import SimpleXMLRPCServer
import threading

# Create a custom server class to disable logging
class CustomXMLRPCServer(SimpleXMLRPCServer):
    def log_request(self, code='-', size='-'):
        # Override this method to disable logging
        pass

# Start the server
server = CustomXMLRPCServer(("localhost", 8080), allow_none=True)
print("Servidor Python ejecutándose en el puerto 8080...")

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