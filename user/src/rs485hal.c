#include "rs485.h"

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
//#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"


unsigned char deviceRxIndicator;
extern unsigned char errorDetected;

#define RX_WAIT_INTERVAL 3
#define RS485_INTERFACE_BITRATE 115200

unsigned char deviceTransmitInProgress=0;

unsigned char deviceInterfaceRxPointer;

unsigned char i2cResetFlag=0;

extern unsigned char rs485event;


unsigned char deviceRxBuffer[RX_485_BUFFER_SIZE];
unsigned char deviceTxBuffer[TX_485_BUFFER_SIZE];
extern unsigned char i2cAlive;

USART_InitTypeDef usinit;
DMA_InitTypeDef   dmaInitStr;
TIM_TimeBaseInitTypeDef	base_timer;
RCC_ClocksTypeDef rccClocks;



void rs485conf(void)
{


usinit.USART_BaudRate=RS485_INTERFACE_BITRATE;
usinit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
usinit.USART_Mode=USART_Mode_Rx|USART_Mode_Tx ;
usinit.USART_Parity=USART_Parity_No;
usinit.USART_StopBits =USART_StopBits_1 ;
usinit.USART_WordLength=USART_WordLength_8b;
USART_Init(USART3,&usinit);



dmaInitStr.DMA_BufferSize=4;
dmaInitStr.DMA_DIR=DMA_DIR_PeripheralDST;
dmaInitStr.DMA_M2M=DMA_M2M_Disable; 
dmaInitStr.DMA_MemoryBaseAddr=(u32)deviceTxBuffer;
dmaInitStr.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
dmaInitStr.DMA_MemoryInc=DMA_MemoryInc_Enable;
dmaInitStr.DMA_Mode=DMA_Mode_Normal; 
dmaInitStr.DMA_PeripheralBaseAddr=(uint32_t) &(USART3->DR);
dmaInitStr.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;  
dmaInitStr.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
dmaInitStr.DMA_Priority=DMA_Priority_Medium; 



DMA_Init(DMA1_Channel2,&dmaInitStr);



USART_Cmd (USART3,ENABLE);
USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
USART_ITConfig(USART3,USART_IT_RXNE,ENABLE); 
NVIC_EnableIRQ(USART3_IRQn);
DMA_SetCurrDataCounter(DMA1_Channel2,0); 

DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);
NVIC_EnableIRQ(DMA1_Channel2_IRQn);

RCC_GetClocksFreq(&rccClocks);
 

TIM_TimeBaseStructInit(&base_timer);
base_timer.TIM_Prescaler =0; 
//base_timer.TIM_Period =rccClocks.PCLK1_Frequency/20000;
//TIM_TimeBaseInit(TIM15, &base_timer);
//TIM_ITConfig(TIM15,TIM_IT_Update,ENABLE);
//NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
//TIM_Cmd(TIM15, ENABLE);


deviceRxIndicator=0;

deviceInterfaceRxPointer=0;

}



void sendDeviceInterface(unsigned char num)
{

if(DMA_GetCurrDataCounter(DMA1_Channel2) ==0){
GPIO_WriteBit(GPIOB,GPIO_Pin_2,Bit_SET );

DMA_SetCurrDataCounter(DMA1_Channel2,num);
deviceTransmitInProgress=1; 

DMA_Cmd(DMA1_Channel2,ENABLE); }
}




void clearDeviceRxBuffer(void){deviceInterfaceRxPointer=0;deviceRxBuffer[0]=0;deviceRxBuffer[4]=0;}










void USART3_IRQHandler(void)
{
unsigned short tmp;
tmp=USART3->SR;
deviceRxBuffer[deviceInterfaceRxPointer]=USART3->DR;
if(deviceInterfaceRxPointer<(RX_485_BUFFER_SIZE-1) )deviceInterfaceRxPointer++;
deviceRxIndicator=RX_WAIT_INTERVAL;
}





 void DMA1_Channel2_IRQHandler(void)
{ 

DMA_Cmd(DMA1_Channel2,DISABLE);
DMA_ClearITPendingBit (DMA1_IT_TC2);

}

void rs485periodicHandle(void)
{
unsigned short tmp;
tmp=USART3->SR;

if(tmp&USART_SR_TC){deviceTransmitInProgress=0;GPIO_WriteBit(GPIOB,GPIO_Pin_2,Bit_RESET ); USART3->SR=tmp&~USART_SR_TC; }

if(deviceRxIndicator)
{
deviceRxIndicator--;
if(deviceRxIndicator==0)rs485event|=EVENT_DEVICE_INTERFACE_DATA_RECIEVED;

}



}

//TIM_ClearITPendingBit(TIM4,TIM_IT_Update); 

