print(' <py> Azure Sphere 2019 micropython')

import utime as time
import usocket as socket

def get(host):
    print()
    s = socket.socket()
    ai = socket.getaddrinfo(host, 80)
    addr = ai[0][-1]
    print(" <py> Connecting to", host)
    s.connect(addr)
    print(" <py> Sending")
    s.send("GET /iot.php HTTP/1.0\r\nHost:" + host + "\r\n\r\n")
    print(" <py> Receive")
    print(s.recv(4096))
    s.close()
    print(" <py> Closed")
    
get("wizio.eu") # AllowedConnections
