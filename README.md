# Azure Sphere micropython

**How To**
* NEED installed Azure Sphere SDK ( Windows 10 only )
* [Python 3](https://www.python.org/)
* MAKE ... [I use MinGW](http://www.mingw.org/)
* Download [micropython](https://github.com/micropython/micropython)

* Move this folder **azure-minimal** to micropython/ports
* Open in folder **azure-minimal**
* Open **Makefile** and edit your paths [CROSS_DIR, SYSROOT, PYTHON]
* Execute **make**
* Run PACK_IMAGE.BAT (edit path to **azsphere**)
* Run UPLOAD.BAT (edit path to **azsphere**)
