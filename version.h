#define OS_VERSION 0x320A
#define BOARD_VER  0x0304   // v3-4M
/*
CHANGELOG ------DD/MM/YYYY---------------------------------------------------------------------------------------------------------------

v.2.0 (beta10)   09/06/2021 (build 0x320A - froggo)
      O..O
     (-~~-)   (\/)
    ( >__< )  (..)
    ^^ ~~ ^^ C(")(")

- complete firmware rebuild
- step-up - upgrade to (semi) Operating System (separating core & additional modules)
- meet Froggo - mascot for this version & his little bunny friend

v.2.0 (beta8)   07/06/2021
- added boot codes (user led is flashing)
    - only *_______*_*_ code is boot ok
    - code *_______****_*_ is WiFi connection problem
- added LED matrix support (now only 16 colors defined by subtopic .../map)

v.2.0 (beta7)   06/05/2021
- added webgui downloading software (hold user button unless long led light ~ 1s)
- added fail-safe webgui and autoreset functions (address /reset)

v.2.0 (beta6)   06/05/2021
- reworked UUID handling
- added GUI for UUID setting

v.2.0 (beta5)   06/05/2021
- added GUI for periphreal boards setting
- added flash allocation scheme

v.2.0 (beta4)   27/04/2021
- added SPIFFS WEBGUI for WiFi setting

v.2.0 (beta3)   26/04/2021
- added mDNS support (192.168.4.1/bluf -> espcraft.local/bluf)
- bugfix (IP in AP mode shows 0.0.0.0)

v.2.0 (beta2)   26/04/2021
- added auto AP (switch to Acces Point if WiFi is not accesible), SSID is ESPCraft(uuid), PASS is espcraft

v.2.0 (beta1)   26/04/2021
- new OTA (Arduino OTA changed to Async Elegant OTA lib. - HTTP OTA)

v.1.0 (release) 12/04/2021
- debug & settings via UART
- supported peripreals - pushbutton, potentiometer & leds
- support for OTA (Over The Air) programming via Arduino OTA

*/