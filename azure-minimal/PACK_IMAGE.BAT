del app.image

copy build\app.elf approot\bin
copy app_manifest.json approot
copy main.py approot

C:/Users/HP/.platformio/packages/tool-azure/azsphere image package-application --input approot --output "app.image" --sysroot 2+Beta1905 --verbose --hardwaredefinition "avnet_aesms_mt3620.json"

pause