#include "msp430g2553.h"
#include "stdint.h"

const uint8_t led7Seg[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0xC7, 0x88, 0xA1};

#define DIG_L 10
#define DIG_A 11
#define DIG_D 12

void led7seg_disp_load(uint8_t *buffer) {
    buffer[0] = DIG_L;
    buffer[1] = 0;
    buffer[2] = DIG_A;
    buffer[3] = DIG_D;
}

void led7seg_init_timer() {
  //Set MCLK = SMCLK = 1MHz
  DCOCTL  = 0;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL  = CALDCO_1MHZ;

  TA0CTL = TASSEL_2 + ID_0  + MC_1; // SMCLK as clock source, divider 0, up-mode

  CCTL0 = CCIE;     // Enable interrupt for CCR0
  CCR0  = 5000;     // 5ms
  _BIS_SR(GIE);     // Enable global interrupts
}