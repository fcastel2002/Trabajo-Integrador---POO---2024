1. Crear el proyecto

2. Agregar los archivos de la librería XMLRPC
   (preferentemente en un directorio)

3. Codificar el programa deseado

Cambios a realizar sobre plataforma Windows:

4. Colocar en las primeras líneas del programa
   principal (y en el que corresponda)
   #include <windows.h>

5. Incluir en las primeras líneas de los archivos
   XmlRpcDispatch.cpp y XmlRpcSocket.cpp, 
   lo siguiente:
   #include <windows.h>
   #if defined(_WINDOWS_H)
   # define _WINDOWS Puma3D
   #endif  // _WINDOWS_H

6. Agregar en la línea de comandos de g++ 
   o configurar en el IDE, la opción de compilar
   usando librerías de windows: -lws2_32

7. Compilar