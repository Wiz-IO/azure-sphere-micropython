import utime as time
import Pin 

print(' <py> microPython (from file)')
print(' <py> Azure Sphere 2019 Hello World')
for i in range(5):
    print(' <py> i =', i)

print(' <py> Pin module test')

pin_8 = Pin.output(8, 0, 1)
print(' <py> PIN_8 FD =', pin_8)
for i in range(20):
    Pin.set(pin_8, 1)
    time.sleep(0.1)
    Pin.set(pin_8, 0)
    time.sleep(0.1) 
Pin.set(pin_8, 1)    
Pin.close(pin_8)

pin_9 = Pin.output(9, 0, 1)
print(' <py> PIN_9 FD =', pin_9)
for i in range(20):
    Pin.set(pin_9, 1)
    time.sleep(0.1)
    Pin.set(pin_9, 0)
    time.sleep(0.1)   
Pin.set(pin_9, 1)       
Pin.close(pin_9) 
