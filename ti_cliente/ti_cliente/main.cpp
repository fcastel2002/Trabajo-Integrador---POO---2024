#include "Cliente.h"

int main() {
    Cliente cliente("127.0.0.1", 9000);

    // Attempt to connect to the server
    std::cout << "Connecting to the server..." << std::endl;
    XmlRpcValue noParams;
    std::string response = cliente.enviarComando("conectar", noParams);
    std::cout << "Response: " << response << std::endl;

    // Example of sending a command to activate motors
    response = cliente.enviarComando("activar_motores", noParams);
    std::cout << "Response: " << response << std::endl;

    // Example of sending a command to move the effector
    XmlRpcValue params;
    params[0] = 10; // x
    params[1] = 20; // y
    params[2] = 30; // z
    params[3] = 5;  // speed
    response = cliente.enviarComando("mover_efector", params);
    std::cout << "Response: " << response << std::endl;

    // Attempt to disconnect from the server
    response = cliente.enviarComando("desconectar", noParams);
    std::cout << "Response: " << response << std::endl;

    return 0;
}
