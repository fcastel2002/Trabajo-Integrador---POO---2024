#include "Servidor.h"
#include <cstdlib>
#include <ctime>

ServidorNumeros::ServidorNumeros() {}

void ServidorNumeros::agregarUsuario(const string& alias, const string& ip) {
    lock_guard<mutex> lock(mtx);
    if (usuarios.find(alias) == usuarios.end()) {
        usuarios[alias] = Usuario(alias, ip);
    }
}

void ServidorNumeros::registrarSecuencia(const string& alias, const string& ip) {
    lock_guard<mutex> lock(mtx);
    if (usuarios.find(alias) != usuarios.end()) {
        usuarios[alias].secuenciasIniciadas++;
    } else {
        // Si no existe el usuario, se agrega y se inicia la secuencia
        usuarios[alias] = Usuario(alias, ip);
        usuarios[alias].secuenciasIniciadas++;
    }
}

void ServidorNumeros::agregarSecuencia(string alias) {
    if (usuarios.find(alias) != usuarios.end()) {
        usuarios[alias].secuenciasIniciadas++;
    } else {
        interfaz.mostrarMensaje("Error: Usuario no encontrado para iniciar secuencia.");
    }
}

void ServidorNumeros::agregarNumero(string alias) {
    lock_guard<mutex> lock(mtx);
    if (usuarios.find(alias) != usuarios.end()) {
        usuarios[alias].numerosGenerados++;
    } else {
        interfaz.mostrarMensaje("Error: Usuario no encontrado para agregar número.");
    }
}

vector<string> ServidorNumeros::obtenerCantidadUsuarios() {
    vector<string> resultados;
    resultados.push_back("Cantidad de usuarios diferentes: " + to_string(usuarios.size()));
    for (const auto& [alias, usuario] : usuarios) {
        resultados.push_back("Alias: " + alias + ", IP: " + usuario.ip);
    }
    return resultados;
}

vector<string> ServidorNumeros::obtenerEstadisticasUsuario(string alias) {
    vector<string> estadisticas;
    if (usuarios.find(alias) != usuarios.end()) {
        Usuario& usuario = usuarios[alias];
        estadisticas.push_back("Alias: " + usuario.alias);
        estadisticas.push_back("Direccion IP: " + usuario.ip);
        estadisticas.push_back("Numeros generados: " + to_string(usuario.numerosGenerados));
        estadisticas.push_back("Secuencias iniciadas: " + to_string(usuario.secuenciasIniciadas));
    } else {
        estadisticas.push_back("El usuario no tiene registros.");
    }
    return estadisticas;
}

void ServidorNumeros::apagarServidor() {
    exit();
}

MetodoGenerarNumero::MetodoGenerarNumero(ServidorNumeros* s) 
    : XmlRpcServerMethod("generarNumero", s), servidor(s) {}

void MetodoGenerarNumero::execute(XmlRpcValue& params, XmlRpcValue& result) {
    double cota_inferior = static_cast<double>(params[0]);
    double cota_superior = static_cast<double>(params[1]);
    string alias = static_cast<string>(params[2]);
    string ip = static_cast<string>(params[3]);

    servidor->agregarUsuario(alias, ip);
    servidor->agregarNumero(alias);

    double numero = cota_inferior + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (cota_superior - cota_inferior)));
    time_t ahora = time(0);
    char* tiempo = ctime(&ahora);

    result[0] = numero;
    result[1] = string(tiempo);
}

// Implementación del método para registrar secuencias
MetodoRegistrarSecuencia::MetodoRegistrarSecuencia(ServidorNumeros* s)
    : XmlRpcServerMethod("registrarSecuencia", s), servidor(s) {}

void MetodoRegistrarSecuencia::execute(XmlRpcValue& params, XmlRpcValue& result) {
    string alias = static_cast<string>(params[0]);
    string ip = static_cast<string>(params[1]);

    servidor->registrarSecuencia(alias, ip);

    result = "Secuencia registrada exitosamente.";
}

