print(' <py> Azure Sphere 2019 curl')

import curl as c

def send(host, message):
    c.init()
    c.setopt(c.CURLOPT_PORT, 80)
    c.setopt(c.CURLOPT_URL, host)
    c.setopt(c.CURLOPT_POSTFIELDS, message)
    c.setopt(c.CURLOPT_USERAGENT, "AzureSphere")
    c.setopt(c.CURLOPT_CONNECTTIMEOUT, 20)
    print( c.perform() )
    c.end()
    
print(' <py> begin')    
send("wizio.eu/iot.php", "hello") 
print(' <py> end')
