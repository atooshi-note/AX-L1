/*
AX-L1

使用デバイス:Arduino Nano(中華)
ボード:Arduino Nano
プロセッサ:ATmega328P(Old Bootloader)
中華CH340は自動でCOMポートとして認識された(TH50 Windows11)
sysclk = 16MHz

timer0 : 8bit   5,6pin delay(),millis(),micros() 977Hz
timer1 : 16bit  9,10pin servo_lib 490Hz
timer2 : 8bit   3,11pin tone() 490Hz

タクトスイッチ
周期:200msec
ON幅:100msec(duty:50%)
A,B相の位相ズレ:50msec(90°)

温め3-1
手動温め29


MAP
Digital 2,4,7,8    : Internal Pull Up push:LOW
Digital 3  timer2  : MStimer2
Digital 9  timer1  : ロータリーエンコーダへ
Digital 10 timer1  : ロータリーエンコーダへ 相は現場で
Digital 11 timer2  : MStimer2
*/

#include <MsTimer2.h>

#define lbtn 2
#define atatame 4
#define syudou 7
#define rbtn 8
#define A 9
#define B 10

char FLG = 0;
char FLG1 = 0;
char FLG2 = 0;
char cnt = 0;
char cnt1 = 0;
char mscnt = 0;
char outselect = 0; // right
char pulsecnt = 0; // パルスの数 1pulse(H-L):2,2pulse(H-L-H-L):4,3pulse:6

void pulseout(void){
  if(FLG == 1){
    if(cnt < pulsecnt*2){
      if(FLG2 == 0){
        if(outselect == 0){
          digitalWrite(A,HIGH);
        }
        else{
          digitalWrite(B,HIGH);
        }
        digitalWrite(LED_BUILTIN,HIGH);
      }
      else{
        if(outselect == 0){
          digitalWrite(A,LOW);
        }
        else{
          digitalWrite(B,LOW);
        }
        digitalWrite(LED_BUILTIN,LOW);
      }

      if(FLG1 == 0){
        if(outselect == 0){
          digitalWrite(B,LOW);
        }
        else{
          digitalWrite(A,LOW);
        }
        digitalWrite(LED_BUILTIN,HIGH);
      }
      else{
        if(outselect == 0){
          digitalWrite(B,HIGH);
        }
        else{
          digitalWrite(A,HIGH);
        }
        digitalWrite(LED_BUILTIN,LOW);
      }
      if(mscnt == 1){ // 50msecずらす
        if(cnt1 == 0){
          FLG2=!FLG2;
        }
        cnt1 = 1;
      }
      if(mscnt == 2){ // 50msec*2=100msec
        FLG1 = !FLG1;
        mscnt = 0;
        cnt++;
        cnt1 = 0;
      }

    }
    else{
      cnt = 0;
      FLG = 0;
      FLG1 = 0;
      FLG2 = 0;
      digitalWrite(LED_BUILTIN,HIGH);
    }
  }
  else{
    digitalWrite(A,HIGH);
    digitalWrite(B,HIGH);
    digitalWrite(LED_BUILTIN,LOW);
  }
}

void buttonkanshi(void){
  mscnt++;
  if(mscnt >= 100){
    mscnt = 0;
  }

  unsigned char leftval = 0;
  unsigned char rightval = 0;
  unsigned char atatameval = 0;
  unsigned char syudouval = 0;

  leftval = digitalRead(lbtn);
  if(leftval == 0){
    FLG = 1;
    mscnt = 0;
    outselect = 1;
    pulsecnt = 1;
  }

  rightval = digitalRead(rbtn);
  if(rightval == 0){
    FLG = 1;
    mscnt = 0;
    outselect = 0;
    pulsecnt = 1;
  }

 atatameval = digitalRead(atatame);
  if(atatameval == 0){
    FLG = 1;
    mscnt = 0;
    outselect = 0;
    pulsecnt = 5;
  }

 syudouval = digitalRead(syudou);
  if(syudouval == 0){
    FLG = 1;
    mscnt = 0;
    outselect = 1;
    pulsecnt = 18;
  }
}

void setup() {
  MsTimer2::set(20,buttonkanshi); // 50msec period
  MsTimer2::start();
  //Serial.begin(9600);

  pinMode(lbtn, INPUT_PULLUP);
  pinMode(rbtn,INPUT_PULLUP);
  pinMode(atatame, INPUT_PULLUP);
  pinMode(syudou,INPUT_PULLUP);

  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);

  pinMode(LED_BUILTIN,OUTPUT);

}

void loop() {
  pulseout();
}
