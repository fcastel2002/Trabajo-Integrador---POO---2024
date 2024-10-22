import json

with open("servidor\\usuarios.json", "r") as archivo:
    print(json.load(archivo))