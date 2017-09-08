# LiquidCrystal_I2Cyr

Библиотека предназначена для работы с тексовыми LCD дисплеями на базе совместимого контроллера HD44780, 
подключаемого в МК по I2C, через переходник на базе PFC8574, в которых нет поддержки русских символов ВООБЩЕ.
В основе данной библиотеки NewLiquidCrystal, последнюю версию
которой можно скачать https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads/.

Библиотека предполагает, что русские символы, выводимые на экран при помощи метода print
находятся в кодировке указанной в константе LCD_CHARSET (по умолчанию windows-1251/cp1251).
Это несколько неудобно, т.к. в среде Arduino IDE работа с файлами идет исключительно в 
кодировке UTF-8. Но при желании, можно использовать перекодировку из UTF-8 в LCD_CHARSET, 
в том числе и налету, если это позволяют ресурсы МК использовать метод print_u.

Да, переходников PFC8574 существует как минимум 3 разных варианта, об этом стоит почитать 
http://arduino-info.wikispaces.com/LCD-Blue-I2C

Arduino module LiquidCrystal_I2Cyr with Cyrillic symbols

WARNING! TOP SECRET! THIS IS VERY ULTRA HIGH TOP SECRET GALACTIC SCALE WORK! ALL WHO READ THIS TEXT IS DEAD!
USE IT ON YOUR OWN RISK!
