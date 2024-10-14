from xmlrpc.server import SimpleXMLRPCServer

class SimpleRobotController:
    def conectar(self):
        print("Connecting to the robot...")
        return "Robot connected"

    def activar_motores(self):
        print("Activating motors...")
        return "Motors activated"

    def mover_efector(self, x, y, z, velocidad):
        print(f"Moving effector to ({x}, {y}, {z}) with speed {velocidad}...")
        return f"Effector moved to ({x}, {y}, {z}) with speed {velocidad}"

    def desconectar(self):
        print("Disconnecting from the robot...")
        return "Robot disconnected"

def run_server():
    server = SimpleXMLRPCServer(("127.0.0.1", 9000), allow_none=True)
    controller = SimpleRobotController()
    server.register_instance(controller)
    print("Server is running on 127.0.0.1:9000...")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("Shutting down the server...")
        server.shutdown()
        print("Server has been shut down.")

if __name__ == "__main__":
    run_server()
