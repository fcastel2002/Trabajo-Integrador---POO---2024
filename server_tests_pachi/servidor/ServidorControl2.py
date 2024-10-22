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
            with open("usuarios.json", "r") as archivo:
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

    def interpreta_comando(self, mensaje_json):
        # Deserializar el mensaje y ejecutar el comando correspondiente
        try:
            mensaje = json.loads(mensaje_json)
            usuario = mensaje["usuario"]
            clave = mensaje["clave"]
            comando = mensaje["comando"]
            parametros = mensaje["parametros"]

            if not self._validar_usuario(usuario, clave):
                return json.dumps({"error": "Acceso denegado: Usuario o clave incorrectos"})

            resultado = self._ejecutar_comando(comando, parametros)
            self.logger.registrar_log(comando, "127.0.0.1", usuario, True)
            return json.dumps({"resultado": resultado})

        except Exception as e:
            self.logger.registrar_log("error", "127.0.0.1", "sistema", False)
            return json.dumps({"error": f"Error al interpretar el comando: {str(e)}"})

    def _ejecutar_comando(self, comando, parametros):
        # Ejecutar el comando recibido
        if comando == "conectar":
            self.robot.puerto_serial = parametros["puerto_COM"]
            self.robot.baudios = int(parametros["tasa_baudios"])
            try:
                return json.dumps({"Respuesta":self.robot.conectar()})
            except Exception as e:
                return json.dumps({"Respuesta":{e}})
        elif comando == "desconectar":
            try: 
                return json.dumps({"Respuesta":self.robot.desconectar()})
            except Exception as e:
                return json.dumps({"Respuesta":{e}})
        elif comando == "activar_motores":
            try:
                return json.dumps({"Respuesta":self.robot.activar_motores()})
            except Exception as e:
                return json.dumps({"Respuesta":{e}})
        elif comando == "desactivar_motores":
            try:
                return json.dumps({"Respuesta":self.robot.desactivar_motores()})
            except Exception as e:
                return json.dumps({"Respuesta":{e}})
        elif comando == "mover_efector":
            x = parametros["X"]
            y = parametros["Y"]
            z = parametros["Z"]
            velocidad = parametros["velocidad"] if "velocidad" in parametros else None
            if velocidad is not None:
                try:
                    return json.dumps({"Respuesta":self.robot.mover_efector(x,y,z,velocidad)})
                except Exception as e:
                    return json.dumps({"Respuesta":{e}})
            else:
                try:
                    return json.dumps({"Respuesta":self.robot.mover_efector_posicion(x,y,z)})
                except Exception as e:
                    return json.dumps({"Respuesta":{e}})
        elif comando == "homming":
            try:
                return json.dumps({"Respuesta":self.robot.homming()})
            except Exception as e:
                return json.dumps({"Respuesta":{e}})
        elif comando == "reportar_estado":
            try:
                return json.dumps({"Respuesta":self.robot.reportar()})
            except Exception as e:
                return json.dumps({"Respuesta":{e}})
        elif comando == "reportar_posicion":
            try:
                return json.dumps({"Respuesta":self.robot.reportar_posicion()})
            except Exception as e:
                return json.dumps({"Respuesta":{e}})
        elif comando == "actuar_efector":
            accion_efector = parametros["accion_efector"]
            try: 
                return json.dumps({"Respuesta":self.robot.actuar_efector(accion_efector)})
            except Exception as e:
                return json.dumps({"Respuesta":{e}})
        elif comando == "ejecutar_automatico":
            nombre_archivo = parametros["nombre_archivo"]
            try: 
                return json.dumps({"Respuesta":self.robot.ejecutar_automatico(nombre_archivo)})
            except Exception as e:
                return json.dumps({"Respuesta":{e}})

        elif comando == "comandos":
            
            try:
                comandos_disponibles = self.consola.opciones_menu
                return json.dumps({comandos_disponibles})
            except Exception as e:
                return json.dumps({"Respuesta":{e}})
        else:
            raise ValueError(f"Comando '{comando}' no reconocido.")

    