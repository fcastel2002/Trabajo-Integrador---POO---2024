#include "Servidor.h"
#include "Interfaz.h"
#include <thread>

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Uso: ./servidor <puerto>\n";
        return -1;
    }

    int puerto = atoi(argv[1]);
    srand(time(0));

    ServidorNumeros servidor;
    MetodoGenerarNumero metodo(&servidor);
    MetodoRegistrarSecuencia metodoRegistrar(&servidor);
    Interfaz interfaz;

    servidor.bindAndListen(puerto);
    interfaz.mostrarMensaje("Servidor ejecutandose en el puerto " + to_string(puerto) + ". Presiona Ctrl+C para detenerlo.");

    thread t([&servidor]() { servidor.work(-1.0); });

    int opcion;
    do {
        opcion = interfaz.solicitarOpcionMenuServidor();

        switch (opcion) {
            case 1: {
                vector<string> resultados = servidor.obtenerCantidadUsuarios();
                for (const string& mensaje : resultados) {
                    interfaz.mostrarMensaje(mensaje);
                }
                break;
            }
            case 2: {
                string alias = interfaz.solicitarAlias();
                vector<string> estadisticas = servidor.obtenerEstadisticasUsuario(alias);
                for (const string& mensaje : estadisticas) {
                    interfaz.mostrarMensaje(mensaje);
                }
                break;
            }
            case 3:
                interfaz.mostrarMensaje("Apagando el servidor...");
                servidor.apagarServidor();
                break;
            default:
                interfaz.mostrarMensaje("Opcion no valida. Intente nuevamente.");
                break;
        }
    } while (opcion != 3);

    t.join();
    return 0;
}




