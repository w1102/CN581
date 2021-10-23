#include "bmp180.h"
#include "msp430g2553.h"

#define PRESCALE 2
#define BMP180_ADDR 0x77
#define OSS 0

// eeprom data
uint16_t ac4, ac5, ac6;
int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;

// calculation data
int32_t x1, x2, x3, b3, b5, b6, p;
uint32_t b4, b7;
uint8_t msb, lsb, xlsb;


uint16_t uint_combine() {
    return (uint16_t) (msb << 8 | lsb);
}

int16_t int_combine() {
    return (int16_t) (msb << 8 | lsb);
}


void bmp_init() {
    P1SEL  |= 0xC0;                             // Assign I2C pins to USCI_B0
    P1SEL2 |= 0xC0;

    UCB0CTL1 = UCSWRST;                         // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;       // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;              // Use SMCLK, keep SW reset

    UCB0BR0 = PRESCALE;                               // 1Mhz/10 ~ 100khz
    UCB0BR1 = 0;

    UCB0I2CSA = BMP180_ADDR;                           // Set slave address
    UCB0CTL1 &= ~UCSWRST;                       // Clear SW reset, resume operation
    UCB0I2CIE = UCNACKIE;

}

uint8_t bmp_read_calib() {
    uint8_t data[22], length = 22;
    uint8_t firstAddr = 0xAA;
    

    while (UCB0CTL1 & UCTXSTP);             // Cho den khi tin hieu STT duoc gui xong
    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, Gui bit START

    while (!(IFG2&UCB0TXIFG));                  //  Cho cho bit START gui xong
    if(UCB0STAT & UCNACKIFG) return UCB0STAT;   //  Neu bao loi thì thoat khoi ham
    UCB0TXBUF = firstAddr;                      //  Dia chi luu gia tri sensor

    while (!(IFG2&UCB0TXIFG));                  //  Cho gui xong
    if(UCB0STAT & UCNACKIFG) return UCB0STAT;   //  Neu bao loi thì thoat khoi ham
    
    UCB0CTL1 &= ~UCTR;                      // I2C RX
    UCB0CTL1 |= UCTXSTT;                    // I2C RESTART
    IFG2 &= ~UCB0TXIFG;                     // Xoa co ngat USCI_B0 TX

    while (UCB0CTL1 & UCTXSTT);             // Cho den khi I2C STT duoc gui di

    uint8_t i;
    for(i = 0; i < (length - 1); i++) {
        while (!(IFG2&UCB0RXIFG));
        IFG2 &= ~UCB0TXIFG;                 // Clear USCI_B0 TX int flag
        data[i] = UCB0RXBUF;
    }

    while (!(IFG2&UCB0RXIFG));
    IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
    UCB0CTL1 |= UCTXSTP;                    // I2C stop condition after 1st TX
    data[length - 1] = UCB0RXBUF;
    IFG2 &= ~UCB0TXIFG;                       // Clear USCI_B0 TX int flag

    for(i = 0; i < length; i += 2) {
        msb = data[i];
        lsb = data[i + 1];
        
        switch (i >> 1) {
            case 0: ac1 = int_combine(); break;
            case 1: ac2 = int_combine(); break;
            case 2: ac3 = int_combine(); break;

            case 3: ac4 = uint_combine(); break;
            case 4: ac5 = uint_combine(); break;
            case 5: ac6 = uint_combine(); break;

            case 6:  b1 = int_combine(); break;
            case 7:  b2 = int_combine(); break;
            case 8:  mb = int_combine(); break;
            case 9:  mc = int_combine(); break;
            case 10: md = int_combine(); break;
        }
    }

    return 0;

}

int16_t bmp_read(uint8_t addr) {

    while (UCB0CTL1 & UCTXSTP);             // Cho den khi tin hieu STT duoc gui xong
    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, Gui bit START

    while (!(IFG2&UCB0TXIFG));                  //  Cho cho bit START gui xong
    if(UCB0STAT & UCNACKIFG) return UCB0STAT;   //  Neu bao loi thì thoat khoi ham
    UCB0TXBUF = addr;                           //  Dia chi luu gia tri sensor

    while (!(IFG2&UCB0TXIFG));                  //  Cho gui xong
    if(UCB0STAT & UCNACKIFG) return UCB0STAT;   //  Neu bao loi thì thoat khoi ham
    
    UCB0CTL1 &= ~UCTR;                      // I2C RX
    UCB0CTL1 |= UCTXSTT;                    // I2C RESTART
    IFG2 &= ~UCB0TXIFG;                     // Xoa co ngat USCI_B0 TX

    while (UCB0CTL1 & UCTXSTT);             // Cho den khi I2C STT duoc gui di

    while (!(IFG2&UCB0RXIFG));              //  MSB
    IFG2 &= ~UCB0TXIFG;                     //  Clear flag
    msb = UCB0RXBUF;

    while (!(IFG2&UCB0RXIFG));              //  LSB
    IFG2 &= ~UCB0TXIFG;                     //  Clear flag
    UCB0CTL1 |= UCTXSTP;                    //  I2C stop condition after 1st TX

    lsb = UCB0RXBUF;

    IFG2 &= ~UCB0TXIFG;                     //  Clear USCI_B0 TX int flag

    return msb << 8 | lsb;

}

uint8_t bmp_send(uint8_t addr, uint8_t data) {
    while (UCB0CTL1 & UCTXSTP);             // Cho den khi tin hieu STT duoc gui xong
    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, Gui bit START

    while (!(IFG2&UCB0TXIFG));                  //  Cho cho bit START gui xong
    if(UCB0STAT & UCNACKIFG) return UCB0STAT;   //  Neu bao loi thì thoat khoi ham
    UCB0TXBUF = addr;                           //  Gui dia chi thanh ghi can ghi

    

    while (!(IFG2&UCB0TXIFG));                  //  Cho gui xong
    if(UCB0STAT & UCNACKIFG) return UCB0STAT;   //  Neu bao loi thì thoat khoi ham
    UCB0TXBUF = data;                           // Gui du lieu

    while (!(IFG2&UCB0TXIFG));                  //  Cho gui xong
    if(UCB0STAT & UCNACKIFG) return UCB0STAT;   //  Neu bao loi thì thoat khoi ham
    UCB0CTL1 |= UCTXSTP;                        //  Gui bit STOP
    IFG2 &= ~UCB0TXIFG;                         //  Xoa co USCI_B0 TX
    return 0;
}

uint16_t bmp_read_ut() {
    
    bmp_send(0xF4, 0x2E);

    __delay_cycles(4500);

    return bmp_read(0xF6);
}

uint16_t bmp_calib_temperatue(uint16_t ut) {
    x1 = (((int32_t)ut - (int32_t)ac6)*(int32_t)ac5) >> 15;
    x2 = ((int32_t)mc << 11)/(x1 + md);  
    b5 = x1 + x2; 
    return (((b5 + 8)>>4));
}

int16_t bmp_read_up() {
    
    bmp_send(0xF4, 0x34);

    __delay_cycles(5000);

    while (UCB0CTL1 & UCTXSTP);             // Cho den khi tin hieu STT duoc gui xong
    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, Gui bit START

    while (!(IFG2&UCB0TXIFG));                  //  Cho cho bit START gui xong
    if(UCB0STAT & UCNACKIFG) return UCB0STAT;   //  Neu bao loi thì thoat khoi ham
    UCB0TXBUF = 0xF6;                           //  Dia chi luu gia tri sensor

    while (!(IFG2&UCB0TXIFG));                  //  Cho gui xong
    if(UCB0STAT & UCNACKIFG) return UCB0STAT;   //  Neu bao loi thì thoat khoi ham
    
    UCB0CTL1 &= ~UCTR;                      // I2C RX
    UCB0CTL1 |= UCTXSTT;                    // I2C RESTART
    IFG2 &= ~UCB0TXIFG;                     // Xoa co ngat USCI_B0 TX

    while (UCB0CTL1 & UCTXSTT);             // Cho den khi I2C STT duoc gui di

    while (!(IFG2&UCB0RXIFG));              //  MSB
    IFG2 &= ~UCB0TXIFG;                     //  Clear flag
    msb = UCB0RXBUF;

    while (!(IFG2&UCB0RXIFG));              //  LSB
    IFG2 &= ~UCB0TXIFG;                     //  Clear flag
    lsb = UCB0RXBUF;

    while (!(IFG2&UCB0RXIFG));              //  XLSB
    IFG2 &= ~UCB0TXIFG;                     //  Clear flag
    UCB0CTL1 |= UCTXSTP;                    //  I2C stop condition after 1st TX

    xlsb = UCB0RXBUF;

    IFG2 &= ~UCB0TXIFG;                     //  Clear USCI_B0 TX int flag

    return (((uint32_t) msb << 16) | ((uint32_t) lsb << 8) | (uint32_t) xlsb) >> (8-OSS);

    
}

int32_t bmp_calib_pressure(int16_t up) {

    b6 = b5 - 4000;
    // Calculate B3
    x1 = (b2 * (b6 * b6)>>12)>>11;
    x2 = (ac2 * b6)>>11;
    x3 = x1 + x2;
    b3 = ((((ac1)*4 + x3)<<OSS) + 2)>>2;

    // Calculate B4
    x1 = (ac3 * b6)>>13;
    x2 = (b1 * ((b6 * b6)>>12))>>16;
    x3 = ((x1 + x2) + 2)>>2;
    b4 = (ac4 * (uint32_t)(x3 + 32768))>>15;

    b7 = ((uint32_t)(up - b3) * (50000>>OSS));

    if (b7 < 0x80000000)
        p = (b7<<1)/b4;
    else
        p = (b7/b4)<<1;   

    x1 = (p>>8) * (p>>8);
    x1 = (x1 * 3038)>>16;
    x2 = (-7357 * p)>>16;
    p += (x1 + x2 + 3791)>>4;
  
    return p;

}