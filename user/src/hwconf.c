#include"main_inc.h"
GPIO_InitTypeDef  gp1;

void baseHwConfig(void)
{
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 

RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO| RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|
RCC_APB2Periph_GPIOD |RCC_APB2Periph_GPIOE,ENABLE);

GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);

gp1.GPIO_Speed=GPIO_Speed_10MHz ;		

gp1.GPIO_Mode=GPIO_Mode_AF_PP; 
gp1.GPIO_Pin=GPIO_Pin_5;				//usart2 TX
GPIO_Init (GPIOD,&gp1); 

gp1.GPIO_Mode=GPIO_Mode_IPU; 
gp1.GPIO_Pin=GPIO_Pin_6;				//usart2 RX
GPIO_Init (GPIOD,&gp1); 



gp1.GPIO_Mode=GPIO_Mode_Out_PP; 				
gp1.GPIO_Pin=GPIO_Pin_8;				//LED
GPIO_Init (GPIOE,&gp1); 

GPIOE->BRR=GPIO_Pin_8;

}

