from xmlrpc.server import SimpleXMLRPCServer
from Logger import Logger
import json
import threading


class ServidorControl:
    def __init__(self, consola, robot, ip="127.0.0.1", puerto=9000):
        self.consola = consola
        self.robot = robot
        self.ip = ip
        self.puerto = puerto
        self.server_thread = None
        self.server = None
        self.logger = Logger()
        self.usuarios_autorizados = self._cargar_usuarios()

    def _cargar_usuarios(self):
        # Cargar usuarios desde un archivo JSON
        try:
            #with open("servidor\\usuarios.json", "r") as archivo:
            with open("servidor\\usuarios.json", "r") as archivo:
                return json.load(archivo)
        except FileNotFoundError:
            print("No se encontró el archivo de usuarios.")
            return {}

    def _validar_usuario(self, usuario, clave):
        # Validar que el usuario y la clave sean correctos
        if usuario in self.usuarios_autorizados and self.usuarios_autorizados[usuario] == clave:
            return True
        return False

    def iniciar(self):
        # Crear el servidor y registrar la única función "Interpreta_Comando"
        self.server = SimpleXMLRPCServer((self.ip, self.puerto), allow_none=True, logRequests=False)
        self.server.register_function(self.interpreta_comando, "Interpreta_Comando")
        self.server_thread = threading.Thread(target=self._iniciar_servidor)
        self.server_thread.daemon = True
        self.server_thread.start()
        print(f"Servidor RPC iniciado en {self.ip}:{self.puerto}\n")

    def _iniciar_servidor(self):
        try:
            self.server.serve_forever()
        except KeyboardInterrupt:
            self.server.shutdown()

    def disconnect(self):
        # Detener el servidor
        if self.server:
            print("Iniciando cierre del servidor...")
            self.server.shutdown()
            self.server.server_close()
            self.server_thread.join()  # Asegurarse de que el hilo termine
            print("Servidor cerrado correctamente.")

    def interpreta_comando(self, usuario, clave, comando, parametros = None):
        # Validar usuario y clave
        if not self._validar_usuario(usuario, clave):
            return {"error": "Acceso denegado: Usuario o clave incorrectos"}

        try:
            # Ejecutar el comando correspondiente
            resultado = self._ejecutar_comando(comando, parametros)
            
            # Registrar la operación en el log
            self.logger.registrar_log(comando, "127.0.0.1", usuario, True)
            return {"resultado": resultado}

        except Exception as e:
            # Registrar error en el log y devolver mensaje de error
            self.logger.registrar_log("error", "127.0.0.1", "sistema", False)
            return {"error": f"Error al interpretar el comando: {str(e)}"}


    def _ejecutar_comando(self, comando, parametros = None):
        # Ejecutar el comando recibido
        if comando == "conectar":
            self.robot.puerto_serial = parametros[0]  # puerto_COM
            self.robot.baudios = int(parametros[1])   # tasa_baudios
            try:
                return self.robot.conectar()
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == "desconectar":
            try: 
                return self.robot.desconectar()
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == "activar_motores":
            try:
                return self.robot.activar_motores()
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == "desactivar_motores":
            try:
                return self.robot.desactivar_motores()
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == "mover_efector":
            x = float(parametros[0])  # X
            y = float(parametros[1])  # Y
            z = float(parametros[2])  # Z
            if len(parametros) > 3:
                velocidad = float(parametros[3])  # velocidad
            else:
                velocidad = None
            
            if velocidad is not None:
                try:
                    return self.robot.mover_efector(x, y, z, velocidad)
                except Exception as e:
                    return f"Error: {str(e)}"
            else:
                try:
                    return self.robot.mover_efector_posicion(x, y, z)
                except Exception as e:
                    return f"Error: {str(e)}"
        
        elif comando == "homming":
            try:
                return self.robot.homming()
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == "reportar_estado":
            try:
                return self.robot.reportar()
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == "reportar_posicion":
            try:
                return self.robot.reportar_posicion()
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == "actuar_efector":
            accion_efector = parametros[0]  # acción sobre el efector
            try: 
                return self.robot.actuar_efector(accion_efector)
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == "ejecutar_automatico":
            nombre_archivo = parametros[0]  # nombre del archivo
            try: 
                return self.robot.ejecutar_automatico(nombre_archivo)
            except Exception as e:
                return f"Error: {str(e)}"
        elif comando == "Cambiar modo"
        
        else:
            raise ValueError(f"Comando '{comando}' no reconocido.")

