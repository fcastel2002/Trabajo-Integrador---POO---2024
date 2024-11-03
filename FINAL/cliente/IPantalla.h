#pragma once
#include <string>
#include <vector>

class IPantalla {
public:
    virtual ~IPantalla() = default;

    // Muestra un mensaje de texto en pantalla
    virtual void mostrarTexto(const std::string& mensaje) = 0;

    // Muestra un mensaje de error en pantalla
    virtual void mostrarError(const std::string& error) = 0;

    // Limpia la pantalla
    virtual void limpiarPantalla() = 0;

    // Captura la entrada del usuario mostrando un mensaje
    virtual std::string capturarEntrada(const std::string& mensaje) = 0;
    virtual std::string capturarEntrada(const std::string& mensaje, const std::vector<std::string>& opciones) = 0;


    // Muestra un menú y retorna la opción elegida por el usuario
    virtual int mostrarMenu(const std::vector<std::string>& opciones, const std::string& tituloMenu) = 0;

    // Refresca la pantalla, actualizando su contenido
    virtual void refrescarPantalla() = 0;
};
