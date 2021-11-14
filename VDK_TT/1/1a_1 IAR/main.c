#include "msp430g2553.h"
 // khai b?o port v?o/ra
#define LEDS P2OUT //khai bao leds bang ngo ra output
//---------- Khai b?o prototype h?m
void delayms(int ms); //khai bao ham delay
void blink(int time); //khai bao ham chop tat led 
void shiftL(); //khai bao ham dich trai
//??nh nghia chuong tr?nh ch?nh
void main() {
    //---kh?i d?ng CHIP--------------
    WDTCTL = WDTPW + WDTHOLD;
    P2DIR = 0xFF;
    P2SEL = 0x00;
    blink(5);
    //---V?ng l?p chuong tr?nh ch?nh--------------
    while (1) {
        shiftL(); // module led sang tr?i
        delayms(500); // module delay
    }
}
//----------- ??nh nghia chuong tr?nh con------------------
//-----------dinh nghia ham delay
void delayms(int ms) {
    for (int i = 0; i < ms; i++)
        __delay_cycles(100);
}
//------------dinh nghia ham chop tat led
void blink(int time) {
    for (int i = 0; i < time; i++) {
        LEDS = 0xFF;
        delayms(200);
        LEDS = 0x00;
        delayms(200);
    }
}
//------------ dinh nghia ham dich trai
void shiftL() {
    char dat = 0x80;
    for (int i = 0; i < 8; i++) {
        LEDS = dat;
        dat = dat >> 1;
        delayms(200);
    }
} // end main