<hr/>
<p align="center">
<img src="https://img.shields.io/github/last-commit/nvias/ESPCraft-2.0.svg?style=for-the-badge" />
&nbsp;
<img src="https://img.shields.io/travis/com/nvias/ESPCraft-2.0.svg/master?style=for-the-badge" />
&nbsp;
<img src="https://img.shields.io/github/license/nvias/ESPCraft-2.0.svg?style=for-the-badge" />
&nbsp;
<img src="https://img.shields.io/github/v/tag/nvias/ESPCraft-2.0?label=LATEST&style=for-the-badge" />
&nbsp;
</p>
<hr/>

# ESPCraft-2.0 (Work in progress, please use ver. 2.0-beta8 from releases)
New ESP Craft firmware project written in Wiring (e.g. Arduino C++) for our ESP32-based boards. 
if you want to update firmware of your board, download the compiled binaries from binaries folder and upload them to your board at espcraft.local/update

# News in release 2.0-beta10 (07/06/2021) (build 0x320A)
- full-fat rework of entire firmware
- boot codes rework
- cold boot addition (alias first setup of new/ereased board)
- new system for work with flash (eeprom) (not backwards compatible with older mode bcs of new memory map!)
- new WEBGUI on the go...
- multitasking...

# Naming scheme
- Every OS build has 2-byte version number saved in board memory
- Version number is separated into 4 4-bit sections
- 0x O 2 0 A
   | | | | |
   | | | | OS build
   | | | OS minor version number
   | | OS major version number
   | OS type - |0-normal|1-customized|2-experimental|3-beta|
   (HEXADECIMAL)
