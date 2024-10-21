#include "Cliente.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Uso: ./cliente <servidor_url> <puerto>\n";
        return -1;
    }
    string servidorUrl = argv[1];
    int puerto = stoi(argv[2]);
    ClienteRobot cliente(servidorUrl, puerto);
    cliente.setAlias("robot_client", "127.0.0.1");
    cliente.mostrarMenu();
    return 0;
}
