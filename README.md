# ESPCraft-2.0
New ESP Craft firmware project written in Wiring (e.g. Arduino C++) for our ESP32-based boards. 
if you want to update firmware of your board, download the compiled binaries from binaries folder and upload them to your board at espcraft.local/update

# News in release 2.0-beta8 (07/06/2021)
- added boot codes (user led is flashing)
    - only *_______*_*_ code is boot ok
    - code *_______****_*_ is WiFi connection problem
- added LED matrix support (now only 16 colors defined by subtopic .../map)