#include "Interfaz.h"
#include <iostream>

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

vector<string> Interfaz::solicitarParametros(const string& comando) {
    vector<string> parametros;

    if (comando == "1") {  // conectar
        string puertoCOM, tasaBaudios;
        cout << "Ingrese el puerto COM: ";
        cin >> puertoCOM;
        cout << "Ingrese la tasa de baudios: ";
        cin >> tasaBaudios;
        parametros.push_back(puertoCOM);
        parametros.push_back(tasaBaudios);
    } else if (comando == "5") {  // mover_efector
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

        parametros.push_back(x);
        parametros.push_back(y);
        parametros.push_back(z);
        if (!velocidad.empty()) {
            parametros.push_back(velocidad);
        }
    } else if (comando == "9") {
        string accion_efector;
        cout << "'1' para activar efector, '0' para desactivar: ";
        cin >> accion_efector;
        parametros.push_back(accion_efector);
    } else if (comando == "10") {
        string nombre_archivo;
        cout <<"Ingrese el nombre del archivo de texto (sin extension): ";
        cin >> nombre_archivo;
        parametros.push_back(nombre_archivo);
    }
    
    // Otros comandos no necesitan parámetros, devolvemos un vector vacío
    return parametros;
}
