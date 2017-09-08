#ifndef LiquidCrystal_I2Cyr_h
#define LiquidCrystal_I2Cyr_h

#include <LiquidCrystal_I2C.h>

//Указать под свой дисплей в данном случае указано для 1602
//Число символов в строке на экране LCD
#define LCD_COLS          16
//Число строк на экране LCD
#define LCD_ROWS          2

////////////////////////////////////////////////////////////////
//Число байт на 1 символ шрифта
#define BYTES_PER_SYMBOL  8
//Число пользовательских символов (зависит от самого дисплея)
#define LCD_USER_CHARS    8

class LiquidCrystal_I2Cyr : public LiquidCrystal_I2C {
  uint8_t lcd_buf[LCD_ROWS][LCD_COLS];
  uint8_t _flag_refresh;
  uint8_t _x, _y;
public:
   // Constructor with backlight control
   LiquidCrystal_I2Cyr(uint8_t lcd_Addr, uint8_t En, uint8_t Rw, uint8_t Rs, 
                     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                     uint8_t backlighPin, t_backlighPol pol = POSITIVE);
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS );
  void clear();
  void home();
  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t, uint8_t); 
#if defined(ARDUINO) && ARDUINO >= 100
  virtual size_t write(uint8_t);
#else
  virtual void write(uint8_t);
#endif
  void show();
  void clear_buf();
  void print_u(const char *s);
};

#endif
