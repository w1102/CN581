#include "msp430g2553.h"
 //-------------------------- khai b�o port v�o/ra
#define LEDS P2OUT // khai bao leds bang output
//#define LEDS1 P2OUT
//#define LEDS2 P2OUT
//#define LEDS3 P2OUT
#define K1 BIT0 //khai bao phim an K1
#define K2 BIT1 //khai bao phim an K2
#define K3 BIT2 //khai bao phim an K3
#define K4 BIT3 //khai bao phim an K4

//---------------------------khai bao prototypr h�m dich tr�i, dich phai,s�ng dan phai,s�ng d?n trai

void shiftL(); //khai bao ham dich trai
void shiftR(); //khai bao ham dich phai
void sangdanphai(); //khai bao ham sang dan phai
void sangdantrai(); //hai bao ham sang dan trai

//---------------------------- Khai b�o prototype h�m
void delayms(int ms);
void blink(int time);
//----------------------------�?nh nghia chuong tr�nh ch�nh
void main() {
    //---kh?i d?ng CHIP--------------
    WDTCTL = WDTPW + WDTHOLD;
    P1DIR = BIT4; //MO OUTPUT CHO C�C BIT 0,1,2,3 O PORT 1 L�M NUT NHAN
    P1REN |= (BIT0 + BIT1 + BIT2 + BIT3); //BAT DIEN TRO KEO
    P1OUT |= (BIT0 + BIT1 + BIT2 + BIT3); //BAT DIEN TRO KEO LEN CHO C�C BIT O PORT 1
    P2DIR = 0xFF; //MO OUTPUT CHO C�C BIT O PORT 2
    P2SEL = 0x00; //CHO PHEP SU DUNG C�C BIT O PORT 2 L�M OUTPUT
    blink(5); //CHO C�C LED O PORT 2 CHOP TAT 5 LAN

    //---V�ng lap chuong tr�nh ch�nh--------------
    while (1) {
        if ((P1IN & K1) == 0) shiftL();
        if ((P1IN & K2) == 0) shiftR();
        if ((P1IN & K3) == 0) sangdanphai();
        if ((P1IN & K4) == 0) sangdantrai();
        delayms(200);
    }
}
//----------- �?nh nghia chuong tr�nh con------------------
//------------------------------dinh nghia chong trinh delay
void delayms(int ms) {
    for (int i = 0; i < ms; i++)
        __delay_cycles(100);
}
//------------------------------dinh nghia chuong trinh chop tat led
void blink(int time) {
    for (int i = 0; i < time; i++) {
        LEDS = 0xFF;
        delayms(200);
        LEDS = 0x00;
        delayms(200);
    }
}
//------------------------------dinh nghia chuong trinh dich trai
void shiftL() {
    char dat = 0x01;
    for (int i = 0; i < 8; i++) {
        LEDS = dat;
        dat = dat << 1;
        delayms(200);
    }
    LEDS = 0x00;
}
//--------------------------------dinh nghia chuong trinh dich phai
void shiftR() {
    char dat = 0x80;
    for (int i = 0; i < 8; i++) {
        LEDS = dat;
        dat = dat >> 1;
        delayms(200);
    }
    LEDS = 0x00;
}
//------------------------------dinh nghia chuong trinh sang dan phai
void sangdanphai() {
    char dat = 0x80;
    char dat2 = 0x00;

    for (int i = 0; i < 8; i++) {
        LEDS = dat2 + dat;
        dat = dat >> 1;
        dat2 = LEDS;
        delayms(200);
    }
    LEDS = 0x00;
}
//----------------------------------dinh nghia chuong trinh sang dan trai
void sangdantrai() {
    char dat = 0x01;
    char dat1 = 0x00;
    for (int i = 0; i < 8; i++) {
        LEDS = dat1 + dat;
        dat = dat << 1;
        dat1 = LEDS;
        delayms(200);
    }
    LEDS = 0x00;
}