#include "msp430g2553.h"
 //-------------------------- khai b�o port v�o/ra
#define LEDS P2OUT //khai bao leds bang output
#define K1 BIT0 //khai bao nut nhan K1
#define K2 BIT1 //khai bao nut nhan K2
#define K3 BIT2 //khai bao nut nhan K3
#define K4 BIT3 //khai bao nut nhan K4
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
    blink(5); //    CHO C�C LED O PORT 2 CHOP TAT 5 LAN

    //---V�ng lap chuong tr�nh ch�nh--------------
    while (1) {
        if ((P1IN & K1) == 0) P2OUT = 0x01;
        if ((P1IN & K2) == 0) P2OUT = 0x02;
        if ((P1IN & K3) == 0) P2OUT = 0x03;
        if ((P1IN & K4) == 0) P2OUT = 0x04;
        delayms(200);
    } // end while
} // end main
//----------- �?nh nghia chuong tr�nh con------------------
//------------------------------dinh nghia chuong trinh delay
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