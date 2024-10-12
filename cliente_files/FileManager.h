//
// Created by Usuario on 09/10/2024.
//

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>

class FileManager {
std::string m_rutaArchivo{};
public:
    FileManager(std::string rutaArchivo);
    std::string cargarArchivo();
    std::string serializar() const;
};



#endif //FILEMANAGER_H
