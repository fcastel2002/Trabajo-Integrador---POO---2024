//
// Created by Usuario on 09/10/2024.
//

#ifndef ORDEN_H
#define ORDEN_H

#include <string>
#include <map>



class Orden {
private:
    std::string tipo;
    std::map<std::string, std::string> parametros;
public:
    Orden(std::string tipo, std::map<std::string, std::string> parametros);
    std::string serializar() const;

};



#endif //ORDEN_H
