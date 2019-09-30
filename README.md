# Azure Sphere micropython

This is a simple example how to use micropython in Azure Sphere ( may be bugs yet )

Used Board: [Azure Sphere MT3620 Starter AES-MS-MT3620-SK-G by Avnet](https://www.avnet.com/shop/us/products/avnet-engineering-services/aes-ms-mt3620-sk-g-3074457345636825680/)

```python
import utime as time
import Pin 

print(' <py> Azure Sphere 2019 Hello World')
for i in range(5):
    print(' <py> i =', i)

pin_8 = Pin.output(8, 0, 1)
for i in range(20):
    Pin.set(pin_8, 1)
    time.sleep(0.1)
    Pin.set(pin_8, 0)
    time.sleep(0.1)    
Pin.close(pin_8)
```

![image](https://raw.githubusercontent.com/Wiz-IO/azure-sphere-micropython/master/azure-sphere-micropython.jpg) 

**How To**
* Need installed [Azure Sphere SDK](https://docs.microsoft.com/en-us/azure-sphere/install/install)
* [Python 3](https://www.python.org/)
* MAKE ... [I use from MinGW](http://www.mingw.org/)
* Download [micropython](https://github.com/micropython/micropython)
* Move this folder **azure-minimal** to micropython/ports
* Open folder **azure-minimal**
* Open **Makefile** and edit your paths [ CROSS_DIR, SYSROOT, PYTHON ]
* Execute **make**
* Run PACK_IMAGE.BAT (edit path to **azsphere**)
* Run UPLOAD.BAT (edit path to **azsphere**)


**Notes**
* Log_Debug() is [redirected](https://github.com/Wiz-IO/azure-sphere-micropython/blob/master/azure-minimal/az_uart.c#L23) to real uart AVNET_AESMS_ISU0_UART ( 4 )
* main.py is executed script ( packed to image )
* if source is compiled ( make ), edit only main.py >>> pack_image >>> upload
* app.image is precompiled example can be uploaded for test

