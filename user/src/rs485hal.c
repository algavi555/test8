

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"




#define RS485_INTERFACE_BITRATE 115200

USART_InitTypeDef usinit;


unsigned char tempdata;

void rs485conf(void)
{


usinit.USART_BaudRate=RS485_INTERFACE_BITRATE;
usinit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
usinit.USART_Mode=USART_Mode_Rx|USART_Mode_Tx ;
usinit.USART_Parity=USART_Parity_No;
usinit.USART_StopBits =USART_StopBits_1 ;
usinit.USART_WordLength=USART_WordLength_8b;
USART_Init(USART2,&usinit);





USART_ITConfig(USART2,USART_IT_RXNE,ENABLE); 
USART_ITConfig(USART2,USART_IT_TXE,ENABLE); 
NVIC_EnableIRQ(USART2_IRQn);
USART_Cmd (USART2,ENABLE);


 

}





void USART2_IRQHandler(void)
{
unsigned short tmp;
tmp=USART2->SR;

if(tmp&USART_SR_TXE)
{
USART2->DR=0x31;
}

if(tmp&USART_SR_RXNE)
{
tempdata=USART2->DR;
GPIOE->BSRR=GPIO_Pin_8;
}

}





