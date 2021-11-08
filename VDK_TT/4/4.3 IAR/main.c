#include <msp430f2112.h>

#include <stdio.h>

#include <stdlib.h>

#define SMCLK_F 1000000 // frequency of Sub-System Master Clock in Hz
#define BAUDRATE 9600 // may be ... 1200, 2400, 4800, 9600, 19200, ...
#define UART_RX_INT_EN 0 // not use INT

// led 7seg pin
#define PDAT P1OUT
#define PROW P3OUT

// key button pin
#define K1 BIT0
#define K4 BIT3

// function prototype declaration
void initUART();
void uart_putc(char c);
void uart_puts(const char * s);
char uart_data_ready();
char uart_getc();
void uart_gets(char * s);

void delayms(int ms);
void scanled();
void toDisplay(int m);

// variable declaration
char chrin;
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
    0x01,
    0x02,
    0x03
};

char buffer[4] = {
    0,
    0,
    0,
    0
};
int idx = 0;

void main(void) {

    WDTCTL = WDTPW + WDTHOLD;

    P2DIR = 0xF0;
    P2REN = 0x09;
    P2OUT = 0x09;

    P1DIR = 0xFF;
    P3DIR = 0xFF;

    initUART();
    uart_puts("Chao cac ban !\n");

    int delay_time = 200;

    while (1) {

        // nhận phím ấn
        if ((P2IN & K1) == 0) {
            uart_puts("VDK MSP430g2553 !\n");
            delayms(delay_time);
        }
        if ((P2IN & K4) == 0) {
            uart_puts("TDH - CDT !\n");
            delayms(delay_time);
        }

        if (uart_data_ready()) {
            char c = uart_getc();
            toDisplay(c);
        }

        scanled();
        delayms(5);
    }
}

//---------------Thu vien UART------------------------
void initUART() {
    unsigned int tmp;
    // cau hinh bo dao dong DCO Fsc = 1MHZ
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    // cau hinh chan cho UART
    P3SEL |= BIT4 + BIT5;
    //P3SEL2   |= BIT4+BIT5;

    // khung du lieu
    UCA0CTL0 = 0x00; // 8 bit, none, 1 stop

    // Chon xung CLK
    UCA0CTL1 |= UCSSEL_2; // chon clk = SMCLK = 1MHZ  

    tmp = SMCLK_F / BAUDRATE;
    UCA0BR0 = tmp & 0x00FF;
    UCA0BR1 = ((tmp >> 8) & 0x00FF);

    UCA0MCTL = UCBRS_1; // cau hinh thu tu bit truyen--> MSB first

    UCA0CTL1 &= ~UCSWRST; // Start UART
    if (UART_RX_INT_EN) {
        IE2 |= UCA0RXIE;
        __bis_SR_register(GIE);
    }
}

void uart_putc(char c) {
    while (!(IFG2 & UCA0TXIFG)); // Wait until USCI_A0 TX buffer empty
    UCA0TXBUF = c; // assign character to TX buffer
}

void uart_puts(const char * s) {
    while ( * s != '\0') {
        uart_putc( * s);
        s++;
    }
}
char uart_getc() {
    while (!(IFG2 & UCA0RXIFG)); // Wait until USCI_A0 RX receive a complete character
    return UCA0RXBUF; // assign RX buffer to function 's name
}
void uart_gets(char * s) {
    * s = uart_getc();
    while ( * s != 0x0d) {
        s++;
        * s = uart_getc();
    }
    * s = 0;
}
char uart_data_ready() {
    if (IFG2 & UCA0RXIFG) return 1;
    else return 0;
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

    buffer[3] = value % 10; value /= 10;
    buffer[2] = value % 10; value /= 10;
    buffer[1] = value % 10; value /= 10;
    buffer[0] = value % 10;
}