//* ioSerial.cpp - Library for Serial; Created by iLya Osipov; Private *//

#include "Arduino.h"
#include <SoftwareSerial.h>
#include <AltSoftSerial.h>

#include "ioSerial3.h"

SoftwareSerial mySerial1(12, 10); // RX, TX
AltSoftSerial mySerial2;//(8,9);  // RX, TX

unsigned long LastTimeAvailable[3] {0,0,0};

ioSerial3::ioSerial3(byte iid)
{
    id = iid ;
    
}

byte ioSerial3::available(byte n)
{
    byte r = 0;
    char c = 0;
    
  /*  if (n == 1) {
        r = Serial.available();
        
        c = Serial.peek();
        //test for RESET pressed
        //Serial.print("{{{{{");
        //Serial.print(byte(c),HEX);
        //Serial.print('|');
        if (((c == 0x20) || (c == 0x30))) {// && ((c1 == 0x20) || (c1 == 0x30))) {
            //если идет байт реасета, и предыдущий тоже был таким то
            pinMode(ResetPin, OUTPUT); //call reset
            digitalWrite(ResetPin, HIGH);
        }
      //  if (((c == 0x20) || (c  == 0x30))) {c1=c ;} else {c1=0;};
        // если идет байт ресета, то запоминаем его, если нет то сбрасываем запоменное
        // так как должны идти подряд
        
    }
   */
  if (n == 1) r = Serial.available();
  if (n == 2) r = mySerial1.available();
  if (n == 3) r = mySerial2.available();
    
  return r;
}

byte ioSerial3::DirExist(byte n) // 1 - YES; 0 - NO; 255 - UNDEFINED
{
  byte r = UNDEFINED;
  if ((n<1) && (n>3)) { return r; };
  
  r = available(n);
    
  if (r == 0) { // неопределенное состояние - надо пингануть
    r = UNDEFINED;
    Ping(n);
      
    if ((millis() - LastTimeAvailable[n-1]) >= UNDEFINED_TIMEOUT) {
      r = NO;  // если слишком долго ждем значит там никого
    }
  }
  else {
    r = YES;  // Определенно там что есть
    LastTimeAvailable[n-1] = millis();
  }
  return r;
}

void ioSerial3::Ping(byte n)
{
    if (n == 1) Serial.write(   char(64+(id-1)*3+1)); // A //Было '.' //0x41
    if (n == 2) mySerial1.write(char(64+(id-1)*3+2)); // B
    if (n == 3) mySerial2.write(char(64+(id-1)*3+3)); // C
}

int ioSerial3::Peek(byte n)
{
    if (n == 1) return Serial.peek();
    if (n == 2) return mySerial1.peek();
    if (n == 3) return mySerial2.peek();
}

byte ioSerial3::ReadBuf(byte n, byte *buf, unsigned int len)
{

    byte i = 0;
    char c, prev = 0;
 
    
    while (available(n) > 0) {
        LastTimeAvailable[n-1] = millis();
  
        
        if (n==1) c = Serial.read();
        if (n==2) c = mySerial1.read();
        if (n==3) c = mySerial2.read();
        
        if (c==STARTCHAR1) { prev=c;
        }
        if ((c==STARTCHAR2) && (prev==STARTCHAR1)) {
            while ((available(n) > 0) && (i<len))  {
    
                if (n==1) c = Serial.read();
                if (n==2) c = mySerial1.read();
                if (n==3) c = mySerial2.read();
                buf[i] = c;
                i++;
            }
            return i;
        };
    };
    return i;
}

byte ioSerial3::SendBuf(byte n, byte *buf, unsigned int len)
{
    if (n == 1) {
        Serial.write(STARTCHAR1);
        Serial.write(STARTCHAR2);
        Serial.write(buf, len);
    }

    if (n == 2) {
        mySerial1.write(STARTCHAR1);
        mySerial1.write(STARTCHAR2);
        mySerial1.write(buf, len);
    }

    if (n == 3) {
        mySerial2.write(STARTCHAR1);
        mySerial2.write(STARTCHAR2);
        mySerial2.write(buf, len);
    }
    return UNDEFINED;
}

Vector ioSerial3::ReadVector(byte n)
{
    Vector R;
    byte rez;
    R.type = ERROR;
    R.id = 0; R.annex_id = 0;
    
    rez = ReadBuf(n, (byte*) &R, sizeof(R));
  
    if (rez != sizeof(R)) {
        R.type = ERROR;
        R.annex_id = 1;
        return R;
    
    }
    if ((R.annex_id + R.id) != 0xFFFF) {
        R.type = ERROR;
        R.annex_id = 2;
        return R;
    }
    //посылам подтверждение о прочтении //версия 1 март 2017
    if (n == 1) {
        Serial.write(ENDCHAR1);
        Serial.write(ENDCHAR2);
        Serial.write(ENDCHAR1);
        Serial.write(ENDCHAR2);
    }
    
    if (n == 2) {
        mySerial1.write(ENDCHAR1);
        mySerial1.write(ENDCHAR2);
        mySerial1.write(ENDCHAR1);
        mySerial1.write(ENDCHAR2);
    }
    
    if (n == 3) {
        mySerial2.write(ENDCHAR1);
        mySerial2.write(ENDCHAR2);
        mySerial2.write(ENDCHAR1);
        mySerial2.write(ENDCHAR2);
    }
    return R;
}

void ioSerial3::SendConfirm(byte n, int id, byte A)
{ // A = ACCEPT, REJECT, CONFIRMABOUTCONFIRM
  Vector V;
  V.type = A;
  V.id = id;
  V.dir = n;
  //V.Data.number = 0;
  SendVector(n, V);
}

byte ioSerial3::SendVector(byte n, Vector V)
{
  V.annex_id=0xFFFF-V.id;
  return SendBuf(n, (byte*) &V, sizeof(V));
}

byte ioSerial3::SendVectorln(byte n, Vector V)
{
    V.annex_id=0xFFFF-V.id;
    SendBuf(n, (byte*) &V, sizeof(V));
    
    //byte i = 0;
    char c, prev = 0;
    
    for(byte i=0; i<5; i++) {
    delay(20);
    while (available(n) > 0) {
        LastTimeAvailable[n-1] = millis();
        
        
        if (n==1) c = Serial.read();
        if (n==2) c = mySerial1.read();
        if (n==3) c = mySerial2.read();
        
        if ((c==ENDCHAR2) && (prev==ENDCHAR1)) {
            return YES;
        };
        if (c==ENDCHAR1) { prev=c;} else { prev = 0;};
    };
    } // for
    return NO;
}


void ioSerial3::begin(int s9600)
{
    Serial.begin(s9600);
    while (!Serial) {};
    
    mySerial1.begin(s9600);
    mySerial2.begin(s9600);
};
