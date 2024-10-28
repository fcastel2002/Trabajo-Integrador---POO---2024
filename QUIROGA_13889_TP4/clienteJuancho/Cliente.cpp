#include "Cliente.h"
#include <sstream>

// Constructor del cliente
ClienteRobot::ClienteRobot(const string& url, int puerto)
    : servidorUrl(url), servidorPuerto(puerto) {}

// Método para configurar alias y IP
void ClienteRobot::setAlias(const string& nuevoAlias, const string& nuevaIP) {
    alias = nuevoAlias;
    ip = nuevaIP;
}

// Método para ejecutar comandos y enviar petición al servidor
void ClienteRobot::ejecutarComando(const string& comando, const vector<vector<string>>& parametros) {
    XmlRpc::XmlRpcClient cliente(servidorUrl.c_str(), servidorPuerto);
    XmlRpc::XmlRpcValue args, result;

    // Asignar el nombre de usuario y la clave
    args[0] = alias;  // usuario (el alias)
    args[1] = "clave123";  // clave (puedes hacer que sea dinámico más tarde)
    args[2] = comando;  // comando

    // Pasar los parámetros como un array de XmlRpcValue
    XmlRpc::XmlRpcValue paramArray;
    for (size_t i = 0; i < parametros.size(); ++i) {
        cout << parametros[i].size() << endl;
        for (size_t j = 0; j < parametros[i].size(); ++j) {

            if (parametros[i].size() > 1) {
                paramArray[i+j] = parametros[i][j];
            } else {
                paramArray[i] = parametros[i][j];
            }
            
        }
        //cout << paramArray[i] << endl;
    }
    args[3] = paramArray;  // parámetros

    // Ejecutar el comando en el servidor
    if (cliente.execute("Interpreta_Comando", args, result)) {
        // cout << "Respuesta del servidor: " << result << endl;
            if (result.getType() == XmlRpc::XmlRpcValue::TypeArray) {
            std::cout << "Respuesta del servidor:\n";
            for (size_t i = 0; i < result.size(); ++i) {
                // Verificamos si cada elemento es de tipo string antes de imprimirlo
                if (result[i].getType() == XmlRpc::XmlRpcValue::TypeString) {
                    std::cout << result[i] << std::endl;
                }
            }
        } else {
            std::cout << "El tipo de 'result' es: " << result.getType() << std::endl;
            std::cout << "Contenido de 'result': " << result << std::endl;
        }
    } else {
        cerr << "Error al ejecutar el comando." << endl;
    }
}

// Método para mostrar el menú de interacción
void ClienteRobot::mostrarMenu() {
    interfaz.mostrarMensaje("Bienvenido al sistema de control del robot.");
    while (1) {
        interfaz.mostrarOpciones();
        
        string comandoSeleccionado = interfaz.solicitarComando();
        string comando;

        if (comandoSeleccionado == "15") {
            cout << "Saliendo del programa" << endl;
            break;
        }

        if (comandoSeleccionado == "1") {
            comando = "conectar";
        } else if (comandoSeleccionado == "2") {
            comando = "desconectar";
        } else if (comandoSeleccionado == "3") {
            comando = "activar_motores";
        } else if (comandoSeleccionado == "4") {
            comando = "desactivar_motores";
        } else if (comandoSeleccionado == "5") {
            comando = "mover_efector";
        } else if (comandoSeleccionado == "6") {
            comando = "homming";
        } else if (comandoSeleccionado == "7") {
            comando = "reportar_estado";
        } else if (comandoSeleccionado == "8") {
            comando = "reportar_posicion";
        } else if (comandoSeleccionado == "9") {
            comando = "actuar_efector";
        } else if (comandoSeleccionado == "10") {
            comando = "ejecutar_automatico";
        } else {
            cerr << "Comando no válido." << endl;
            continue;
        }
        
        // Solicitar los parámetros dependiendo del comando seleccionado
        vector<vector<string>> parametros = interfaz.solicitarParametros(comandoSeleccionado);
        
        // Ejecutar el comando con los parámetros
        ejecutarComando(comando, parametros);
    }
}
