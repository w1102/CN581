#include "msp430g2553.h"
 //#define K    BIT0
#define LEDS P2OUT //khai bao led tren toan port2

#define K1 BIT0 //khai bao ph�m nhan K1
#define K2 BIT1 //khai bao ph�m nhan K2
#define K3 BIT2 //khai bao ph�m nhan K3
#define K4 BIT3 //khai bao ph�m nhan K4

unsigned char key;

//----------------------------------------------
void delayms(int ms); // khai bao ham delay
void blink(int time); //khai bao ham cho tat led
void shiftL(); //khai bao ham dich trai
void shiftR(); //khai bao ham dich phai
void sangdantrai(); //khai bao ham sang dan trai
void sangdanphai(); //khai bao ham sang dan phai

//---------------------------------------------

void main(void) {
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    P2DIR = 0XFF; //khai bao ngo ra output cho port2
    P2OUT = 0xff; //cho tat cat ngo ra tren port2 la ot put
    P2SEL = 0; // cho phep su dung tat ca cac bit tren port2 lam output

    P1DIR = BIT4; //cho phep ngo ra intput tren port1
    P1REN |= (BIT0 + BIT1 + BIT2 + BIT3); //bat dien tro keo
    P1OUT |= (BIT0 + BIT1 + BIT2 + BIT3); //bat dien tro len

    P1IE |= (BIT0 + BIT1 + BIT2 + BIT3); // cho phep ngat
    P1IES |= (BIT0 + BIT1 + BIT2 + BIT3); //cho phep ngat tat ca cac bit
    _BIS_SR(GIE); //cbho phep ngat toan cuc
    blink(5); // cho led chop tat 5 lan

    while (1) {
        //key=P1IN & K;
        // if(key==0)
        //LEDS=0x01;
        //delayms(100);
        //LEDS= 0x00;
        delayms(200);
    }
}
//------------------------------khai bao vector ngat
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) { // dinh nghia ham ngat

    if ((P1IFG & K1) == K1) {
        shiftL();
        delayms(200);

    }
    if ((P1IFG & K2) == K2) {
        LEDS = 0x02;
        shiftR();
        delayms(200);

    }
    if ((P1IFG & K3) == K3) {
        LEDS = 0x03;
        sangdantrai();
        delayms(200);
    }

    if ((P1IFG & K4) == K4) {
        LEDS = 0x04;
        sangdanphai();
        delayms(200);

    }
    P1IFG = 0x00;
}
//--------------------------------dinh nghia ham delay
void delayms(int ms) {
    for (int i = 0; i < ms; i++)
        __delay_cycles(100);
}
//---------------------------------dinh nghia ham chop tat led
void blink(int time) {
    for (int i = 0; i < time; i++) {
        LEDS = 0xff;
        delayms(200);
        LEDS = 0x00;
        delayms(200);
    }
}
//-----------------------------dinh nghia ham dich trai
void shiftL() {
    char dat = 0x01;
    for (int i = 0; i < 8; i++) {
        LEDS = dat;
        dat = dat << 1;
        delayms(200);

    }
}
//----------------------------d?nh nghia ham d?ch phai
void shiftR() {
    char dat = 0x80;
    for (int i = 0; i < 8; i++) {
        LEDS = dat;
        dat = dat >> 1;
        delayms(200);

    }
}

//--------------------------d?nh nghja ham sang dan trai
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
//-------------------------d?nh nghia ham sang dan phai
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