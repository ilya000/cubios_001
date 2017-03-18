// =============================================================================
// ||   Cubios
#define vers      0.4
// ||   ---------------------------
// ||   (c) iLya Os   //   ilya@osipov.ru
// =============================================================================
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <avr/wdt.h>
#include <ioSerial3.h>

//Первый сделанный Серый-куб / первый порт стандарт Serial
//int Port2Screen1[3] {2, 1, 3}; int Port2Screen2[3] {1, 3, 2};
//#define ID 1
 
//Все кроме серого/ Чёрный-куб в включателем / первый порт стандарт Serial
int Port2Screen2[3] {3, 1, 2};
int Port2Screen1[3] {2, 3, 1}; 

int Screen2PortX[3] {3, 1, 2}; 
int Screen2PortY[3] {2, 3, 1}; 

//*
#define ID 5
#define HAVESPRITE 0
int InitInit[3] { INITR_144GREENTABIO, INITR_144GREENTAB, INITR_144GREENTAB };   //5
word MainColor = 0x07FF; //  CYAN
//*/

/*
#define ID 6
#define HAVESPRITE 1
int InitInit[3] { INITR_144GREENTABIO, INITR_144GREENTABIO, INITR_144GREENTABIO }; //6
word MainColor = 0xF81F; // MAGENTA 0xF81F
//*/

/*
#define ID 7
#define HAVESPRITE 0
int InitInit[3] { INITR_144GREENTAB, INITR_144GREENTAB, INITR_144GREENTAB }; //6
word MainColor = 0x001F; //BLUE
//*/

/*
#define ID 8
#define HAVESPRITE 0
int InitInit[3] { INITR_144GREENTAB, INITR_144GREENTAB, INITR_144GREENTAB }; //6
word MainColor = 0xF800; // RED  
//*/

/*
#define ID 1  // Last
#define HAVESPRITE 0
int InitInit[3] { INITR_144GREENTAB, INITR_144GREENTAB, INITR_144GREENTAB }; //6
word MainColor = 0x07E0; // GREEN  
//*/

/*
#define ID 2  
#define HAVESPRITE 0
int InitInit[3] { INITR_144GREENTAB, INITR_144GREENTAB, INITR_144GREENTAB }; //6
word MainColor = 0x7777; // Dark Gr
//*/

/*
#define ID 3  
#define HAVESPRITE 0
int InitInit[3] { INITR_144GREENTAB, INITR_144GREENTAB, INITR_144GREENTAB }; //6
word MainColor = 0xAAAA; // Gr
//*/

/*
#define ID 4  
#define HAVESPRITE 1
int InitInit[3] { INITR_144GREENTAB, INITR_144GREENTAB, INITR_144GREENTAB }; //6
word MainColor = 0xFFE0; // YELLOW
//*/


ioSerial3 bus(ID);

#define OR ||
#define AND &&


/// Ресеты!!!
#define difficultReset() wdt_enable(WDTO_1S); while(1) {}
void(* easyReset) (void) = 0;//declare reset function at address 0

inline uint16_t C565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
inline uint8_t C565R(uint16_t c0) {
return ((c0 & 0xF800) >> 11 << 3);
}
inline uint8_t C565G(uint16_t c0) {
return ((c0 & 0x07E0) >> 5 << 2);
}
inline uint8_t C565B(uint16_t c0) {
return ((c0 & 0x001F) << 3);
}
inline uint8_t Lo(word w) {
  return w & 0xff;
}
inline uint8_t Hi(word w) {
  return w >> 8;
}
inline word SwapWord(word w) {
  return ((w & 0xff) << 8) | (w >> 8);
}

#define LEFT 1
#define RIGHT 2


double old_time, current_time, delta;
#define TFTW            128 //128     // screen width
#define TFTH            128 //160     // screen height
#define GAP              20 // - должен быть больше спрайта
#define TFTW2            64     // half screen width
#define TFTH2            64     // half screen height

// Color definitions
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//static Adafruit_ST7735 d3 = Adafruit_ST7735(4,  A0, 3);
//static Adafruit_ST7735 d2 = Adafruit_ST7735(2 , A0, A1);
//static Adafruit_ST7735 d1 = Adafruit_ST7735(6,  A0, 5 );

#define DE(n) de[n-1]
static Adafruit_ST7735 de[3] = {Adafruit_ST7735(6,  A0, 5), Adafruit_ST7735(2 , A0, A1), Adafruit_ST7735(4,  A0, 3 )};

#define cc1 C565(0xFF,0x3B,0x30) //0xF32   //#FF3B30  1
#define cc2 C565(0xFF,0x95,0x00) //  2
#define cc3 C565(0xFF,0xCC,0x00) //  3
#define cc4 C565(0x4C,0xD9,0x64) //  4
#define cc5 C565(0x34,0xAA,0xDC) //   //#34AADC  5
#define cc6 C565(0x00,0x7A,0xFF) //  6
#define cc7 C565(0x58,0x56,0xD6) //  7
#define cc8 C565(0xFF,0x2D,0x55) //  8
#define cc9 C565(0x1F,0x1F,0x21) //  9
#define cc10 C565(0xF7,0xF7,0xF7) //  10
#define cc11 C565(0xFF,0xD3,0xE0) //  11
#define cc12 C565(0xC6,0x43,0xFC)   //#C643FC  12

// bird sprite
// bird sprite colors 
#define C0 C565(0,0,0)
#define C1 C565(195,165,75)
#define C2 C565(255,254,174)
#define C3 ST7735_WHITE
#define C4 ST7735_RED
#define C5 C565(251,216,114)
#define ii C565(000,000,000)
#define WW C565(255,255,255)
#define RR C565(255,50,50)
#define BB C565(50,50,255)

//const int  DispOfs = 20, SSize = 16;
//#define BIRDW             16     // bird width
//#define BIRDH             16     // bird height

const uint8_t GifDIG1[] PROGMEM =
{ 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x00, 0x00, 0xfa, 0x5f
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x00, 0x00
  , 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x05, 0xa0
  , 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x05, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00, 0x05, 0xa0, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x5f
};

const uint8_t GifDIG2[] PROGMEM =
{ 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x00, 0x00, 0xfa, 0x5f
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x00, 0x00
  , 0xfa, 0x5f, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x05, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0
  , 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x05, 0xa0
  , 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00
  , 0xfa, 0x5f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00
};

const uint8_t GifDIG3[] PROGMEM =
{ 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x00, 0x00, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x00, 0x00, 0xfa, 0x5f
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x00, 0x00
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x05, 0xa0
  , 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x05, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x05, 0xa0, 0x05, 0xa0, 0x05, 0xa0, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00
  , 0xfa, 0x5f, 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00, 0x05, 0xa0, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40
  , 0xfa, 0x5f, 0x00, 0x00, 0xa8, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40
  , 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x40, 0x00, 0x00, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00
  , 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0xaa, 0x40, 0xaa, 0x40, 0x00, 0x00, 0xfa, 0x5f, 0xfa, 0x5f, 0xfa, 0x5f, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x5f
};

#define DigSizeX 16
#define DigSizeY 15

#define SpritLimit 6
int SpritS = 0;

struct TSprite
{
  word id;
  byte scr;
  int DigStep;
  uint8_t addX;
  uint8_t startx;
  bool hidden;
  float X;
  float oldX;
  float incX;
  float Y;
  float oldY;
  float incY;
  int waitConform; // 0 если не отправляли, 1 и больше после попытки отправки
  byte waitN;
  //0 - летим, 1 попытка отдать (сформирован вектор), 2 попытка провалилась ждем ного отскока
};
struct TSprite Sprite[SpritLimit];


//byte n = 50; //n = 1 2 3 - направление
byte DirStatus;

struct Vector DataIn, DataOut;

int Doors[3] { 1, 1, 1 }; // 1 закрытие / -1 открытие / 0 стоит
int DoorX[3] { 0, 0, 0 }; // текущий шаг открытия/закрытия

int Colors1[8][8] {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};


