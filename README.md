# rLog

A universal approach to displaying debug messages on various platforms and frameworks. 
With the ability to completely exclude debug messages from the code. 
Analogue of <esp32-hal-log.h> for ESP32, but with the ability to work on AVR or ESP8266. 
Tested on AVR, ESP32, ESP8266 for PlatformIO and Arduino IDE.

----------------------

Before using the library, set the debug level of the project using a macro definition

#define CONFIG_RLOG_PROJECT_LEVEL RLOG_LEVEL_XXXX

where RLOG_LEVEL_XXXX is one of the following values (see rLog.h):

- RLOG_LEVEL_NONE       (0)    /* No log output */
- RLOG_LEVEL_ERROR      (1)    /* Critical errors, software module can not recover on its own */
- RLOG_LEVEL_WARN       (2)    /* Error conditions from which recovery measures have been taken */
- RLOG_LEVEL_INFO       (3)    /* Information messages which describe normal flow of events */
- RLOG_LEVEL_DEBUG      (4)    /* Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
- RLOG_LEVEL_VERBOSE    (5)    /* Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */

This can be done using the platformio.ini file, sdkconfig.h or project_config.h (as in the examples).

----------------------

If you want to use the global macro definition file project_config.h (which will be available in any PlatformIO project library),
then place it in the SRC folder of the project (in the same place as the main project file)
and add the following definitions to the platformio.ini project config file:

[env]
build_flags = -Isrc

This flag will force the compiler to look through the project directory when pre-building libraries

----------------------

The output of debug messages to the serial port is carried out using the functions - macros:

- rlog_v(tag, format, ...)
- rlog_d(tag, format, ...)
- rlog_i(tag, format, ...)
- rlog_w(tag, format, ...)
- rlog_e(tag, format, ...)

where tag is just a text label to identify the module from where the function was called.

If the level of the message function (for example, rlog_v) is higher than that specified in CONFIG_RLOG_PROJECT_LEVEL, then the preprocessor will simply exclude this call during compilation, reducing memory costs.

----------------------

See examples:
- arduinoide-arduino
- platformio-avr-arduino
- platformio-esp32-arduino
- platformio-esp32-espidf
- platformio-esp8266-arduino
