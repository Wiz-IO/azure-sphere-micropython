print(' <py> Azure Sphere 2019 I2C')

from dev import I2C

c = I2C(2) # ISUx_I2C

c.open()    

print(' <py> writed', c.write(b'\x99') )
print(' <py> read', c.read(4))

print(' <py> write-read', c.writeRead(b'\x8D', 6))

c.close()
print(' <py> End')