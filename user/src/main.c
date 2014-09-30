
#include "main_inc.h"
#include "rs485.h"
#include "string.h"
#define FUNC_READ  10
#define FUNC_WRITE 30

extern unsigned char rs485event;
extern unsigned char r485Active;

extern char spiHandlerFlag;

struct function485 func;

extern short motor_power1;
extern short motor_power2;

extern signed short acmiddle[];
extern unsigned short encold[2];

unsigned	char data_buffer[15]={0};
unsigned	char rx_buffer[20];
extern unsigned short encAngl[];


unsigned char i2cRresetTimeout=2, i2cResetCounter=0;

extern unsigned char i2cToverFlag;
unsigned short enc1,enc2;

unsigned short tmp;
const unsigned short * const addrPointer = (const unsigned short * const) 0x800FFF0;

extern unsigned char i2cToverStatus;

char motor_handler_flag;
char i2c_handler_flag;

signed short motionVals[5];


unsigned char main_handler_flag;
unsigned char errorDetected;
unsigned short blinkPeriod=5,blinkCounter;
extern unsigned char i2cResetFlag;
   char firstTime=2;
  void motor_handler(void);


 void main_handler(void)
 {

 // if(firstTime){firstTime--;  I2C_Transmit(0x18,0x20,1);}



memcpy(data_buffer,encAngl,4);
memcpy(data_buffer+4,motionVals,6);

//data_buffer[7]=((GPIOB->IDR)>>3)&3 ;

	   /*
if(i2cToverStatus==151)
{
i2cResetCounter++;
if(i2cResetCounter>i2cRresetTimeout)
{
i2cResetCounter=0;
i2cToverStatus=	155;
GPIOB->CRL|=GPIO_CRL_CNF7|GPIO_CRL_CNF6_1;
}		   	
 }		*/

 blinkCounter++;
 if(blinkCounter>blinkPeriod)
 {
 blinkCounter=0;

if(GPIOB->ODR&GPIO_Pin_13)
{
if(r485Active){GPIOB->BRR=GPIO_Pin_13;r485Active=0; }
}
else GPIOB->BSRR=GPIO_Pin_13;
}

 if( errorDetected)
 {
errorDetected=0;
//GPIOB->BSRR=GPIO_Pin_12;
 }
 //else GPIOB->BRR=GPIO_Pin_12;


 
 }

int main(void)
{

baseHwConfig();
rs485conf();
i2cConfig();
func.id=10;//(*addrPointer)&0xff; //10
func.size=11;

func.data=data_buffer;
rs485RegisterFunction(&func);


func.id=30;
func.size=7;
func.data=rx_buffer;
rs485RegisterFunction(&func);


while(1)
{	/*
if(TIM1->CNT>HI1)TIM1->CNT=HI1;
  if(TIM1->CNT<LO1)TIM1->CNT=LO1;
  if(TIM4->CNT>HI2)TIM4->CNT=HI2;
  if(TIM4->CNT<LO2)TIM4->CNT=LO2;
  */


if(i2c_handler_flag){i2c_handler_flag=0; i2cEncHandler(); }

if(motor_handler_flag)
	{ 

	motor_handler(); 
	motor_handler_flag=0;
	}
if(main_handler_flag){	main_handler(); 	main_handler_flag=0;}


if(spiHandlerFlag){spiEncHandler();spiHandlerFlag=0; }

if(rs485event)rs485handler();
if(i2cToverFlag)i2cTransmissionOverHandler();
if(i2cResetFlag){
	errorDetected|=8;
	/* GPIOB->CRL&=~(GPIO_CRL_CNF6|GPIO_CRL_CNF7); GPIOB->BRR=GPIO_Pin_7|GPIO_Pin_6;  */
	i2cConfig();
	i2cResetFlag=0;}
}

}

