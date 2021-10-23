#include "msp430g2553.h"

#include <stdint.h>
#include <stdio.h>

#include "bmp180.h"
#include "led7seg.h"

#define PW_KEY BIT5

// display digit variable
uint8_t idx = 0, buff[4] = {0, 0, 0, 0};

// display dot variable
volatile int display_dot = 0, dot_idx = 0; char tmp[6];
volatile int mode = 0;

void delay_ms(uint16_t);

void set_buffer(float);
void set_buffer_from_int(int32_t);
void set_buffer_from_float(float);


void main(void) {

    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    // Set DCO clock 1Mhz and config timer A
    led7seg_init_timer();

    // Set pin
    P2SEL &= ~0xC0; P2DIR = 0xFF;
    P1DIR = 0x0F;

    // Config interrupt for P1.5
    P1REN |= PW_KEY; // enable pull-up resistor
    P1OUT |= PW_KEY; // pull_up resistor
    P1IE  |= PW_KEY; // enable interrupt
    P1IES |= PW_KEY; // rising edge interrupt

    led7seg_disp_load(buff);

    bmp_init();
    bmp_read_calib();


    int32_t ut, up, air_pressure, water_pressure;
    float water_level;

    ut = bmp_read_ut();
    bmp_calib_temperatue(ut);

    up = bmp_read_up();
    air_pressure = bmp_calib_pressure(up);

    set_buffer( air_pressure / 1000.0 );

    for(;;) {

        if (mode == 0) {
            up = bmp_read_up();
            air_pressure = bmp_calib_pressure(up);

            set_buffer( (float)air_pressure / 1000.0 );

        }
        else {
            up = bmp_read_up();
            water_pressure = bmp_calib_pressure(up);

            water_level = (float)(water_pressure - air_pressure) / 10.0;

            set_buffer(water_level);
            

        }
    }


}


// Timer A0 interrupt service routine, led scaner
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0(void) {

    // display row digit
    P2OUT = led7Seg[ buff[idx] ];

    // display water level mode dot
    if(mode == 1 && idx == 3)
        P2OUT &= ~BIT7;

    // display float dot
    if(display_dot && idx == dot_idx)
        P2OUT &= ~BIT7;

    // display column digit
    P1OUT |= 0x01 << idx;
    idx = idx == 0 ? 3 : idx - 1;

    // turn off all led after 500us to avoid ghosting effect
    __delay_cycles(500);
    P1OUT &= 0xF0;

}

// Port 1 external interrupt, toggle mode
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) {
    if (P1IFG & PW_KEY) {
        mode = mode == 1 ? 0 : 1;
    }

    P1IFG &= ~PW_KEY;

}

void delay_ms(uint16_t ms) {

    uint16_t i;
    for(i = 0; i < ms; i++)
        __delay_cycles(1000);
}

void set_buffer_from_int(int32_t value) {
    display_dot = 0;
    
    buff[3] = value % 10; value /= 10;
    buff[2] = value % 10; value /= 10;
    buff[1] = value % 10; value /= 10;
    buff[0] = value % 10;
}

void set_buffer_from_float(float value) {
    display_dot = 1;

    if (value < 100) {
        sprintf(tmp, "%.2f", value);
        dot_idx = 1;

        buff[0] = tmp[0] - '0';
        buff[1] = tmp[1] - '0';
        buff[2] = tmp[2] - '0';
        buff[3] = tmp[3] - '0';
    }
    else {
        sprintf(tmp, "%.1f", value);
        dot_idx = 2;

        buff[0] = tmp[0] - '0';
        buff[1] = tmp[1] - '0';
        buff[2] = tmp[2] - '0';
        buff[3] = tmp[3] - '0';
    }
}

void set_buffer(float f) {
    if (f == (int32_t)f)
        set_buffer_from_int( (int32_t)f );
    else
        set_buffer_from_float(f);
}