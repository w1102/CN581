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

unsigned int ReadADC10(int chanel);

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
char str[50];
void main(void) {

    WDTCTL = WDTPW + WDTHOLD;

    P2DIR = 0xF0;
    P2REN = 0x09;
    P2OUT = 0x09;

    P1DIR = 0xFF;
    P3DIR = 0xFF;

    
    // Khởi động ADC
    ADC10CTL1 = ADC10DIV_3;
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON;

    initUART();
    uart_puts("Chao cac ban !\n");

   
    scanled();

    int value = 0;

    while (1) {

         value = ((ReadADC10(10) - 673) * 423) / 1024;
 

         
         sprintf(str, "Gia tri cua ADC = %d\n", value);

        uart_puts(str);
        
        delayms(2000);
        
    }
}

//================================= uart ======================================
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


// ===================== adc ==================
unsigned int ReadADC10(int chanel) {
    ADC10AE0 |= (BIT0<<chanel);
    ADC10CTL1 |= (chanel*0x1000u);
    ADC10CTL0 |= ENC + ADC10SC;
    while(!(ADC10CTL0 & ADC10IFG));
    return ADC10MEM;
}

//====================================================
void delayms(int ms) {
    int i;
    for (i = 0; i < ms; i++)
        __delay_cycles(1000);
}
//====================================================

void scanled() {

    PDAT = code7seg[buffer[idx]];
    PROW |= code74HC[idx];

    __delay_cycles(1000);
    PDAT = 0xFF;
    PROW &= ~code74HC[idx];

    idx = idx == 3 ? 0 : idx + 1;

}
//====================================================

void toDisplay(int value) {

    buffer[3] = value % 10; value /= 10;
    buffer[2] = value % 10; value /= 10;
    buffer[1] = value % 10; value /= 10;
    buffer[0] = value % 10;
}