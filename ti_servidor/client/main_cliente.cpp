#include "Cliente.h"
#include "Interfaz.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Uso: ./cliente <servidor_url> <puerto>\n";
        return -1;
    }

    string servidorUrl = argv[1];
    int puerto = stoi(argv[2]);
    ClienteNumeros cliente(servidorUrl, puerto);
    Interfaz interfaz;

    string alias = interfaz.solicitarAlias();
    string ip = interfaz.solicitarIP();
    cliente.setAlias(alias, ip);
    cliente.iniciarNuevaSecuencia();

    int opcion;
    do {
        opcion = interfaz.solicitarOpcionMenuCliente();

        switch (opcion) {
            case 1:
                cliente.solicitarNumero(false);
                break;
            case 2:
                cliente.solicitarNumero(true);
                break;
            case 3:
                cliente.mostrarEstadisticas();
                break;
            case 4:
                cliente.mostrarNumerosEnOrden();
                break;
            case 5:
                alias = interfaz.solicitarAlias();
                ip = interfaz.solicitarIP();
                cliente.setAlias(alias, ip);
                cliente.iniciarNuevaSecuencia();
                break;
            case 6:
                cliente.iniciarNuevaSecuencia();
                break;
            case 7:
                interfaz.mostrarMensaje("Saliendo...");
                break;
            default:
                interfaz.mostrarMensaje("Opcion no valida. Intente nuevamente.");
                break;
        }
    } while (opcion != 7);

    return 0;
}


