#ifndef DEBUG_UTILS_H

/**
 * When using serial prints over the USB Serial
 * an open USB Serial terminal/ monitor is required.
 * To run ESP32 w/o open connection set this flag to 0:
[env]
build_flags =
  '-D ARDUINO_USB_CDC_ON_BOOT=0'
 *
 * activate serial debug in platform.ini if needed:
[env]
build_flags =
  '-D SERIAL_DEBUG'
*/

#ifdef SERIAL_DEBUG
#define serialBegin(x) Serial.begin(x)
#define serialPrint(x) Serial.print(x)
#define serialPrintln(x) Serial.println(x)
#define serialFlush() Serial.flush()
#else
#define serialBegin(x)
#define serialPrint(x)
#define serialPrintln(x)
#define serialFlush()
#endif

#endif
