

void Mig() {
 // DE(1).invertDisplay(true); DE(2).invertDisplay(true); DE(3).invertDisplay(true);
 // delay(10);
 // DE(1).invertDisplay(false); DE(2).invertDisplay(false); DE(3).invertDisplay(false);
}

void DrawHideSprite(struct TSprite &S) {

  //uint8_t n = 1;
  //int korX = 0; //  корректировка
  int x1 = int(S.X);
  int y1 = int(S.Y);
  int oldX = int (S.oldX);
  int oldY = int (S.oldY);

  DE(S.scr).fillRect( oldX, oldY, DigSizeX * 2, DigSizeY * 2, MainColor);
};

void DrawSprite(struct TSprite &S) {
  word a = 0;
  uint8_t  d;
  byte n = S.scr;
  //int korX = 0,
  //int limX, x2; //  корректировка
  int x1 = int(S.X);
  int y1 = int(S.Y);
  int oldX = int (S.oldX);
  int oldY = int (S.oldY);
  int k = 2;

  if (S.DigStep < 39) {
    S.DigStep++;
  } else {
    S.DigStep = 0;
  };
  d = int (S.DigStep / 10);
  if (d == 3) d = 1;

  if (S.hidden == true) {  // если скрыт, то стерли и ушли.
    //  nfillRect(n, oldX, oldY, DigSizeX, DigSizeY, C0);
    return;
  };


  if (oldX < x1) { //- движ в право
    DE(n).fillRect( oldX, oldY, (x1 - oldX), DigSizeY * k  , MainColor);
  }
  if (oldY < y1) { //-Движ в низ/свеж
    DE(n).fillRect( oldX, oldY, DigSizeX * k, (y1 - oldY)  , MainColor);
  }
  if (x1 < oldX) { //- движение в лево
    DE(n).fillRect( x1 + DigSizeX * k, oldY, (oldX - x1), DigSizeY * k, MainColor);
  };
  if (y1 < oldY) { //-Движ в верх/свеж
    DE(n).fillRect( oldX, y1 + DigSizeY * k, DigSizeX * k, (oldY - y1), MainColor);
  };

  ///- это нужно чтобы не было прооблем у правого края
  /* x2 = x1  + DigSizeX - 1; //x1 + DigSizeX - 1 - korX
    limX = limX;
    if (x2 > TFTW) {
     limX = DigSizeX + TFTW - x2 + 1;
    };
  */


  if (x1 > oldX) { // в право
    S.addX = -1;
    S.startx = DigSizeX * k - 1; //DigSizeX - 1; limX  - 1
  } else if (x1 < oldX) { // в лево
    S.addX = 1;
    S.startx = 0;
  };

  DE(n).setAddrWindow(x1, y1, x1  + DigSizeX * k - 1, y1 + DigSizeY * k - 1);

  for (uint8_t tmpy = 0;  tmpy < DigSizeY * k; tmpy++ ) {
    for (uint8_t tmpx = S.startx; (tmpx >= 0) && (tmpx < DigSizeX * k); tmpx = tmpx + S.addX) {
      int i = ((tmpx / k + (tmpy / k) * DigSizeX) * 2);
      if (i >= sizeof(GifDIG1)) {
        i = i - sizeof(GifDIG1);
      };
      if (d == 0) a = pgm_read_word(&(GifDIG1[i]));
      if (d == 1) a = pgm_read_word(&(GifDIG2[i]));
      if (d >= 2) a = pgm_read_word(&(GifDIG3[i]));
      a = SwapWord(a);
      if (a == 0xfa5f) a = MainColor; //C0;

      DE(n).pushColor(a);
    } ;
  };


};

void PrintPing(byte n, int sc) {
  // Рисует символ кто с кем соединен!

  byte n1 = Port2Screen1[n - 1];
  byte n2 = Port2Screen2[n - 1];

  DE(n1).setTextSize(1);
  DE(n1).setCursor(110, 75);
  DE(n1).println(char(sc));

}

void DoDoors(byte n) {
  // Делает рисование процесса дверей!
  // n это port serialport


  if ((n < 1) OR (n > 3)) return;

  byte n1 = Port2Screen1[n - 1];
  byte n2 = Port2Screen2[n - 1];

  if (Doors[n - 1] == -1) { //Если открыта или открываем
    if (DoorX[n - 1] > 0) { // Если в процессе
      DoorX[n - 1]--;
      for (int i = 15; i >= 0; i--) {
        DE(n1).drawRect( 0 + i * 8 + DoorX[n - 1], 122,                      1, 6, BLACK);
        DE(n2).drawRect( 0,                        0 + i * 8 + DoorX[n - 1], 6, 1, BLACK);
      }
    } else {
      //DoorX[n - 1] = 0;
      //Doors[n - 1] = 0;
    }
  }

  if (Doors[n - 1] == 1) { //Если закрыта или закрываем
    if (DoorX[n - 1] < 8) { // Если в процессе
      for (int i = 0; i < 16; i++) {
        DE(n1).drawRect( 0 + i * 8 + DoorX[n - 1], 122,                      1, 6, WHITE);
        DE(n2).drawRect( 0,                        0 + i * 8 + DoorX[n - 1], 6, 1, WHITE);
      }
      DoorX[n - 1]++;
    } else {
      //DoorX[n - 1] = 0;
      //Doors[n - 1] = 0;
    }
  }
};


//inline uint16_t C565(uint8_t r, uint8_t g, uint8_t b) {
//  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
//}


void CalcPalette(byte n, word c0, word c1, word c2) {

  // Serial.print(">>>>");
  // Serial.print(c0,HEX);
  // Serial.print(" =");
  // Serial.print(C565(C565R(c0),C565G(c0),C565B(c0)), HEX );
  // Serial.println("<<<<");
  float r1 = (C565R(c1) - C565R(c0)) / 15;
  float g1 = (C565G(c1) - C565G(c0)) / 15;
  float b1 = (C565B(c1) - C565B(c0)) / 15;
  float r2 = (C565R(c2) - C565R(c0)) / 15;
  float g2 = (C565G(c2) - C565G(c0)) / 15;
  float b2 = (C565B(c2) - C565B(c0)) / 15;
  for (uint8_t y = 0;  y < 8; y++ ) {
    for (uint8_t x = 0; x < 8; x++) {
      Colors1[y][x] = C565(int(r1 * x + r2 * y + C565R(c0)), int(g1 * x + g2 * y + C565G(c0)), int(b1 * x + b2 * y + C565B(c0)));
    } ;
  };
};

void DrawPalette(byte n) {
  // Делает рисование процесса дверей!
  // n это port serialport
  for (uint8_t y = 0;  y < 8; y++ ) {
    for (uint8_t x = 0; x < 8; x++) {
      DE(n).fillRect( int(128 - (x + 1) * 16), y * 16, 15, 15, Colors1[y][x] );
      //
      //Colors1[y][x]);
      //Serial.println(x*y);
    } ;
  };
};
