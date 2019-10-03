print(' <py> Azure Sphere 2019 LED EXAMPLE')
import utime as time
from dev import LED
print(' <py> Led begin')

red = LED(8) # AVNET_AESMS_PIN11_GPIO8
for i in range(10):
    red.toggle()
    time.sleep(0.1)
red.off()

green = LED(LED.GREEN) # LED.GREEN = 9
for i in range(10):
    green.toggle()
    time.sleep(0.2) 
green.off()       

blue = LED(LED.BLUE) # LED.BLUE = 10
blue.on()
time.sleep(1.0) 
blue.off()

print(' <py> Led end')