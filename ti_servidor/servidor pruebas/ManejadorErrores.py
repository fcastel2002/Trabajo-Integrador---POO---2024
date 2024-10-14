class ErrorDeConexion(Exception):
    def __init__(self, tipo):
        mensajes = {
            1: "Falla: No hay conexión con el sistema. Verifique el comando 'conectar'.",
            2: "Falla: El sistema está inactivo. Verifique que los motores estén encendidos."
        }
        self.mensaje = mensajes.get(tipo, "Error de conexión desconocido.")

    def __str__(self):
        return self.mensaje

class ErrorDeParametros(Exception):
    def __init__(self, tipo):
        mensajes = {
            1: "Parámetros fuera de los límites aceptados.",
            2: "Número de parámetros incorrecto. Use 'ayuda <comando>' para más información.",
            3: "Parámetros inválidos. Use 'ayuda <comando>' para más información."
        }
        self.mensaje = mensajes.get(tipo, "Error de parámetros.")

    def __str__(self):
        return self.mensaje

class ErrorDeEstado(Exception):
    def __init__(self, tipo):
        mensajes = {
            1: "Los motores ya están apagados.",
            2: "Los motores ya están encendidos.",
            3: "La conexión ya está activa.",
            4: "La conexión ya está inactiva."
        }
        self.mensaje = mensajes.get(tipo, "Error de estado.")

    def __str__(self):
        return self.mensaje
