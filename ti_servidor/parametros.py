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