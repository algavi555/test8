#ifndef __MAIN_INC__H
#define __MAIN_INC__H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_i2c.h"
#include "misc.h"

#define HI1 6257
#define LO1 1000
#define HI2 9000
#define LO2 1360

#define MODE_DIRECT_SPEED_CONTROL
#define MODE_OFF
#define MODE_POSITION_CONTROL

void baseHwConfig(void);
void rs485conf(void);
void rs485handler2(void);
void rs485periodicHandle(void);
void i2cTransmissionOverHandler(void);
void I2C_Recieve(unsigned char sla, unsigned char reg, unsigned char number);
void I2C_Transmit(unsigned char sla, unsigned char reg, unsigned char number);
void i2cConfig(void);
void i2cEncHandler(void);
#endif
