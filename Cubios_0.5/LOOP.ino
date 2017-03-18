void loop() {

  static int n = 0;
  int sc, dn = 0;
  int neighbor[4] = {0, 0, 0, 0};
  static int neighborColor[4] = {MainColor, MainColor, MainColor, MainColor};
  int prevn, nextn;

  //word col = 0;

  /* n++;  if (n == 4) n = 1; */
  for (n = 1; n < 4; n++) {

    current_time = millis();
    DirStatus = UNDEFINED;


    bus.Ping(n);
    DirStatus = bus.DirExist(n);

    if (DirStatus == NO) {  // нет связи
      DoCloseDoor(n);
      neighbor[n] = 0; // размыкалось, значит потом надо читать вновь
    }
    if (DirStatus == UNDEFINED) { //непонтно но пинг брошен, выясним
      DoPauseDoor(n);
    }
    if (DirStatus == YES) { //есть связь
      //sc = bus.Peek(n);
      DataIn = bus.ReadVector(n);

      DoOpenDoor(n);
      // PrintPing(n, sc);

      if (DataIn.type == DATA) {  // получаем и читаем
        byte ind = 0;
        if (NewSpriteFromVector(DataIn, n , ind) == false) { //если НЕ можем создать
          Mig();
        }
      }

    } // DirStatus == YES

  }// for

  CalcGame();

  // проверка передачи соседу c экстраполяцийе
  for (int i = 0; i < SpritS; i++) {
    if (Sprite[i].incY < 0) {
      if (Sprite[i].Y + Sprite[i].incY * 2 < 0) { //

        DataOut.type = DATA;
        DataOut.id = Sprite[i].id;
        DataOut.dir = DIRY;
        DataOut.Data.f.x = Sprite[i].X + Sprite[i].incX * GAP;//);
        DataOut.Data.f.y = Sprite[i].Y + Sprite[i].incY * GAP;//);
        DataOut.Data.f.incx = Sprite[i].incX;
        DataOut.Data.f.incy = Sprite[i].incY;
        if (bus.DirExist(Screen2PortY[Sprite[i].scr - 1]) == YES) {
          if (bus.SendVectorln(Screen2PortY[Sprite[i].scr - 1], DataOut) == YES) {

            DrawHideSprite(Sprite[i]);
            DeleteObj(Sprite[i].id);
            break; // выходим из цикла for, тк потенциально может разрушить очередь объектов, так как вынимаем прямо из под
          } else {
            Mig();
          }// SendVectorln

        } //DirExist
      }
    } // incY < 0
    if (Sprite[i].incX < 0) {
      if (Sprite[i].X + Sprite[i].incX * 2 < 0) { //

        DataOut.type = DATA;
        DataOut.id = Sprite[i].id;
        DataOut.dir = DIRX;
        DataOut.Data.f.x = Sprite[i].X + Sprite[i].incX * GAP;//);
        DataOut.Data.f.y = Sprite[i].Y + Sprite[i].incY * GAP;//);
        DataOut.Data.f.incx = Sprite[i].incX;
        DataOut.Data.f.incy = Sprite[i].incY;
        if (bus.DirExist(Screen2PortX[Sprite[i].scr - 1]) == YES) {
          if (bus.SendVectorln(Screen2PortX[Sprite[i].scr - 1], DataOut) == YES) {

            DrawHideSprite(Sprite[i]);
            DeleteObj(Sprite[i].id);
            break; // выходим из цикла for, тк потенциально может разрушить очередь объектов, так как вынимаем прямо из под
          } else {
            Mig();
          }// SendVectorln

        } //DirExist
      }
    } // incY < 0
  } //for


  DrawGame();
  //DoDoors(n);

  delay(10);



  // delta = millis() - current_time;  // БАГ!!!
  //delay(33 - delta);                // БАГ!!!
}
