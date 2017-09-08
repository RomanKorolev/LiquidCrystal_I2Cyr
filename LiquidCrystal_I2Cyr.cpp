#include "LiquidCrystal_I2Cyr.h"

//КОНСТАНТЫ В PROGMEM Кодировка выводимых на экран строк должна быть CP866, т.к. коды ASCII коды 0x80 .. 0x9F
const unsigned char LCDKirillicRecodingMap [] PROGMEM= {//таблица перекодировки кириллических символов	
	0x41, 0x00, 0x42, 0x01, 0x02, 0x45, 0x03, 0x04,				//АБВГДЕЖЗ
	0x05, 0x06, 0x4b, 0x07, 0x4d, 0x48, 0x4f, 0x08,				//ИЙКЛМНОП
	0x50, 0x43, 0x54, 0x09, 0x0a, 0x58, 0x0b, 0x0c,				//РСТУФХЦЧ
	0x0d, 0x0e, 0x0f, 0x10, 0x62, 0x11, 0x12, 0x13				//ШЩЪЫЬЭЮЯ
};

const unsigned char LCDKirillicFont [] PROGMEM= {//битовый массив кириллических символов
	0b11111,0b10001,0b10000,0b11110,0b10001,0b10001,0b11110,0b00000,	//Б (0)
	0b11111,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b00000,	//Г (1)
	0b01111,0b00101,0b00101,0b01001,0b10001,0b11111,0b10001,0b10001,	//Д (2)
	0b10101,0b10101,0b10101,0b01110,0b10101,0b10101,0b10101,0b00000,	//Ж (3)
	0b11110,0b00001,0b00001,0b00110,0b00001,0b00001,0b11110,0b00000,	//З (4)
	0b10001,0b10001,0b10011,0b10101,0b11001,0b10001,0b10001,0b00000,	//И (5)
	0b00100,0b10101,0b10001,0b10011,0b10101,0b11001,0b10001,0b00000,	//Й (6)
	0b01111,0b00101,0b00101,0b00101,0b00101,0b10101,0b01001,0b00000,	//Л (7)
	0b11111,0b10001,0b10001,0b10001,0b10001,0b10001,0b10001,0b00000,	//П (8)
	0b10001,0b10001,0b10001,0b01010,0b00100,0b01000,0b10000,0b00000,	//У (9)
	0b01110,0b10101,0b10101,0b10101,0b01110,0b00100,0b00100,0b00000,	//Ф (10)
	0b10001,0b10001,0b10001,0b10001,0b10001,0b10001,0b11111,0b00001,	//Ц (11)
	0b10001,0b10001,0b10001,0b01111,0b00001,0b00001,0b00001,0b00000,	//Ч (12)
	0b10101,0b10101,0b10101,0b10101,0b10101,0b10101,0b11111,0b00000,	//Ш (13)
	0b10101,0b10101,0b10101,0b10101,0b10101,0b10101,0b11111,0b00001,	//Щ (14)
	0b11000,0b01000,0b01000,0b01110,0b01001,0b01001,0b01110,0b00000,	//Ъ (15)
	0b10001,0b10001,0b10001,0b11001,0b10101,0b10101,0b11001,0b00000,	//Ы (16)
	0b01110,0b10001,0b00101,0b01011,0b00001,0b10001,0b01110,0b00000,	//Э (17)
	0b10010,0b10101,0b10101,0b11101,0b10101,0b10101,0b10010,0b00000,	//Ю (18)
	0b01111,0b10001,0b10001,0b01111,0b00101,0b01001,0b10001,0b00000		//Я (19)
};

LiquidCrystal_I2Cyr::LiquidCrystal_I2Cyr(uint8_t lcd_Addr, uint8_t En, uint8_t Rw,
                                     uint8_t Rs, uint8_t d4, uint8_t d5,
                                     uint8_t d6, uint8_t d7, uint8_t backlighPin, 
                                     t_backlighPol pol) : LiquidCrystal_I2C(lcd_Addr, En, Rw, Rs, d4, d5, d6, d7, backlighPin, pol)
{
  clear();
}

#if defined(ARDUINO) && ARDUINO >= 100
inline size_t LiquidCrystal_I2Cyr::write(uint8_t ch) {
#else
inline void LiquidCrystal_I2Cyr::write(uint8_t ch) {
#endif
//Пишем во внутренний в буфер вместо LCD
  if(lcd_buf[_y][_x] != ch){
    _flag_refresh = 1;
    lcd_buf[_y][_x] = ch;
  }
  _x++;
  if(_x >=  _cols){
    if(_y < _numlines - 1){
      _y++;
      _x = 0;
    }else{
      _x = _cols - 1;
    }
  }
#if defined(ARDUINO) && ARDUINO >= 100
  return 1;
#endif
}

void LiquidCrystal_I2Cyr::begin(uint8_t cols, uint8_t rows, uint8_t dotsize) {
	LiquidCrystal_I2C::begin(cols, rows, dotsize);
	_numlines = (rows < LCD_ROWS) ? rows : LCD_ROWS;
	_cols = (cols < LCD_COLS) ? cols : LCD_COLS;
	clear();
}

void LiquidCrystal_I2Cyr::clear(){
  LiquidCrystal_I2C::clear();
  clear_buf();
}

//Переопределяем метод установки курсора
void LiquidCrystal_I2Cyr::setCursor(uint8_t x, uint8_t y){
  if(y < _numlines){
    _y = y;
  }
  if(x < _cols){
    _x = x;
  }
}

//Метод показывает текст, который был подготовлен с помощью методов print
void LiquidCrystal_I2Cyr::show(){
  //Если нет никаких изменений, то ничего не выводим на экран
  if(!_flag_refresh){
    return;
  }
  _flag_refresh = 0;
  uint8_t loaded_map[LCD_USER_CHARS];
  uint8_t loaded_chars_cnt = 0;
  //Загружаем текст из буфера в памяти в LCD
  for(uint8_t y = 0; y < _numlines; y++){
    LiquidCrystal_I2C::setCursor(0, y);
    for(uint8_t x = 0; x < _cols; x++){
      uint8_t ch = lcd_buf[y][x];
      if(ch >= 0x80 && ch < 0xA0){
        uint8_t found = 0;
        for(uint8_t i = 0; i < loaded_chars_cnt; i++){
          if(ch == loaded_map[i]){
            found = 1;
            ch = i;
            break;
          }
        }
        //Если символ еще не находится в массиве загружаемых пользователем
        if(!found){
          PGM_P p = reinterpret_cast<PGM_P>(LCDKirillicRecodingMap + ch - 0x80);
          ch = pgm_read_byte(p);
          if(ch <= 0x13){
            if(loaded_chars_cnt >= LCD_USER_CHARS){
              ch = '?'; //Если все доступные 8 пользовательских символов уже использованы
            }else{
              loaded_map[loaded_chars_cnt] = ch;
              ch = loaded_chars_cnt++;
            }
          }
        }
      }
      //Реально пишем символ в LCD
      LiquidCrystal_I2C::write(ch);
    }
  }

  // Загружаем шрифты
  // К сожалению метод createChar, вызывает LiquidCrystal_I2C::write
  // поэтому пришлось колхозить на низком уровне, загружаем сразу все символы
  //command(LCD_SETCGRAMADDR); // <= is inaccessinble (private LCD::command)
  send(LCD_SETCGRAMADDR, COMMAND);
  for(uint8_t k = 0; k < loaded_chars_cnt; k++){
    PGM_P p = reinterpret_cast<PGM_P>(LCDKirillicFont + loaded_map[k] * BYTES_PER_SYMBOL);
    for(uint8_t i = 0; i < BYTES_PER_SYMBOL; i++){
      LiquidCrystal_I2C::write(pgm_read_byte(p++));
    }
  }
  //Авто перевод курсора, хотя возможно это лишнее
  setCursor(0, 0);
}


void LiquidCrystal_I2Cyr::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  send(LCD_SETCGRAMADDR | (location << 3), COMMAND);
  for (int i=0; i<8; i++) {
    LiquidCrystal_I2C::write(charmap[i]);
  }
  send(LCD_SETDDRAMADDR, COMMAND);  //Костыль чтобы выйти из этого режима
}


void LiquidCrystal_I2Cyr::clear_buf(){
  _x = _y = 0;
  for(uint8_t y = 0; y < _numlines; y++){
    for(uint8_t x = 0; x < _cols; x++){
      lcd_buf[y][x] = ' ';
    }
  }
  _flag_refresh = 1;
}

void LiquidCrystal_I2Cyr::print_u(const char *s){
  unsigned char ch;
  for(uint8_t i = 0; s[i];){
    ch = s[i++];
    switch (ch){
      case 0xD0:
        ch = s[i++];
        if (ch == 0x81) { ch = 0xA8; break; }
        if (ch >= 0x90 && ch <= 0xBF) ch += 0x30;
        break;
      case 0xD1:
        ch = s[i++];
        if (ch == 0x91) { ch = 0xB8; break; }
        if (ch >= 0x80 && ch <= 0x8F) ch += 0x70;
        break;
    }
    write(ch); //Пишем во внутренний буфер (в память МК lcd_buf)
  }
}

