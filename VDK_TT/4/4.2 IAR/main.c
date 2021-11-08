#include <msp430f2112.h>

#include <stdio.h>

#include <stdlib.h>


#define SMCLK_F 1000000 // frequency of Sub-System Master Clock in Hz
#define BAUDRATE 9600 // may be ... 1200, 2400, 4800, 9600, 19200, ...
#define UART_RX_INT_EN 0 // cau hinh nhan du lieu: 0 - khong dùng INT; 1 - dùng INT

//----for key------------
#define K1 BIT0
#define K4 BIT3

//------------- Khai báo prototype hàm
void initUART();
void uart_putc(char c);
void uart_puts(const char * s);
char uart_data_ready();
char uart_getc();
void uart_gets(char * s);
void delayms(int ms);

//-------Khai báo biến-----------------
char chrin;

//---Định nghĩa chương trình chính-------
void main(void) {

    WDTCTL = WDTPW + WDTHOLD;

    P2DIR = 0xF0;
    P2REN = 0x09;
    P2OUT = 0x09;

    P1DIR = 0xFF;

    initUART(); // khởi động UART
    uart_putc('@');

    delayms(1000);

    // Vòng lặp chương trình chính
    while (1) {

        // nhận phím ấn
        if ((P2IN & K1) == 0) uart_putc('1');
        if ((P2IN & K4) == 0) uart_putc('4');

        //kiểm tra và xử lý ký tự nhận được từ PC
        if (uart_data_ready()) {
            char kytu = uart_getc();
            if (kytu != 0) {
                // nhận được ký tự và đưa ra LED đơn
                P1OUT = kytu;
            }
        }

        delayms(20);
    } // end while
} // end main

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

    // thiet lap toc do baud 
    //UCA0BR0  = 104; // 9600
    //UCA0BR1  = 0;
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
//==============Xu ly ngat--UART-------------------------
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    // chrin = uart_getc();
    // switch(chrin){
    //   case '1':  P3OUT |=LEDR;
    //              break;   
    //   case '2':  P3OUT &=~LEDR;
    //              break;                    
    // }
}

//------------------------------------
void uart_putc(char c) {
    while (!(IFG2 & UCA0TXIFG)); // Wait until USCI_A0 TX buffer empty
    UCA0TXBUF = c; // assign character to TX buffer
}
//------------------------------------
void uart_puts(const char * s) {
    while ( * s != '\0') {
        uart_putc( * s);
        s++;
    }
}
//------------------------------------
char uart_getc() {
    while (!(IFG2 & UCA0RXIFG)); // Wait until USCI_A0 RX receive a complete character
    return UCA0RXBUF; // assign RX buffer to function 's name
}
//------------------------------------
void uart_gets(char * s) {
    //  if (IFG2&UCA0RXIFG){
    * s = uart_getc();
    while ( * s != 0x0d) {
        s++;
        * s = uart_getc();
    }
    * s = 0;
    //  }
}
//------------------------------------
char uart_data_ready() {
    if (IFG2 & UCA0RXIFG) return 1;
    else return 0;
}
//----------------------------
void delayms(int ms) {
    int i;
    for (i = 0; i < ms; i++)
        __delay_cycles(100);
}