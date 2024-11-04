from xmlrpc.server import SimpleXMLRPCServer
from Logger import Logger
import json
import threading
from ManejadorErrores import ErrorArchivos

class ServidorControl:
    def __init__(self, consola, robot, ip="127.0.0.1", puerto=9000):
        self.consola = consola
        self.robot = robot
        self.ip = ip
        self.puerto = puerto
        self.server_thread = None
        self.server = None
        self.logger = Logger()
        self.usuarios_autorizados = None
        
        
        self.current_user = None
        self.current_clave = None
        self.current_comando = None
        self.respuestas = ["","",[]]
    
    def _cargar_usuarios(self):
        # Cargar usuarios desde un archivo JSON
        try:
            file_name = "usuarios.json"
            with open(file_name, "r") as archivo:
                self.usuarios_autorizados = json.load(archivo)
            return "Usuarios cargados correctamente."
        except FileNotFoundError:
            raise ErrorArchivos(2, file_name)

    def _validar_usuario(self, usuario, clave):
        # Validar que el usuario y la clave sean correctos
        usuario = usuario.lower()
        self.current_user = usuario
        self.current_clave = clave
        
        if self.current_user in self.usuarios_autorizados and self.usuarios_autorizados[usuario] == self.current_clave:
            return True
        return False

    def iniciar(self):
        # Crear el servidor y registrar la única función "Interpreta_Comando"
        self.server = SimpleXMLRPCServer((self.ip, self.puerto), allow_none=True, logRequests=False)
        self.server.register_function(self.interpreta_comando, "Interpreta_Comando")
        self.server_thread = threading.Thread(target=self._iniciar_servidor)
        self.server_thread.daemon = True
        self.server_thread.start()
        return f"Servidor RPC iniciado en {self.ip}:{self.puerto}\n"

    def _iniciar_servidor(self):
        try:
            self.server.serve_forever()
        except KeyboardInterrupt:
            self.server.shutdown()

    def disconnect(self):
        # Detener el servidor
        if self.server:
            self.server.shutdown()
            self.server.server_close()
            self.server_thread.join()  # Asegurarse de que el hilo termine
            return "Servidor cerrado correctamente."
        

    def interpreta_comando(self, usuario, clave, comando, parametros = None):
        # Validar usuario y clave
        if not self._validar_usuario(usuario, clave):
            mensaje_error = self.crear_respuesta("Acceso denegado: Usuario o clave incorrectos")
            return mensaje_error

        try:
            # Ejecutar el comando correspondiente
            #print(f"{comando}\n")
            self.current_comando = comando
            resultado = self._ejecutar_comando(comando, parametros)
            self.crear_respuesta(resultado)
            resultado = self.respuestas
            
            mensaje_log = self.logger.registrar_log(comando, "127.0.0.1", usuario, True)
            if mensaje_log:
                return mensaje_log
            
            return resultado

        except Exception as e:
            # Registrar error en el log y devolver mensaje de error
            mensaje_log = self.logger.registrar_log("error", "127.0.0.1", "sistema", False)
            if mensaje_log:
                return f"Error al interpretar el comando: {str(e)}"
            else:
                return f"Error al interpretar el comando: {str(e)}"

    def crear_respuesta(self, resultado):
        # Asegurarse de que `self.respuestas` sea una lista con tres elementos
        self.respuestas = ["", "", []]  # Inicializar con tipos específicos
        
        # Asignar el usuario y comando como strings
        self.respuestas[0] = self.current_user
        self.respuestas[1] = self.current_comando
        
        # Asignar `resultado` como lista en la posición [2]
        if isinstance(resultado, list):
            self.respuestas[2] = resultado
        else:
            self.respuestas[2] = [resultado]

    def _ejecutar_comando(self, comando, parametros = None):
        comandos_disponibles = [
        "Conectarse al Robot", "Desconectar Robot", "Activar motores", "Desactivar motores",
        "Mover efector", "Realizar maniobra homming", "Reportar estado", "Reportar posicion",
        "Actuar efector", "Ejecutar automatico", "Cambiar modo", "Aprendizaje", "comandos" ]
        # Ejecutar el comando recibido
        if comando == comandos_disponibles[0]:
            self.robot.puerto_serial = parametros[0]  # puerto_COM
            self.robot.baudios = int(parametros[1])   # tasa_baudios
            try:
                return self.robot.conectar()
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == comandos_disponibles[1]:
            try: 
                return self.robot.desconectar()
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == comandos_disponibles[2]:
            try:
                return self.robot.activar_motores()
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == comandos_disponibles[3]:
            try:
                return self.robot.desactivar_motores()
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == comandos_disponibles[4]:
            x = float(parametros[0])  # X
            y = float(parametros[1])  # Y
            z = float(parametros[2])  # Z
            if len(parametros) > 3 and not parametros[3] == "":
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
                    return self.robot.mover_efector(x, y, z)
                except Exception as e:
                    return f"Error: {str(e)}"
        
        elif comando == comandos_disponibles[5]:
            try:
                return self.robot.homming()
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == comandos_disponibles[6]:
            try:
                return self.robot.reportar()
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == comandos_disponibles[7]:
            try:
                return self.robot.reportar_posicion()
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == comandos_disponibles[8]:
            accion_efector = parametros[0]  # acción sobre el efector
            try: 
                return self.robot.actuar_efector(accion_efector)
            except Exception as e:
                return f"Error: {str(e)}"
        
        elif comando == comandos_disponibles[9]:
            nombre_archivo = parametros[0]  # nombre del archivo
            if parametros[1] is not None:
                archivo = []
                for i in range(1,len(parametros)):
                    archivo.append(parametros[i])
                try: 
                    return self.robot.ejecutar_automatico(nombre_archivo, archivo)
                except Exception as e:
                    return f"Error: {str(e)}"
            else:        
                try: 
                    return self.robot.ejecutar_automatico(nombre_archivo)
                except Exception as e:
                    return f"Error: {str(e)}"
        elif comando == comandos_disponibles[10]:
            try:
                if self.consola.tipo_movimiento == 'absoluto':
                    return self.robot.modo_relativo()
                else:
                    return self.robot.modo_absoluto()
            except Exception as e:
                return f"Error: {str(e)}"
        elif comando == comandos_disponibles[11]:
            nombre_archivo = parametros[0]
            activacion = None
            try:
                activacion = parametros[1]
            except Exception:
                activacion = None
                return f"Error: Debe enviar segundo parametro para activacion o desactivacion"
            try:
                return self.robot.aprender(nombre_archivo, activacion)
            except Exception as e:
                return f"Error: {str(e)}"
        elif comando == comandos_disponibles[12]:
            return comandos_disponibles
        else:
            raise ValueError(f"Comando '{comando}' no reconocido.")

