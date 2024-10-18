import socket
import json

# Configuración del servidor
HOST = '127.0.0.1'  # Dirección IP local
PORT = 65432        # Puerto del servidor

def iniciar_servidor():
    # Crear el socket del servidor
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as servidor:
        servidor.bind((HOST, PORT))
        servidor.listen()
        print(f"Servidor escuchando en {HOST}:{PORT}")

        # Esperar a que un cliente se conecte
        conn, addr = servidor.accept()
        with conn:
            print(f"Conectado con {addr}")
            # Recibir datos del cliente
            data = conn.recv(1024)
            if not data:
                return

            # Deserializar el JSON recibido
            objeto_recibido = json.loads(data.decode('utf-8'))
            print(f"Recibido: {objeto_recibido}")

            # Procesar el objeto y generar una respuesta
            respuesta = {"status": "OK", "message": "Objeto recibido correctamente"}

            # Serializar la respuesta a JSON y enviarla al cliente
            conn.sendall(json.dumps(respuesta).encode('utf-8'))

if __name__ == '__main__':
    iniciar_servidor()
