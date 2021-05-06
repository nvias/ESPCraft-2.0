//Internal 4-kB flash allocation scheme
/*
ADDR (DEC)  ._______________._______________._______________._______________.
0000-0511   |   WIFI-SSID   |   WIFI-PASS   |BOARD-SETTINGS |X X X X X X X X|
0512-1023   |X X X X X X X X|X X X X X X X X|X X X X X X X X|X X X X X X X X|
1024-1535   |X X X X X X X X|X X X X X X X X|X X X X X X X X|X X X X X X X X|
1536-2047   |X X X X X X X X|X X X X X X X X|X X X X X X X X|X X X X X X X X|
            |---------------+---------------+---------------+---------------|
2048-2559   |X X X X X X X X|X X X X X X X X|X X X X X X X X|X X X X X X X X|
2560-3071   |X X X X X X X X|X X X X X X X X|X X X X X X X X|X X X X X X X X|
3072-3583   |X X X X X X X X|X X X X X X X X|X X X X X X X X|X X X X X X X X|
3584-4095   |X X X X X X X X|X X X X X X X X|X X X X X X X X|X X X X X X X X|


BOARD        0 1 2 3 4 5 6 7 8 9 A B C D E F 0 1 2 3 4 5 6 7 8 9 A B C D E F
SETTINGS    ._______________._______________._______________._______________.
000-031     |DTYPE BOARDUUID|X X X X X X X X|X X X X X X X X|X X X X X X X X|
032-063     |X X X X X X X X|X X X X X X X X|X X X X X X X X|X X X X X X X X|
064-095     |X X X X X X X X|X X X X X X X X|X X X X X X X X|X X X X X X X X|
096-127     |X X X X X X X X|X X X X X X X X|X X X X X X X X|X X X X X X X X|

*/
// BASE ADDRESS values
#define SSID_ADDR   0                                   // 128 bytes for SSID
#define PASS_ADDR   128                                 // 128 bytes for PASS
#define SET_ADDR    256                                 // board settings base address