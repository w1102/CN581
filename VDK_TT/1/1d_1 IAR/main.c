#include "msp430g2553.h"
 //#define K    0
#define LEDS P2OUT //khai bao led tren toan port2

#define K1 BIT0 //khai bao ph�m nhan K1
#define K2 BIT1 //khai bao phim nhan K2
#define K3 BIT2 //khai bao phim nhan K3
#define K4 BIT3 //khai bao phim nhan K3

unsigned char key; //khai bao bien key
unsigned char key1; //khai bao bien key1
//----------------------------------------------
void delayms(int ms); // khai bao ham delay
void blink(int time); // khai bao ham chop tat led
void shiftL(); // khai bao ham dich trai
void shiftR(); // khai bao ham dich phai
void sangdantrai(); // khia bao ham sang dan trai
void sangdanphai(); // khai bao ham sang dan phai
void tangsonhiphan(); // khai bao hamfm tang so nhi phan
void giamsonhiphan(); // khai bao ham giam so nhi phan
void ngat(); // khai bao ham ngat so nhi phan ve 0xff
void ngat1(); // khai boa ham ngat so nhi phan ve 0x00

//---------------------------------------------

void main(void) {
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    P2DIR = 0xff; // khai bao ngo ra output cho port2
    P2OUT = 0xff; // ngo ra output cho port 2
    P2SEL = 0; // cho phep su dung tat ca cac bit tren port2 lam output

    P1DIR = BIT4; //khai intput tren port1
    P1REN |= (BIT0 + BIT1 + BIT2 + BIT3); //bat dien tro keo
    P1OUT |= (BIT0 + BIT1 + BIT2 + BIT3); //bat dien tro len

    P1IE |= (BIT0 + BIT1 + BIT2 + BIT3); // cho phep ngat
    P1IES |= (BIT0 + BIT1 + BIT2 + BIT3); //cho phep ngat tren cac bit
    _BIS_SR(GIE); //cbho phep ngat toan cuc
    blink(5); // cho led chop tat 5 lan

    while (1) {
        key = P1IN & K1;
        key1 = P1IN & K2;

        if (key == 0) {
            tangsonhiphan();
            delayms(200);
        }
        if (key1 == 0) {
            giamsonhiphan();
            delayms(200);
        }
    }

}
//------------------------------khai bao vector ngat
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) { //dinh nghia ham ngat

    //if((P1IFG & K1)==K1){
    //}
    //if((P1IFG & K2)==K2){
    //}
    if ((P1IFG & K3) == K3) {
        ngat();
        delayms(200);
    }

    if ((P1IFG & K4) == K4) {
        ngat1();
        delayms(500);

    }
    P1IFG &= ~(BIT0 + BIT1 + BIT2 + BIT3);
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
//----------------------------dinh nghia ham dich phai
void shiftR() {
    char dat = 0x80;
    for (int i = 0; i < 8; i++) {
        LEDS = dat;
        dat = dat >> 1;
        delayms(200);

    }
}

//--------------------------dinh nghia ham sang dan trai
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
//-------------------------dinh nghia ham sang dan phai
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
//---------------------------dinh nghia ham tang so nhi phan
void tangsonhiphan() {
    char dat = 0x00;
    for (int i = 0; i < 256; i++) {
        LEDS = dat;
        dat = (dat + 1);
        delayms(30);
    }
    LEDS = 0x00;
}

//---------------------------dinh nghia ham giam so nhi phan
void giamsonhiphan() {
    char dat = 0xff;
    for (int i = 0; i < 256; i++) {
        LEDS = dat;
        dat = dat - 1;

        delayms(30);

    }
    LEDS = 0XFF;
}

//----------------------------khai bao ham dua so nhi phan ve 0xff
void ngat() {
    LEDS = 0X00;
    delayms(500);

}
//----------------------------khai bao ham dua so nhi phan ve 0x00
void ngat1() {
    LEDS = 0xFF;
    delayms(500);
}