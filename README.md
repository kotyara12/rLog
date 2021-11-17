# rLog: библиотека для вывода отладочных сообщений с возможностью отключения

Данная библиотека представляет собой _набор макросов_ для вывода _форматированных_ отладочных сообщений в COM-порт (с использованием стандартной функции printf), _с возможностью частичного или полного их исключения из кода программы при необходимости_. Создана на основе библиотеки [<esp32-hal-log.h> для Framework Arduino ESP32](https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/esp32-hal-log.h), с целью обеспечения работоспособности на различных микроконтроллерах и инструментах разработки. Протестировано на AVR, ESP32, ESP8266 для PlatformIO и Arduino IDE. Библиотека _не использует объекты типа String_, вся работа основана на динамическом выделении памяти под строки с помощью стандартных функций, что экономит память.

## Поддерживаниваемые платформы
Библиотека протестирована на **VSCode + PlatformIO** и **Arduino IDE** для микроконтроллеров AVR, ESP32 и ESP8266. Предполагается, что список микроконтроллеров может больше. Библиотека не зависит от используемого Framework-а: например это может быть Arduino или Espressif32 (ESP-IDF).

## Возможности
* Поддержка [форматированных сообщений](https://docs.microsoft.com/ru-ru/cpp/c-runtime-library/format-specification-syntax-printf-and-wprintf-functions?view=msvc-160)
* Автоматический вывод отметки времени, типа сообщения и идентификатора задачи при выводе отладочных сообщений (см. "Формат вывода" ниже). Вам больше не нужно заботится об этом.
* Имеется возможность вывода имени файла и номера строки, из которого была вызвана команда.
* Возможность выбора уровня отладки при компиляции: NONE, ERROR, WARNING, INFO, DEBUG, VERBOSE. Все сообщения, которые выше заданного уровня, будут игнорированы компилятором и не попадут в финальный код
* Поддержка цветовых маркеров при выводе. _Примечание: но эта функция работает не во всех инструментах разработки и не на всех операционных системах (например для PlatformIO под Windows7 мне не удалось заставить это работать, а под Windows 10 работает прекрасно), это не зависит от библиотеки_

## Формат вывода
При настройках "по умолчанию" Вы увидете следующий текст:

**23:59:59 [E] TAG: Message**

где:

* 23:59:59 - Отметка времени
* [E] - Тип сообщения, в данном примере это "ошибка"
* TAG - Ярлык (метка), с помощью которой можно определить, какой модуль вызывал данное сообщение
* Message - Собственно текст сообщения

## Использование
Вывод отладочных сообщений в последовательный порт осуществляется с помощью функций-макросов:

```
rlog_v(tag, format, ...) - вывод сообщения уровня VERBOSE
rlog_d(tag, format, ...) - вывод сообщения уровня DEBUG
rlog_i(tag, format, ...) - вывод сообщения уровня INFORMATION
rlog_w(tag, format, ...) - вывод сообщения уровня WARNING
rlog_e(tag, format, ...) - вывод сообщения уровня ERROR
```

где 

* tag - это текстовая метка для обозначения модуля, откуда была вызвана функция
* format - строка сообщения или формата. Но не следует использовать здесь _строковые переменные напрямую_. Например если Вам требуется вывести имя пользователя из переменной user_name, то вместо `rlog_i(logTAG, user_name)` необходимо использовать конструкцию вида `rlog_i(logTAG, "%s", user_name)`
* ... - данные для вывода в спецификации преобразования, _не обязательно_ (см. https://docs.microsoft.com/ru-ru/cpp/c-runtime-library/format-specification-syntax-printf-and-wprintf-functions)

Например:

```
rlog_e("WiFi", "Не удалось подключиться к точке доступа %s, код ошибки %d", ap_name, err);
```

Будет выведен следующий текст:
```
12:34:10 [E] WiFi: Не удалось подключиться к точке доступа test, код ошибки -201
```

При этом, если "уровень" функции сообщения (например rlog_v) будет выше заданного в CONFIG_RLOG_PROJECT_LEVEL, то предпроцессор просто исключит этот вызов при компиляции, сокращая затраты памяти.

## Настройки

Перед использованием библиотеки как минимум выберите уровень отладочных сообщений проекта с помощью определения макроса

```
#define CONFIG_RLOG_PROJECT_LEVEL RLOG_LEVEL_XXXX
```

где RLOG_LEVEL_XXXX это одно из следующих значений (см. rLog.h):

```
RLOG_LEVEL_NONE       (0)    /* Нет вывода */
RLOG_LEVEL_ERROR      (1)    /* Критические ошибки, программный модуль не может восстановиться самостоятельно */
RLOG_LEVEL_WARN       (2)    /* Состояния ошибки, из которых были приняты меры по устранению */
RLOG_LEVEL_INFO       (3)    /* Информационные сообщения, описывающие нормальный ход событий */
RLOG_LEVEL_DEBUG      (4)    /* Дополнительная информация, которая не требуется для нормального использования (значения, указатели, размеры и т.д.). */
RLOG_LEVEL_VERBOSE    (5)    /* Большие фрагменты отладочной информации или частые сообщения, которые потенциально могут переполнить вывод. */
```

Сделать это можно с помощью файла platformio.ini, sdkconfig.h или project_config.h (как в примерах - см. ниже).

----------------------

Если Вы хотите использовать файл глобальных определений макросов project_config.h (который будет доступен в любой библиотеке проекта PlatformIO),
то поместите его в папку SRC проекта (там же, где и главный файл проекта) 
и добавьте к конфигурационный файл проекта platformio.ini следущие определения:

[env]
build_flags = -Isrc 
	
Этот флаг заставит компилятор при предварительной сборке библиотек просматривать каталог проекта

----------------------


----------------------

## Примеры использования:
* arduinoide-arduino
* platformio-avr-arduino
* platformio-esp32-arduino
* platformio-esp32-espidf
* platformio-esp8266-arduino

## Зависимости
Библиотека зависит только от "стандартных" библиотек:
* stdint.h
* stdio.h
* stdarg.h
* stdlib.h
* time.h

## Примечание
Данные замечания относятся к моим библиотекам, размещенным на ресурсе https://github.com/kotyara12?tab=repositories.

- библиотеки, название которых начинается с префикса **re**, предназначены только для ESP32 и ESP-IDF (FreeRTOS)
- библиотеки, название которых начинается с префикса **ra**, предназначены только для ARDUINO
- библиотеки, название которых начинается с префикса **r**, могут быть использованы и для ARDUINO, и для ESP-IDF

Так как я в настроящее время разрабатываю программы в основном для ESP-IDF, основная часть моих библиотек предназначена только для этого фреймворка. Но Вы можете портировать их для другой системы, взяв за основу.
