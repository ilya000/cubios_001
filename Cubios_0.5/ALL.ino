bool NewSprite(word id, byte scr, int x, int y, float incx, float incy, byte &ind) {
  if (SpritS >= SpritLimit) return false;
  for (int i = 0; i < SpritS; i++) {
    ind = SpritS;
    if (Sprite[i].id == id) return false; //true; ///false; Уже есть НО! Псть будет! Удаляй на той стороне!
  };

  Sprite[SpritS].id = id;
  Sprite[SpritS].scr = scr; 
  Sprite[SpritS].hidden = false;
  Sprite[SpritS].X = 0.0 + x;
  Sprite[SpritS].oldX = 0.0 + x;  //? было y - ошибка?
  Sprite[SpritS].incX = 0.0 + incx;
  Sprite[SpritS].Y = 0.0 + y;
  Sprite[SpritS].oldY = 0.0 + y;
  Sprite[SpritS].incY = 0.0 + incy;
  Sprite[SpritS].DigStep = 0;
  Sprite[SpritS].addX = 1;
  Sprite[SpritS].startx = 0;
  Sprite[SpritS].waitConform = 0;
  Sprite[SpritS].waitN = 0;
  ind = SpritS;
  SpritS++;
  return true;
};

bool NewSpriteFromVector(Vector &V, byte n, byte &ind) {
  byte scr = 0;
  if (V.dir == DIRX) scr = Port2Screen2[n-1];
  if (V.dir == DIRY) scr = Port2Screen1[n-1];
  if (scr == 0) return false;
  
  return NewSprite(V.id, scr ,V.Data.f.y, V.Data.f.x, V.Data.f.incx * -1, V.Data.f.incy * -1, ind);
};


void trySendConform() {
  for (int i = 0; i < SpritS; i++) {
    if (Sprite[i].waitConform > 0) {
      bus.SendConfirm(Sprite[i].waitN, Sprite[i].id, ACCEPT);
      Sprite[i].waitConform++;
    };
    if ( Sprite[i].waitConform > 10 ) {
      Sprite[i].waitConform = 0;
    }
  };
};

bool MarkNeedConform(byte N, word id, byte &inx)
{
  if (Sprite[inx].id != id) {
  //  Serial.println("\n\t\t id!=id  чтото не так===============!!!!!");
  //  delay(5000);
    return false;
  }
  Sprite[inx].waitConform = 1;
  Sprite[inx].waitN = N;
  return true;
};

void UnmarkNeedConform(word id)
{
 // Serial.print("\n~Unmark---- ~");

  for (int i = 0; i < SpritS; i++) {
    if (Sprite[i].id == id) {
  //    Serial.print(Sprite[i].id);
  //    Serial.print("~");
      Sprite[i].waitConform = 0;
      Sprite[i].waitN = 0;
    }
  }
 // Serial.println("~~)");
};


//void SpritesInit();


bool isDoorInProgress(int n) {};



void DoOpenDoor(byte n) {
  Doors[n - 1] = -1;
  DE(Port2Screen1[n-1]).drawRect( 10, 117, 40, 2, GREEN);
  DE(Port2Screen1[n-1]).drawRect( 50-2, 117-2, 6, 6, GREEN);
  DE(Port2Screen2[n-1]).drawRect( 117, 10, 2, 40, GREEN);
  DE(Port2Screen2[n-1]).drawRect( 117-2, 50-2, 6, 6, GREEN);
  
};
void DoCloseDoor(byte n)  {
  Doors[n - 1] = 1;
  DE(Port2Screen1[n-1]).drawRect( 10, 117, 40, 2, BLACK);
  DE(Port2Screen1[n-1]).drawRect( 50-2, 117-2, 6, 6, BLACK);
  DE(Port2Screen2[n-1]).drawRect( 117, 10, 2, 40, BLACK);
  DE(Port2Screen2[n-1]).drawRect( 117-2, 50-2, 6, 6, BLACK);
  
  //nfillRect(Port2Screen2[n-1], 110, 75, 7, 7, BLACK);
};
void DoPauseDoor(byte n)  {
  Doors[n - 1] = 0;
  DE(Port2Screen1[n-1]).drawRect( 10, 117, 40, 2, RED);
  DE(Port2Screen1[n-1]).drawRect( 50-2, 117-2, 6, 6, RED);
  DE(Port2Screen2[n-1]).drawRect( 117, 10, 2, 40, RED);
  DE(Port2Screen2[n-1]).drawRect( 117-2, 50-2, 6, 6, RED);
  //delay(10);
};


bool HideSprite(word id) {
  for (int i = 0; i < SpritS; i++) {
    if (Sprite[i].id == id) {
      Sprite[i].hidden = true;
      DrawHideSprite(Sprite[i]);
      return true;
    };
  };
  return false;
};

void DeleteObj(word id) {
  //int iii = -1;
  for (int i = 0; i < SpritS; i++) {
    if (Sprite[i].id == id) {
      SpritS--;
      Sprite[i] = Sprite[SpritS];
      return;
    };
  };
};


void CalcGame() {
  //Сохраняем старые координаты
  for (int i = 0; i < SpritS; i++) {
    Sprite[i].oldX = Sprite[i].X;  
    Sprite[i].oldY = Sprite[i].Y;
  }

  
  for (int i = 0; i < SpritS; i++) {
    Sprite[i].X = Sprite[i].X + Sprite[i].incX;
    Sprite[i].Y = Sprite[i].Y + Sprite[i].incY;
    if (Sprite[i].X >= (TFTW - 1 - DigSizeX*2)) {
      Sprite[i].incX = abs(Sprite[i].incX) * -1; 
      Sprite[i].X = Sprite[i].X + Sprite[i].incX;
    };
    if (Sprite[i].Y >= (TFTH - 1 - DigSizeY*2)) {
      Sprite[i].incY = abs(Sprite[i].incY) * -1; 
      Sprite[i].Y = Sprite[i].Y + Sprite[i].incY;
    };
    if (Sprite[i].X <= -16) {
      Sprite[i].incX = abs(Sprite[i].incX); 
      Sprite[i].X = Sprite[i].X + Sprite[i].incX;
    };
    if (Sprite[i].Y <= -16) {
      Sprite[i].incY = abs(Sprite[i].incY); 
      Sprite[i].Y = Sprite[i].Y + Sprite[i].incY;
    };
     
  } //for

};

void DrawGame() {
  for (int i = 0; i < SpritS; i++) DrawSprite(Sprite[i]);
  //DoDoors(3); DoDoors(2); DoDoors(1);
};


