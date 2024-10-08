# main.py
import questionary
import sys
import os

# Lista predefinida de usuarios
USERS = [
    {"username": "admin", "role": "Administrador"},
    {"username": "user1", "role": "Usuario"},
    {"username": "user2", "role": "Usuario"},
    # Puedes agregar más usuarios aquí
]

# Parámetros de conexión predefinidos (simulados)
CONNECTION_PARAMS = {
    "Puerto Serial": "/dev/ttyUSB0",
    "Baudios": "9600"
}

# Lista simulada de archivos Gcode
GCODE_FILES = [
    "movimiento_basico.gcode",
    "rutina_ensamblaje.gcode",
    "operacion_pintura.gcode",
    "prueba_movimiento.gcode"
]

# Contraseña ficticia para administradores
ADMIN_PASSWORD = "admin123"

class InterfazServidor:
    def __init__(self, servidor = None):
        self.servidor = servidor
        
    def menu(self):
            while True:
                choice = questionary.select(
                    "Seleccione una opción:",
                    choices=[
                        "Conectar al Robot",
                        "Desconectar del Robot",
                        "Obtener Mensaje de Ayuda",
                        "Reporte General",
                        "Reporte Detallado de Logs",
                        "Seleccionar Modo de Trabajo",
                        "Ver Usuarios Actuales",
                        "Mover Efector Final",
                        "Activar Motores del Robot",
                        "Desactivar Motores del Robot",
                        "Maniobra de Homing",
                        "Carga y Ejecución de Archivo Gcode",
                        "Mostrar/Editar Parámetros de Conexión del Robot",
                        "Salir"
                    ]
                ).ask()
                
                os.system('cls')
                
                if choice == "Conectar al Robot":
                    self.conectar_robot()
                elif choice == "Desconectar del Robot":
                    self.desconectar_robot()
                elif choice == "Obtener Mensaje de Ayuda":
                    self.obtener_mensaje_ayuda()
                elif choice == "Reporte General":
                    self.reporte_general()
                elif choice == "Reporte Detallado de Logs":
                    self.reporte_detallado_logs()
                elif choice == "Seleccionar Modo de Trabajo":
                    self.seleccionar_modo_trabajo()
                elif choice == "Ver Usuarios Actuales":
                    self.ver_usuarios_actuales()
                elif choice == "Mover Efector Final":
                    self.mover_efector_final()
                elif choice == "Activar Motores del Robot":
                    self.activar_motores()
                elif choice == "Desactivar Motores del Robot":
                    self.desactivar_motores()
                elif choice == "Maniobra de Homing":
                    self.maniobra_homing()
                elif choice == "Carga y Ejecución de Archivo Gcode":
                    self.carga_ejecucion_gcode()
                elif choice == "Mostrar/Editar Parámetros de Conexión del Robot":
                    self.mostrar_editar_parametros_conexion()
                elif choice == "Salir":
                    self.salir()
                else:
                    print("\nOpción no reconocida. Por favor, intente de nuevo.\n")
                    
    def conectar_robot(self):
        print("\nConectando al robot...")
        # Aquí puedes agregar la lógica real para conectar al robot
        print("Robot conectado exitosamente.\n")
        
    def desconectar_robot(self):
        print("\nDesconectando del robot...")
        # Aquí puedes agregar la lógica real para desconectar del robot
        print("Robot desconectado exitosamente.\n")
                    
    def obtener_mensaje_ayuda(self):
        print("\n=== Ayuda de Todos los Comandos ===")
        ayuda = {
            "Conectar al Robot": "Conecta el servidor al robot mediante comunicación serie.",
            "Desconectar del Robot": "Desconecta el servidor del robot.",
            "Obtener Mensaje de Ayuda": "Proporciona mensajes de ayuda para todos los comandos disponibles.",
            "Reporte General": "Ofrece un reporte general del estado del servidor y el robot.",
            "Reporte Detallado de Logs": "Ofrece un reporte detallado del log de trabajo del servidor.",
            "Seleccionar Modo de Trabajo": "Selecciona los modos de trabajo (manual/automático, absoluto/relativo).",
            "Ver Usuarios Actuales": "Muestra la lista de usuarios actuales.",
            "Mover Efector Final": "Mueve el efector final a una posición específica.",
            "Activar Motores del Robot": "Activa los motores del robot.",
            "Desactivar Motores del Robot": "Desactiva los motores del robot.",
            "Maniobra de Homing": "Realiza una maniobra de homing del robot.",
            "Carga y Ejecución de Archivo Gcode": "Carga y ejecuta un archivo Gcode seleccionado."
        }
        for comando, descripcion in ayuda.items():
            print(f"- {comando}: {descripcion}")
        print()
    
    def reporte_general(self):
        print("\n=== Reporte General ===")
        # Aquí puedes agregar la lógica real para obtener el reporte
        reporte = {
            "Estado de Conexión": "Conectado",
            "Posición Actual": "X: 10, Y: 20, Z: 30",
            "Estado de Actividad": "Activo",
            "Inicio de Actividad": "2024-04-27 12:00:00",
            "Órdenes Solicitadas": 25,
            "Órdenes con Error": 2
        }
        for clave, valor in reporte.items():
            print(f"{clave}: {valor}")
        print()
        
    def reporte_detallado_logs(self):
        print("\n=== Reporte Detallado de Logs ===")
        # Aquí puedes agregar la lógica real para obtener los logs
        logs = [
            "2024-04-27 12:00:00 - Conectar al Robot - Éxito - Robot conectado.",
            "2024-04-27 12:05:00 - Activar Motores del Robot - Éxito - Motores activados.",
            "2024-04-27 12:10:00 - Desactivar Motores del Robot - Error - No se pudo desactivar.",
            "2024-04-27 12:15:00 - Maniobra de Homing - Éxito - Homing realizado.",
            "2024-04-27 12:20:00 - Carga y Ejecución de Archivo Gcode - Éxito - Archivo ejecutado."
            # Agrega más líneas de log según sea necesario
        ]
        for log in logs:
            print(log)
        print()


    def seleccionar_modo_trabajo(self):
        print("\n=== Seleccionar Modo de Trabajo ===")
        modo = questionary.select(
            "Seleccione el modo de trabajo:",
            choices=["Manual", "Automático"]
        ).ask()
        
        submodo = questionary.select(
            "Seleccione el submodo de trabajo:",
            choices=["Absoluto", "Relativo"]
        ).ask()
        
        print(f"\nModo de trabajo establecido a {modo} - {submodo}.\n")
        # Aquí puedes agregar la lógica real para establecer el modo de trabajo

    def ver_usuarios_actuales(self):
        print("\n=== Usuarios Actuales ===")
        for user in USERS:
            print(f"- {user['username']} ({user['role']})")
        print()

    def mover_efector_final(self):
        print("\n=== Mover Efector Final ===")
        # Solicitar posición final
        x = questionary.text("Ingrese la coordenada X final:").ask()
        y = questionary.text("Ingrese la coordenada Y final:").ask()
        z = questionary.text("Ingrese la coordenada Z final:").ask()
        
        # Validar que se proporcionen valores
        if not (x and y and z):
            print("\nError: Debes proporcionar una posición final completa (X, Y, Z).\n")
            return
    
        # Preguntar si desea especificar la velocidad
        desea_velocidad = questionary.confirm("¿Desea especificar la velocidad del movimiento?").ask()
        
        if desea_velocidad:
            velocidad = questionary.text("Ingrese la velocidad del movimiento:").ask()
            if not velocidad:
                print("\nError: Debes proporcionar un valor para la velocidad.\n")
                return
            print(f"\nMoviendo el efector final a la posición ({x}, {y}, {z}) con velocidad {velocidad}.\n")
            # Aquí puedes agregar la lógica real para mover el efector con velocidad
        else:
            print(f"\nMoviendo el efector final a la posición ({x}, {y}, {z}) con velocidad por defecto.\n")
            # Aquí puedes agregar la lógica real para mover el efector sin especificar velocidad

    def activar_motores(self):
        print("\n=== Activar Motores del Robot ===")
        # Aquí puedes agregar la lógica real para activar los motores
        print("Motores activados exitosamente.\n")

    def desactivar_motores(self):
        print("\n=== Desactivar Motores del Robot ===")
        # Aquí puedes agregar la lógica real para desactivar los motores
        print("Motores desactivados exitosamente.\n")

    def maniobra_homing(self):
        print("\n=== Maniobra de Homing ===")
        # Aquí puedes agregar la lógica real para realizar la maniobra de homing
        print("Maniobra de homing realizada exitosamente.\n")

    def carga_ejecucion_gcode(self):
        print("\n=== Carga y Ejecución de Archivo Gcode ===")
        archivo = questionary.select(
            "Seleccione el archivo Gcode a ejecutar:",
            choices=GCODE_FILES
        ).ask()
        
        if archivo:
            print(f"\nCargando y ejecutando el archivo '{archivo}'...\n")
            # Aquí puedes agregar la lógica real para cargar y ejecutar el archivo Gcode
            print(f"Archivo '{archivo}' ejecutado exitosamente.\n")
        else:
            print("\nNo se seleccionó ningún archivo.\n")

    def mostrar_editar_parametros_conexion(self):
        print("\n=== Mostrar/Editar Parámetros de Conexión del Robot ===")
        # Solicitar confirmación de contraseña ficticia
        password = questionary.password("Ingrese la contraseña de administrador para continuar:").ask()
        
        if password == ADMIN_PASSWORD:
            print("\nContraseña verificada exitosamente.\n")
            while True:
                action = questionary.select(
                    "Seleccione una acción:",
                    choices=["Mostrar Parámetros", "Editar Parámetros", "Volver al Menú Principal"]
                ).ask()
                
                if action == "Mostrar Parámetros":
                    print("\n=== Parámetros de Conexión del Robot ===")
                    for clave, valor in CONNECTION_PARAMS.items():
                        print(f"{clave}: {valor}")
                    print()
                elif action == "Editar Parámetros":
                    puerto = questionary.text("Ingrese el nuevo Puerto Serial:").ask()
                    baudios = questionary.text("Ingrese la nueva Tasa de Baudios:").ask()
                    
                    if puerto:
                        CONNECTION_PARAMS["Puerto Serial"] = puerto
                    if baudios:
                        CONNECTION_PARAMS["Baudios"] = baudios
                    
                    print("\nParámetros de conexión actualizados exitosamente.\n")
                elif action == "Volver al Menú Principal":
                    break
                else:
                    print("\nOpción no reconocida. Por favor, intente de nuevo.\n")
        else:
            print("\nContraseña incorrecta. Acceso denegado.\n")

    def salir(self):
        print("\nSaliendo del Panel de Control. ¡Hasta luego!")
        sys.exit(0)

def main():
    interfaz = InterfazServidor()
    interfaz.menu()

if __name__ == "__main__":
    main()