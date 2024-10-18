import xmlrpc.client

cliente = xmlrpc.client.ServerProxy('http://localhost:9000')

try: 
    respuesta = "hola"
    print("Respuesta:" + respuesta)

except Exception as e:
    print("Error: " + str(e))
    