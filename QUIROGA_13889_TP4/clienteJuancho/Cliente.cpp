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

// Método para crear un mensaje en formato JSON manualmente
string ClienteRobot::crearMensajeJson(const string& usuario, const string& clave, const string& comando, const vector<string>& tipoParametros, const vector<string>& parametros) {
    ostringstream oss;
    oss << "{\"usuario\":\"" << usuario << "\","
        << "\"clave\":\"" << clave << "\","
        << "\"comando\":\"" << comando << "\","
        << "\"parametros\":{";

    for (size_t i = 0; i < tipoParametros.size(); ++i) {
        oss << "\"" << tipoParametros[i] << "\":\"" << parametros[i] << "\"";
        if (i != tipoParametros.size() - 1) {
            oss << ",";
        }
    }

    oss << "}}";  // Cierra el diccionario de "parametros" y el objeto JSON

    return oss.str();
}


// Método para ejecutar comandos y enviar petición al servidor
void ClienteRobot::ejecutarComando(const string& comando, const string& mensajeJson) {
    XmlRpc::XmlRpcClient cliente(servidorUrl.c_str(), servidorPuerto);
    XmlRpc::XmlRpcValue args, result;
    
    // Colocamos el mensaje JSON como el único argumento que enviamos
    args[0] = mensajeJson;
    
    // Siempre ejecutamos el comando registrado en el servidor: "Interpreta_Comando"
    if (cliente.execute("Interpreta_Comando", args, result)) {
        cout << "Respuesta del servidor: " << result << endl;
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
            cout << "Saliendo del programa"<<endl;
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
            cerr << "Comando no valido." << endl;
            return;
        }
        
        string usuario = interfaz.solicitarUsuario();
        string clave = interfaz.solicitarClave();
        
        // Crear el vector de tipo de parámetros según el comando seleccionado
        vector<string> tipoParametros;
        if (comando == "conectar") {
            tipoParametros = {"puerto_COM", "tasa_baudios"};
        } else if (comando == "mover_efector") {
            tipoParametros = {"X", "Y", "Z", "velocidad"};
        } else if (comando == "actuar_efector") {
            tipoParametros = {"accion_efector"};
        } else if (comando == "ejecutar_automatico") {
            tipoParametros = {"nombre_archivo"};
        }
        
        // Solicitar los parámetros dependiendo del comando
        vector<string> parametros = interfaz.solicitarParametros(comandoSeleccionado);
        
        // Crear el mensaje JSON para enviar
        string mensajeJson = crearMensajeJson(usuario, clave, comando, tipoParametros, parametros);
        cout << "Mensaje JSON: " << mensajeJson << endl;
        
        // Ejecutar el comando con el mensaje JSON
        ejecutarComando("Interpreta_Comando", mensajeJson);
    }
}
