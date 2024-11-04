import pytest
from unittest.mock import patch, mock_open
from GestorDeArchivos import GestorDeArchivos
import json

@pytest.fixture
def gestor_archivos():
    # Fixture para crear un gestor de archivos con un nombre de archivo ficticio
    return GestorDeArchivos("archivo_test.txt")

def test_limpiar_archivo(gestor_archivos):
    # Probamos que el método limpiar_archivo vacíe el archivo
    with patch("builtins.open", mock_open()) as mock_file:
        gestor_archivos.limpiar_archivo()
        mock_file.assert_called_once_with("archivo_test.txt", "w")
        # Eliminamos la llamada a truncate, ya que abrir en modo "w" ya limpia el archivo

def test_guardar_linea(gestor_archivos):
    # Probamos que el método guardar_linea guarde correctamente una línea
    objeto_json = json.dumps({"clave": "valor"})
    with patch("builtins.open", mock_open()) as mock_file:
        gestor_archivos.guardar_linea(objeto_json)
        mock_file.assert_called_once_with("archivo_test.txt", "a")
        mock_file().write.assert_called_once_with(objeto_json + "\n")

def test_leer_contenido(gestor_archivos):
    # Probamos que el método leer_contenido lea y deserialice el contenido JSON
    contenido = '{"clave": "valor"}\n{"clave": "otro valor"}\n'
    with patch("builtins.open", mock_open(read_data=contenido)) as mock_file:
        resultado = gestor_archivos.leer_contenido()
        mock_file.assert_called_once_with("archivo_test.txt", "r")
        assert resultado == [{"clave": "valor"}, {"clave": "otro valor"}]

def test_contar_lineas(gestor_archivos):
    # Probamos que el método contar_lineas cuente correctamente el número de líneas
    contenido = "línea1\nlínea2\nlínea3\n"
    with patch("builtins.open", mock_open(read_data=contenido)) as mock_file:
        resultado = gestor_archivos.contar_lineas()
        mock_file.assert_called_once_with("archivo_test.txt", "r")
        assert resultado == 3

def test_leer_ultimas_lineas(gestor_archivos):
    # Probamos que el método leer_ultimas_lineas lea las últimas 'n' líneas correctamente
    contenido = "línea1\nlínea2\nlínea3\nlínea4\n"
    with patch("builtins.open", mock_open(read_data=contenido)) as mock_file:
        resultado = gestor_archivos.leer_ultimas_lineas(2)
        mock_file.assert_called_once_with("archivo_test.txt", "r")
        assert resultado == ["línea3\n", "línea4\n"]

def test_leer_csv(gestor_archivos):
    # Probamos que el método leer_csv lea correctamente el contenido de un archivo CSV
    contenido_csv = "col1,col2\nvalor1,valor2\nvalor3,valor4\n"
    with patch("builtins.open", mock_open(read_data=contenido_csv)) as mock_file:
        with patch("csv.reader", return_value=[
                ["col1", "col2"],
                ["valor1", "valor2"],
                ["valor3", "valor4"]]) as mock_csv_reader:
            resultado = gestor_archivos.leer_csv()
            mock_file.assert_called_once_with("archivo_test.txt", newline='')
            mock_csv_reader.assert_called_once()
            assert resultado == [["col1", "col2"], ["valor1", "valor2"], ["valor3", "valor4"]]

def test_leer_archivo(gestor_archivos):
    # Probamos que el método leer_archivo lea todas las líneas correctamente
    contenido = "línea1\nlínea2\nlínea3\n"
    with patch("builtins.open", mock_open(read_data=contenido)) as mock_file:
        resultado = gestor_archivos.leer_archivo()
        mock_file.assert_called_once_with("archivo_test.txt", "r")
        assert resultado == ["línea1\n", "línea2\n", "línea3\n"]

def test_escribir_archivo(gestor_archivos):
    # Probamos que el método escribir_archivo escriba correctamente una línea
    linea = "Nueva línea"
    with patch("builtins.open", mock_open()) as mock_file:
        gestor_archivos.escribir_archivo(linea)
        mock_file.assert_called_once_with("archivo_test.txt", "a")
        mock_file().write.assert_called_once_with(linea + "\n")
