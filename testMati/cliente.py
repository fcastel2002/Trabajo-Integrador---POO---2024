import socket

# Cliente que se conecta y desconecta automáticamente
def conectar_servidor():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as cliente:
        cliente.connect(('localhost', 9999))
        print("Cliente conectado y desconectado automáticamente")

# Ejecutar la función de conexión del cliente
if __name__ == "__main__":
    conectar_servidor()
