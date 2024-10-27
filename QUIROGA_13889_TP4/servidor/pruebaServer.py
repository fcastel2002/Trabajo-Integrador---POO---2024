from xmlrpc.server import SimpleXMLRPCServer
from xmlrpc.server import SimpleXMLRPCRequestHandler

# Restringir a una ruta específica
class RequestHandler(SimpleXMLRPCRequestHandler):
    rpc_paths = ('/RPC2',)

# Crear el servidor
with SimpleXMLRPCServer(('localhost', 9000), requestHandler=RequestHandler) as server:
    server.register_introspection_functions()

    # Definir una función que reciba y muestre los parámetros de la lista
    def interpreta_comando(param1,param2,param3,param4):
        
        print("Received params:")
        print("param1:", param1)
        print("param2:", param2)
        print("param3:", param3)
        print("param4:", param4)
        # Simular una respuesta
    
        return {"resultado": ["Comando1", "Comando2", "Comando3"]}

    server.register_function(interpreta_comando, 'Interpreta_Comando')

    # Ejecutar el servidor
    print("Servidor XML-RPC corriendo en http://localhost:5000")
    server.serve_forever()
