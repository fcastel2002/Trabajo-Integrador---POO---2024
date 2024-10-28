#include "Interfaz.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void Interfaz::mostrarMensaje(const string& mensaje) {
    cout << mensaje << endl;
}

void Interfaz::mostrarOpciones() {
    cout << "Opciones disponibles: \n";
    cout << "1. conectar\n";
    cout << "2. desconectar\n";
    cout << "3. activar_motores\n";
    cout << "4. desactivar_motores\n";
    cout << "5. mover_efector\n";
    cout << "6. homming\n";
    cout << "7. reportar_estado\n";
    cout << "8. reportar_posicion\n";
    cout << "9. Actuar_efector\n";
    cout << "10. Ejecutar_automatico\n";
    cout << "15. Salir\n";
}

string Interfaz::solicitarComando() {
    string comando;
    cout << "Ingrese el numero de comando: ";
    cin >> comando;
    return comando;
}

string Interfaz::solicitarUsuario() {
    string usuario;
    cout << "Ingrese el usuario: ";
    cin >> usuario;
    return usuario;
}

string Interfaz::solicitarClave() {
    string clave;
    cout << "Ingrese la clave: ";
    cin >> clave;
    return clave;
}

vector<vector<string>> Interfaz::solicitarParametros(const string& comando) {
    vector<vector<string>> parametros;

    if (comando == "1") {  // conectar
        vector<string> puertoCOM, tasaBaudios;
        string puerto, tasaB;
        cout << "Ingrese el puerto COM: ";
        cin >> puerto;
        cout << "Ingrese la tasa de baudios: ";
        cin >> tasaB;
        puertoCOM.push_back(puerto);
        tasaBaudios.push_back(tasaB);
        parametros.push_back(puertoCOM);
        parametros.push_back(tasaBaudios);
    } else if (comando == "5") {  // mover_efector
        vector<string> X, Y, Z, Velocidad;
        string x, y, z, velocidad;
        cout << "Ingrese coordenada X: ";
        cin >> x;
        cout << "Ingrese coordenada Y: ";
        cin >> y;
        cout << "Ingrese coordenada Z: ";
        cin >> z;
        cout << "Ingrese la velocidad (opcional, presione Enter para omitir): ";
        cin.ignore();  // Para evitar problemas si el usuario presiona Enter
        getline(cin, velocidad);
        X.push_back(x);
        Y.push_back(x);
        Z.push_back(x);
        parametros.push_back(X);
        parametros.push_back(Y);
        parametros.push_back(Z);
        if (!velocidad.empty()) {
            Velocidad.push_back(velocidad);
            parametros.push_back(Velocidad);
        }
    } else if (comando == "9") {
        vector<string> Accion_efector;
        string accion_efector;
        cout << "'1' para activar efector, '0' para desactivar: ";
        cin >> accion_efector;
        Accion_efector.push_back(accion_efector);
        parametros.push_back(Accion_efector);
    } else if (comando == "10") {
        vector<string> Nombre_archivo;
        vector<string> archivo;
        string nombre_archivo;

        cout << "Ingrese el nombre del archivo de texto (sin extension): ";
        cin >> nombre_archivo;
        Nombre_archivo.push_back(nombre_archivo);
        parametros.push_back(Nombre_archivo);

        // Preguntar al operador si desea enviar el archivo
        char opcion;
        cout << "¿Desea enviar el archivo de texto? (s/n): ";
        cin >> opcion;

        if (opcion == 's' || opcion == 'S') {
            // Abrir el archivo
            ifstream archivo_texto(nombre_archivo + ".txt");
            if (archivo_texto.is_open()) {
                string linea;
                // Leer línea por línea y agregar al vector `archivo`
                while (getline(archivo_texto, linea)) {
                    archivo.push_back(linea);
                }
                archivo_texto.close();

                // Agregar el vector `archivo` a `parametros`
                parametros.push_back(archivo);
                cout << "Archivo enviado correctamente.\n";
            } else {
                cerr << "Error al abrir el archivo: " << nombre_archivo << ".txt\n";
            }
        }
    }
    
    // Otros comandos no necesitan parámetros, devolvemos un vector vacío
    return parametros;
}
