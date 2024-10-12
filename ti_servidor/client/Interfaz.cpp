#include "Interfaz.h"
#include <limits>


string Interfaz::solicitarAlias() {
    string alias;
    do {
        cout << "Ingrese su alias (maximo 20 caracteres): ";
        cin >> alias;
        if (alias.length() > 20) {
            cout << "Error: el alias es demasiado largo. Intente nuevamente.\n";
        }
    } while (alias.length() > 20);
    return alias;
}

string Interfaz::solicitarIP() {
    string ip;
    bool ipValida;
    do {
        cout << "Ingrese la IP (formato: xxx.xxx.xxx.xxx): ";
        cin >> ip;

        // Validación simple del formato de la IP
        int dots = 0;
        ipValida = true;
        for (char c : ip) {
            if (!isdigit(c) && c != '.') {
                ipValida = false;
                break;
            }
            if (c == '.') dots++;
        }
        // Verificar que la IP tenga exactamente 3 puntos
        if (dots != 3) ipValida = false;

        if (!ipValida) {
            cout << "Error: formato de IP incorrecto. Intente nuevamente.\n";
        }
    } while (!ipValida);

    return ip;
}

double Interfaz::solicitarCotaInferior(double cotaAnterior) {
    string input;
    double cota;
    bool entradaValida = false;

    while (!entradaValida) {
        cout << "Ingrese la cota inferior (o 'r' para repetir la anterior: " << cotaAnterior << "): ";
        cin >> input;

        if (input == "r") {
            return cotaAnterior;
        }

        try {
            cota = stod(input);
            entradaValida = true;
        } catch (invalid_argument&) {
            cout << "Error: debe ingresar un numero valido. Intente nuevamente.\n";
        } catch (out_of_range&) {
            cout << "Error: numero fuera de rango. Intente nuevamente.\n";
        }
    }
    return cota;
}

double Interfaz::solicitarCotaSuperior(double cotaAnterior) {
    string input;
    double cota;
    bool entradaValida = false;

    while (!entradaValida) {
        cout << "Ingrese la cota superior (o 'r' para repetir la anterior: " << cotaAnterior << "): ";
        cin >> input;

        if (input == "r") {
            return cotaAnterior;
        }

        try {
            cota = stod(input);
            entradaValida = true;
        } catch (invalid_argument&) {
            cout << "Error: debe ingresar un numero valido. Intente nuevamente.\n";
        } catch (out_of_range&) {
            cout << "Error: numero fuera de rango. Intente nuevamente.\n";
        }
    }
    return cota;
}

int Interfaz::solicitarOpcionMenuCliente() {
    int opcion;
    while (true) {
        cout << "\nMenu del cliente:\n";
        cout << "1. Solicitar numero\n";
        cout << "2. Solicitar numero con marca de tiempo\n";
        cout << "3. Mostrar estadisticas\n";
        cout << "4. Mostrar numeros en orden ascendente\n";
        cout << "5. Cambiar de alias\n";
        cout << "6. Iniciar nueva secuencia\n";
        cout << "7. Salir\n";
        cout << "Ingrese una opcion: ";

        // Validación de entrada
        if (!(cin >> opcion) || opcion <= 0 || opcion > 7) {
            cout << "Error: debe ingresar un numero entero entre 1 y 7. Intente nuevamente.\n";
            cin.clear(); // Limpia el estado de error de `cin`
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descartar la entrada inválida
        } else {
            return opcion;
        }
    }
}


void Interfaz::mostrarResultado(double numero, const string& tiempo) {
    cout << "Numero: " << numero << " Generado en: " << tiempo << endl;
}

void Interfaz::mostrarResultadoSinTiempo(double numero) {
    cout << "Numero recibido: " << numero << endl;
}

void Interfaz::mostrarEstadisticas(int cantidadNumeros, double suma, double promedio) {
    cout << "Cantidad de numeros recibidos: " << cantidadNumeros << endl;
    cout << "Suma acumulada: " << suma << endl;
    cout << "Promedio: " << promedio << endl;
}

void Interfaz::mostrarNumerosOrdenados(const vector<double>& numeros) {
    cout << "Numeros en orden ascendente:" << endl;
    for (double numero : numeros) {
        cout << numero << " ";
    }
    cout << endl;
}

void Interfaz::mostrarMensaje(const string& mensaje) {
    cout << mensaje << endl;
}
