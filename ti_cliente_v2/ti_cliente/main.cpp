#include <iostream>
#include <curses.h>
#include <string>
#include <XmlRpc.h>

#pragma comment(lib, "pdcurses.a")
#pragma comment(lib, "MSVCRTD.lib")
#pragma comment(lib, "libcmtd.lib")
#pragma comment(lib, "xmlrpc++.lib")
// Simula la conexión con el servidor XMLRPC
void conectarServidor() {
    XmlRpc::XmlRpcClient client("127.0.0.1", 9000);
    XmlRpc::XmlRpcValue noArgs, result;
    client.execute("listar_comandos", noArgs, result);
    printw("Conectando al servidor...\n");
    refresh();
    // Aquí iría la lógica real de conexión al servidor
    printw("Conectado exitosamente!\n");
    refresh();
    getch(); // Esperar una tecla antes de continuar
}

// Simula el envío de solicitudes al servidor
void enviarSolicitud() {
    printw("Enviando solicitud al servidor...\n");
    refresh();
    // Aquí iría la lógica real de enviar la solicitud
    printw("Solicitud enviada!\n");
    refresh();
    getch(); // Esperar una tecla antes de continuar
}

int main() {
    initscr();              // Inicializa PDCurses
    noecho();               // No muestra lo que se escribe
    curs_set(0);            // Oculta el cursor
    keypad(stdscr, TRUE);   // Habilita teclas especiales (como las flechas)

    std::string opciones[] = { "Conectar al servidor", "Enviar solicitud", "Salir" };
    int seleccion = 0;      // Para mantener el índice de la opción seleccionada
    int n_opciones = sizeof(opciones) / sizeof(opciones[0]);

    while (true) {
        clear();            // Limpia la pantalla
        // Mostrar el menú
        for (int i = 0; i < n_opciones; i++) {
            if (i == seleccion) {
                attron(A_REVERSE); // Resaltar la opción seleccionada
            }
            mvprintw(i + 1, 1, opciones[i].c_str()); // Mostrar cada opción
            if (i == seleccion) {
                attroff(A_REVERSE); // Quitar resaltado
            }
        }
        refresh(); // Actualiza la pantalla

        // Manejo de entrada del usuario
        int input = getch(); // Captura la tecla presionada
        switch (input) {
        case KEY_UP:
            seleccion--; // Mover hacia arriba
            if (seleccion < 0) seleccion = n_opciones - 1;
            break;
        case KEY_DOWN:
            seleccion++; // Mover hacia abajo
            if (seleccion >= n_opciones) seleccion = 0;
            break;
        case 10: // Tecla Enter
            if (seleccion == 0) {
                clear();
                conectarServidor(); // Llamar a la función de conectar
            }
            else if (seleccion == 1) {
                clear();
                enviarSolicitud(); // Llamar a la función de enviar solicitud
            }
            else if (seleccion == 2) {
                endwin(); // Terminar PDCurses
                return 0; // Salir del programa
            }
            break;
        }
    }

    endwin(); // Termina PDCurses
    return 0;
}
