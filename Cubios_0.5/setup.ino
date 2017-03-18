void SpritesInit() {
  byte ind = 0;
  if (HAVESPRITE == 1) {
    //if ( NewSprite(random(65535), 1, 60, 12, 1.2, 0.6, ind) == false) {}; //1.6, 0.4
    NewSprite(random(65535), 2, 32, 16, 1.1, 0.8, ind);
    //NewSprite(random(65535), 3,  5, 24, 0.7, 1.2, ind); //Serial.println("Sprite3 INIT ER!");
  }
};

void setup() {
  int r = 0;

  bus.begin(19200); //19200);

  for (byte i = 1; i <= 3; i++) {
    DE(i).initR(InitInit[i - 1]); //INITR_GREENTAB);

    DE(i).fillScreen(0);
    DE(i).setRotation(1);
    DE(i).setTextSize(3);
    DE(i).setCursor(35, 50);
    DE(i).println(i);
  };


  // инициализируем рандом!
  for (int i; i < 20; i++) {
    r = r + analogRead(i);
  };
  r = r ^ micros(); // побитовый xor;
  randomSeed(r);

  SpritesInit();

  for (byte i = 1; i <= 3; i++) DE(i).fillScreen(0);

  CalcPalette(1, MainColor, MainColor, MainColor ) ; // neighborColor[nextn], neighborColor[n]);
  DrawPalette(1); DrawPalette(2); DrawPalette(3);

}

