#include "Cliente.h"
#include <algorithm>

ClienteNumeros::ClienteNumeros(const string& url, int puerto) 
    : servidorUrl(url), servidorPuerto(puerto), ultimaCotaInferior(0), ultimaCotaSuperior(0) {}

void ClienteNumeros::setAlias(const string& nuevoAlias, const string& ip) {
    alias = nuevoAlias;
    this->ip = ip;
}

void ClienteNumeros::solicitarNumero(bool conTiempo) {
    XmlRpcClient cliente(servidorUrl.c_str(), servidorPuerto);
    XmlRpcValue args, result;

    double cotaInferior = interfaz.solicitarCotaInferior(ultimaCotaInferior);
    double cotaSuperior = interfaz.solicitarCotaSuperior(ultimaCotaSuperior);
    ultimaCotaInferior = cotaInferior;
    ultimaCotaSuperior = cotaSuperior;

    args[0] = cotaInferior;
    args[1] = cotaSuperior;
    args[2] = alias;
    args[3] = ip;  // Solicita la IP a través de la interfaz

    if (cliente.execute("generarNumero", args, result)) {
        double numero = static_cast<double>(result[0]);
        string tiempo = result[1];

        numerosRecibidos.push_back(numero);
        if (conTiempo) {
            tiemposRecibidos.push_back(tiempo);
            interfaz.mostrarResultado(numero, tiempo);
        } else {
            interfaz.mostrarResultadoSinTiempo(numero);
        }
    } else {
        interfaz.mostrarMensaje("Error al solicitar el numero.");
    }
}

void ClienteNumeros::iniciarNuevaSecuencia() {
    XmlRpcClient cliente(servidorUrl.c_str(), servidorPuerto);
    XmlRpcValue args, result;
    
    args[0] = alias;
    args[1] = ip;  // IP solicitada desde la interfaz

    if (!cliente.execute("registrarSecuencia", args, result)) {
        interfaz.mostrarMensaje("Error al iniciar secuencia.");
    }
}

void ClienteNumeros::mostrarEstadisticas() {
    if (numerosRecibidos.empty()) {
        interfaz.mostrarMensaje("No se ha recibido ningun numero aun.");
        return;
    }

    double suma = 0;
    for (double numero : numerosRecibidos) {
        suma += numero;
    }
    double promedio = suma / numerosRecibidos.size();

    interfaz.mostrarEstadisticas(numerosRecibidos.size(), suma, promedio);
}

void ClienteNumeros::mostrarNumerosEnOrden() {
    if (numerosRecibidos.empty()) {
        interfaz.mostrarMensaje("No hay numeros recibidos para mostrar.");
        return;
    }

    sort(numerosRecibidos.begin(), numerosRecibidos.end());
    interfaz.mostrarNumerosOrdenados(numerosRecibidos);
}
