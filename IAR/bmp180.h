#ifndef BMP180
#define BMP180

#include <stdint.h>

void bmp_init();

uint8_t bmp_read_calib();

int16_t bmp_read(uint8_t addr);
uint8_t bmp_send(uint8_t addr, uint8_t data);


uint16_t bmp_read_ut();
uint16_t bmp_calib_temperatue(uint16_t ut);


int16_t bmp_read_up();
int32_t bmp_calib_pressure(int16_t up);




#endif