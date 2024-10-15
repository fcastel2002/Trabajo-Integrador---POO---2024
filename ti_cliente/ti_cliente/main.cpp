// src/cliente.cpp
#include <iostream>
#include <XmlRpc.h> // Asegúrate de que la ruta de inclusión está configurada correctamente

int main() {
    try {
        // Crear un cliente XML-RPC apuntando al servidor en 127.0.0.1:9000
        XmlRpc::XmlRpcClient client("127.0.0.1", 9000);
        XmlRpc::XmlRpcValue noArgs; // Sin argumentos
        XmlRpc::XmlRpcValue result;

        // Llamar al método "conectar" sin argumentos
        bool callSuccess = client.execute("activar_motores", noArgs, result);

        if (callSuccess) {
            // Convertir el resultado a booleano
            bool success = static_cast<bool>(result);
            if (success) {
                std::cout << "Conexión exitosa al robot." << std::endl;
            }
            else {
                std::cout << "Error al conectar al robot." << std::endl;
            }
        }
        else {
            std::cerr << "Error en la llamada XML-RPC." << std::endl;
        }
    }
    catch (const XmlRpc::XmlRpcException& e) {
        std::cerr << "Exception: " << e.getMessage() << std::endl;
    }

    return 0;
}