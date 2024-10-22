from xmlrpc.server import SimpleXMLRPCServer
from xmlrpc.server import SimpleXMLRPCRequestHandler
import json

# Restringir a una ruta específica
class RequestHandler(SimpleXMLRPCRequestHandler):
    rpc_paths = ('/RPC2',)

# Crear el servidor
with SimpleXMLRPCServer(('localhost', 9000), requestHandler=RequestHandler) as server:
    server.register_introspection_functions()

    # Definir una función que será llamada por el cliente
    def interpretar_comando(cmd_json):
        try:
            cmd = json.loads(cmd_json)
            print(f"Comando recibido: {cmd_json}")
            return "Comando procesado correctamente"
        except json.JSONDecodeError as e:
            return f"Error al procesar el comando: {e}"

    server.register_function(interpretar_comando, 'interpretar_comando')

    # Ejecutar el servidor
    print("Servidor XML-RPC escuchando en el puerto 9000...")
    server.serve_forever()
