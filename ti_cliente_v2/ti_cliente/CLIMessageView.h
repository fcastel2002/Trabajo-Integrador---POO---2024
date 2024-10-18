#ifndef CLIMESSAGEVIEW_H
#define CLIMESSAGEVIEW_H

#include <string>
#pragma comment(lib, "pdcurses.a")
#pragma comment(lib, "MSVCRTD.lib")
#pragma comment(lib, "libcmtd.lib")  

class CLIMessageView {
public:
    void mostrarMensaje(const std::string& mensaje);
    void mostrarError(const std::string& error);
    void mostrarRespuesta(const std::string& respuesta);
};

#endif
