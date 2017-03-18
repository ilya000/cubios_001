/*
  ioSerial3.h - Library for
  Created by ilya Osipov 
*/

#ifndef ioSerial3_h
#define ioSerial3_h

#define ACCEPT 1
#define REJECT 0
#define CONFIRMABOUTCONFIRM 0x55
//#define PING 5
#define ERROR 0x66
#define DATA 0x77

#define YES 1
#define NO 0
#define UNDEFINED 255

#define DIRX 52
#define DIRY 104

#define UNDEFINED_TIMEOUT 333

#define STARTCHAR1 '['
#define STARTCHAR2 '>'

#define ENDCHAR1 '>'
#define ENDCHAR2 ']'

#define SIGNDTR1 0x20
#define SIGNDTR2 0x30
#define ResetPin A1

#define DEBUG

#include "Arduino.h"


struct __f4_t
{
    float x;
    float y;
    float incx;
    float incy;
};

union __data_t
{
    __f4_t f;
    unsigned char byte[sizeof(__f4_t)];
};

struct Vector
{
 // byte nope;
  byte type;
  word id; // было byte
  byte dir;
  __data_t Data;
  word annex_id;
};

class ioSerial3
{
  public:
    ioSerial3(byte iid);
    
    void begin(int s9600);
    
    byte DirExist(byte n); // 1 - YES; 0 - NO; 255 - UNDEFINED
    Vector ReadVector(byte n);
    void SendConfirm(byte n, int id, byte A); // A = ACCEPT, REJECT, CONFIRMABOUTCONFIRM
    byte SendVector(byte n, Vector V);
    byte SendVectorln(byte n, Vector V);
    
    byte available(byte n);
    void Ping(byte n);
    int Peek(byte n);
    
    byte SendBuf(byte n, byte *buf, unsigned int len);
    byte ReadBuf(byte n, byte *buf, unsigned int len);
    
    private:
        byte id; // id от 1 до 8
};

#endif
