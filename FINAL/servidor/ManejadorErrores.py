class ErrorDeConexion(Exception):
    def __init__(self, tipo):
        mensajes = {
            1: "Falla: No hay conexion con el sistema. Verifique el comando 'conectar'.",
            2: "Falla: El sistema esta inactivo. Verifique que los motores esten encendidos."
        }
        self.mensaje = mensajes.get(tipo, "Error de conexion desconocido.")

    def __str__(self):
        return self.mensaje


class ErrorDeParametros(Exception):
    def __init__(self, tipo):
        mensajes = {
            1: "Parametros fuera de los limites aceptados.",
            2: "Numero de parametros incorrecto. Use 'ayuda <comando>' para mas informacion.",
            3: "Parametros invalidos. Use 'ayuda <comando>' para mas informacion."
        }
        self.mensaje = mensajes.get(tipo, "Error de parametros.")

    def __str__(self):
        return self.mensaje


class ErrorDeEstado(Exception):
    def __init__(self, tipo):
        mensajes = {
            1: "Los motores ya estan apagados.",
            2: "Los motores ya estan encendidos.",
            3: "La conexion ya esta activa.",
            4: "La conexion ya esta inactiva.",
            5: "El efector ya esta activado.",
            6: "El efector ya esta desactivado.",
            7: "El robot ya esta en modo aprendizaje.",
            8: "El robot no esta en modo aprendizaje.",
            9: "No se puede ejecutar en modo aprendizaje activo."
        }
        self.mensaje = mensajes.get(tipo, "Error de estado.")

    def __str__(self):
        return self.mensaje

class ErrorArchivos(Exception):
    def __init__(self, tipo, nombre_archivo = None):
        mensajes = {
            1: "No se pudo escribir en el archivo",
            2: f"El archivo {nombre_archivo} no existe."
        }
        self.mensaje = mensajes.get(tipo, "Error de estado.")

    def __str__(self):
        return self.mensaje