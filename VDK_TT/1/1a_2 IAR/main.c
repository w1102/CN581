#include "msp430g2553.h"
 // khai b�o port v�o/ra
#define LEDS P2OUT //khai bao LEDS bang output
//#define LEDS1 P2OUT
// Khai b�o prototype h�m
void delayms(int ms); //khai bao ham delay
void blink(int time); //khai bao ham choop tat led 
void shiftR(); //khai bao ham dich phai
void shiftL(); //khai bao ham dich trai
void sangdanphai(); //khai bao ham sang dan trai
void sangdantrai(); //khai bao ham sang dan phai
//�?nh nghia chuong tr�nh ch�nh
void main() {
    //---kh?i d?ng CHIP--------------
    WDTCTL = WDTPW + WDTHOLD;
    P2DIR = 0xFF;
    P2SEL = 0x00;
    blink(5);
    //---V�ng l?p chuong tr�nh ch�nh--------------
    while (1) {
        shiftR();
        delayms(200);

        shiftL();
        delayms(200);

        sangdanphai();
        delayms(200);

        sangdantrai();
        delayms(200);
    }
}
//----------- �?nh nghia chuong tr�nh con------------------
//------------dinh nghia ham delay
void delayms(int ms) {
    for (int i = 0; i < ms; i++)
        __delay_cycles(80);
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
//-------------dinh ham dich phai
void shiftR() {
    char dat = 0x80;
    for (int i = 0; i < 8; i++) {
        LEDS = dat;
        dat = dat >> 1;
        delayms(200);

    }
}
//--------------dinh nghia ham dich trai
void shiftL() {
    char dat = 0x01;
    for (int i = 0; i < 8; i++) {
        LEDS = dat;
        dat = dat << 1;
        delayms(200);

    }
}
//--------------dinh nghia h�m s�ng dan phai

void sangdanphai() {
    char dat = 0x80;
    char dat2 = 0x00;

    for (int i = 0; i < 8; i++) {
        LEDS = dat2 + dat;
        dat = dat >> 1;
        dat2 = LEDS;
        delayms(200);

    }
}
//---------------dinh nghia ham sang dan tr�i
void sangdantrai() {
    char dat = 0x01;
    char dat2 = 0x00;

    for (int i = 0; i < 8; i++) {
        LEDS = dat2 + dat;
        dat = dat << 1;
        dat2 = LEDS;
        delayms(200);

    }
}