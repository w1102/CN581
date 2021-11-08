#include "msp430g2553.h"

// led 7seg pin
#define PDAT P2OUT
#define PROW P1OUT

//----for key------------
#define K1 BIT0
#define K2 BIT1
#define K3 BIT2
#define K4 BIT3

// function prototype declaration
void delayms(int);
void scanled();
void toDisplay(int);
unsigned int ReadADC10(int chanel);

// variable declaration
char code7seg[] = {
    0xC0,
    0xF9,
    0xA4,
    0xB0,
    0x99,
    0x92,
    0x82,
    0xF8,
    0x80,
    0x90
};

char code74HC[] = {
    0x00,
    0x40,
    0x80,
    0xC0
};

char buffer[4] = {
    0,
    0,
    0,
    0
};
int idx = 0;
int value = 0;

// =============================== main ============================

void main(void) {
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    //Set MCLK = SMCLK = 1MHz
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    //P1SEL = 0x00;
    P1DIR = 0xF0;
    P1REN = 0x0F;
    P1OUT = 0xFF;

    P2SEL = 0x00;
    P2DIR = 0xFF;

    TA0CTL = TASSEL_2 + ID_0 + MC_1; // SMCLK as clock source, divider 0, up-mode

    CCTL0 = CCIE;     // Enable interrupt for CCR0
    CCR0  = 5000;     // 5ms
    _BIS_SR(GIE);     // Enable global interrupts

    // Khởi động ADC
    ADC10CTL1 = ADC10DIV_3;
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON;

    toDisplay(value);

    for (;;) {

        // xử lý phím ấn
        if ((P1IN & K1) == 0) {
            delayms(100);
            value = ReadADC10(10);
        }

        if ((P1IN & K2) == 0) {
            delayms(100);
            value = ReadADC10(10);
            value = ((value - 673) * 423) / 1024;
        }

        if ((P1IN & K3) == 0) {
            delayms(100);
            value = 0;
        }

        toDisplay(value);

        //scanled();
        //delayms(5);
    }

}

//=============================== timer =============================================
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0(void) {
    scanled();
}

//============================= function ===============================
unsigned int ReadADC10(int chanel) {
    ADC10AE0 |= (BIT0<<chanel);
    ADC10CTL1 |= (chanel*0x1000u);
    ADC10CTL0 |= ENC + ADC10SC;
    while(!(ADC10CTL0 & ADC10IFG));
    return ADC10MEM;
}


void delayms(int ms) {
    int i;
    for (i = 0; i < ms; i++)
        __delay_cycles(1000);
}

void scanled() {

    PDAT = code7seg[buffer[idx]];
    PROW |= code74HC[idx];

    __delay_cycles(1000);
    PDAT = 0xFF;
    PROW &= ~code74HC[idx];

    idx = idx == 3 ? 0 : idx + 1;

}

void toDisplay(int value) {

    buffer[3] = value % 10;
    value /= 10;
    buffer[2] = value % 10;
    value /= 10;
    buffer[1] = value % 10;
    value /= 10;
    buffer[0] = value % 10;
}