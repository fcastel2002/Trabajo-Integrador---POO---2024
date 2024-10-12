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

int Interfaz::solicitarOpcionMenuServidor() {
    string input;  // Leer la entrada como string primero
    int opcion;

    while (true) {
        cout << "\nMenu del servidor:\n";
        cout << "1. Mostrar cantidad de usuarios\n";
        cout << "2. Mostrar estadisticas de un usuario\n";
        cout << "3. Apagar servidor\n";
        cout << "Ingrese una opcion: ";
        
        cin >> input;

        try {
            // Intentamos convertir el string a entero
            opcion = stoi(input);

            // Validamos el rango de opciones
            if (opcion >= 1 && opcion <= 3) {
                return opcion;
            } else {
                cout << "Error: debe ingresar un numero entero entre 1 y 3. Intente nuevamente.\n";
            }
        } catch (invalid_argument&) {
            cout << "Error: debe ingresar un numero valido. Intente nuevamente.\n";
        } catch (out_of_range&) {
            cout << "Error: el numero ingresado esta fuera del rango permitido. Intente nuevamente.\n";
        }
    }
}

void Interfaz::mostrarMensaje(const string& mensaje) {
    cout << mensaje << endl;
}
