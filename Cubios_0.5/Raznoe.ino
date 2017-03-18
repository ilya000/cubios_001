/*
void loop() { /////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////

  n++;
  if ((n == 4) || ( n > 70)) { // чтобы первый раз поработало вхолостую n=50
    n = 0;
  }

  old_time = current_time;
  DirStatus = UNDEFINED;

  //if (propusk == false)
  for (int i = 0; i < SpritS; i++) {
    Sprite[i].oldX = Sprite[i].X;  Sprite[i].oldY = Sprite[i].Y;
  }

  if (n==0) trySendConform();


  if ((n > 0) && (n < 4)) {
    bus.Ping(n);
    DirStatus = bus.DirExist(n);

    if (DirStatus == NO) {  // нет связи
      DoCloseDoor(n);
    }
    if (DirStatus == UNDEFINED) { //непонтно но пинг брошен, выясним
      //   DoPauseDoor(n);
    }
    if (DirStatus == YES) { //есть связь
      DoOpenDoor(n);
    } // DirStatus == YES
  } // n= 1..4

  if (DirStatus == YES) {
    DataIn = bus.ReadVector(n); // читаем данные
  }

  if (DataIn.type == ERROR)
  {
    //Serial.println(">>>>>>ER>>>>>>>>");
    //Serial.println(DataIn.annex_id);
    //Serial.println(F("READVECTOR ERROR!!!"));
  }
  else { // != ERROR
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    if (DataIn.type == DATA) {  // получаем и начинам подтверждать
      //Serial.println(">>>>>>> DATA >>>>>>>");
      byte ind = 0;
      if (NewSpriteFromVector(DataIn, ind) == true) { //если можем создать

        bus.SendConfirm(n, DataIn.id, ACCEPT);
       // Serial.print("\n~mark for conf and ACCEPT~");
       // Serial.println(DataIn.id);

        MarkNeedConform(n, DataIn.id, ind);


      } else
      { // если не можем взять// место например нет на экране
        //bus.SendConfirm(n, DataIn.id, REJECT);
        //Serial.println(":::::REJECT:::::");
      }
    }

    if (DataIn.type == ACCEPT) { // отдали и получили подтвержение, удалем подтвержаем
      // они получили и приняли
      // Serial.println("\n<<in<<<ACCEPT<<<<<<");
      if (HideSprite(DataIn.id) == true) {
        DeleteObj(DataIn.id);
        
        bus.SendConfirm(n, DataIn.id, CONFIRMABOUTCONFIRM);
        
       // delay(10);
       // bus.SendConfirm(n, DataIn.id, CONFIRMABOUTCONFIRM);
      }

    }
    if (DataIn.type == REJECT) {
      //UnmarkNeedConform(n, DataIn.id, ind);
      // они полуили но не приняли
      //FogetObjFromListToSend(DataIn.id);
      //Пересчитать направление об'екта
    }
    if (DataIn.type == CONFIRMABOUTCONFIRM) { //Они получили принли и подтвердили принятие
      // больше не плдтвержаем
      UnmarkNeedConform(DataIn.id);
      
      //Serial.println("\n<<in<<<CONFIRMABOUTCONFIRM<<<<<<");
      //DeleteObj(DataIn.id);
    }

  } // != ERROR


  for (int i = 0; i < SpritS; i++) {
    Sprite[i].X = Sprite[i].X + Sprite[i].incX;
    Sprite[i].Y = Sprite[i].Y + Sprite[i].incY;
    if (Sprite[i].X >= (TFTW + GAP + TFTW + GAP + TFTW - 4 - DigSizeX)) {
      Sprite[i].incX = abs(Sprite[i].incX) * -1; //Sprite[i].incX * -1;
      Sprite[i].X = Sprite[i].X + Sprite[i].incX;
    };
    if (Sprite[i].Y >= (TFTH - 4 - DigSizeY)) {
      Sprite[i].incY = abs(Sprite[i].incY) * -1; //Sprite[i].incY * -1;
      Sprite[i].Y = Sprite[i].Y + Sprite[i].incY;
    };
    if (Sprite[i].X <= 4) {
      Sprite[i].incX = abs(Sprite[i].incX); //Sprite[i].incX * -1;
      Sprite[i].X = Sprite[i].X + Sprite[i].incX;
    };
    if (Sprite[i].Y <= 4) {
      Sprite[i].incY = abs(Sprite[i].incY); //Sprite[i].incY * -1;
      Sprite[i].Y = Sprite[i].Y + Sprite[i].incY;
    };
    // проверка передачи соседу c экстраполяцийе
    if (Sprite[i].incY > 0)
      if (Sprite[i].Y + Sprite[i].incY * 4 > (TFTH - 4 - DigSizeY)) {
        DataOut.type = DATA;
        DataOut.id = Sprite[i].id;
        DataOut.dir = n;
        DataOut.Data.f.x = Sprite[i].X + Sprite[i].incX * GAP;//);
        DataOut.Data.f.y = Sprite[i].Y + Sprite[i].incY * GAP;//);
        DataOut.Data.f.incx = Sprite[i].incX;//);
        DataOut.Data.f.incy = Sprite[i].incY;//);
        bus.SendVector(2, DataOut);
      }
  } //for


  // if (propusk == false) DrawGame(); else Serial.println("\n\t SKIP DRAW");
  DrawGame();


  current_time = millis();

  // Serial.print("\n\t current_time-old_time=");
  // Serial.println(current_time - old_time);
  delta = (current_time - old_time);

  //if (delta > 33) {
  //  propusk = true;
  //  } else {
  //  propusk = false;
  //  delay(33 - delta);
  //  } 
  delay(33);
}
*/
