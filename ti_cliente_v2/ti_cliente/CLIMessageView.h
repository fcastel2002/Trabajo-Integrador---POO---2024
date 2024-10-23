#ifndef CLIMESSAGEVIEW_H
#define CLIMESSAGEVIEW_H

#include <string>
#include "XmlRpc.h"
#pragma comment(lib, "pdcurses.a")
#pragma comment(lib, "MSVCRTD.lib")
#pragma comment(lib, "libcmtd.lib")  
using namespace XmlRpc;
class CLIMessageView {
public:
    void mostrarMensaje(const std::string& mensaje);
    void mostrarError(const std::string& error);
    void mostrarRespuesta(XmlRpcValue& resultado);
};

#endif
