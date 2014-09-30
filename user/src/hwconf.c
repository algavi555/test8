#include"main_inc.h"
GPIO_InitTypeDef  gp1;
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	 
extern USART_InitTypeDef usinit;

void Setup_PWM(void);
void enc_config(void);
void spi_conf(void);
void adc_conf(void);
void usart_conf(void);
void baseHwConfig(void)
{
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_TIM4|RCC_APB1Periph_I2C1,ENABLE); 
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO| RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|
RCC_APB2Periph_GPIOD |RCC_APB2Periph_ADC1,ENABLE);

GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
GPIO_PinRemapConfig (GPIO_Remap_SPI1 ,ENABLE);
gp1.GPIO_Speed=GPIO_Speed_10MHz ;		
gp1.GPIO_Mode=GPIO_Mode_AF_PP; 
gp1.GPIO_Pin=GPIO_Pin_10;				//usart3 TX
GPIO_Init (GPIOB,&gp1); 

gp1.GPIO_Mode=GPIO_Mode_IPU; 
gp1.GPIO_Pin=GPIO_Pin_11;				//usart3 RX
GPIO_Init (GPIOB,&gp1); 


gp1.GPIO_Mode=GPIO_Mode_Out_PP; 
gp1.GPIO_Pin=GPIO_Pin_2;			   //485 OE
GPIO_Init (GPIOB,&gp1); 


gp1.GPIO_Mode=GPIO_Mode_AF_PP; 
gp1.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_6;				//TIM2 PWM OUT   //PRM5  /TIM3 PWM OUT PWM3
GPIO_Init (GPIOA,&gp1); 


gp1.GPIO_Mode=GPIO_Mode_Out_PP; 
gp1.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_1;				//PWM4/PWM2
GPIO_Init (GPIOA,&gp1); 


gp1.GPIO_Mode=GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;// 
gp1.GPIO_Pin=GPIO_Pin_0;				//TIM3 PWM OUT PWM1
GPIO_Init (GPIOB,&gp1); 


gp1.GPIO_Mode=GPIO_Mode_Out_PP; 
gp1.GPIO_Pin=GPIO_Pin_1;				//PWM2
GPIO_Init (GPIOB,&gp1); 


gp1.GPIO_Mode=GPIO_Mode_Out_PP; 
gp1.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				//CS2/M7/M8
GPIO_Init (GPIOC,&gp1); 


gp1.GPIO_Mode=GPIO_Mode_Out_PP; 
gp1.GPIO_Pin=GPIO_Pin_9;				//CS1
GPIO_Init (GPIOB,&gp1); 

gp1.GPIO_Mode=GPIO_Mode_IPU; 
gp1.GPIO_Pin=GPIO_Pin_8;				//Button UP
GPIO_Init (GPIOB,&gp1); 




gp1.GPIO_Mode=GPIO_Mode_AF_PP; 
gp1.GPIO_Pin=GPIO_Pin_3;				//SCK
GPIO_Init (GPIOB,&gp1); 

gp1.GPIO_Mode=GPIO_Mode_IPU; 
gp1.GPIO_Pin=GPIO_Pin_4;				//MISO
GPIO_Init (GPIOB,&gp1); 


gp1.GPIO_Mode=GPIO_Mode_AF_OD ; 
gp1.GPIO_Pin=GPIO_Pin_7;				//i2c	SDA
GPIO_Init (GPIOB,&gp1); 

gp1.GPIO_Mode=GPIO_Mode_AF_PP ; 
gp1.GPIO_Pin=GPIO_Pin_6;				//i2c SCL
GPIO_Init (GPIOB,&gp1);





 gp1.GPIO_Mode=GPIO_Mode_Out_PP; 				
gp1.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13;				//LED
GPIO_Init (GPIOB,&gp1); 

 gp1.GPIO_Mode=GPIO_Mode_IPU; 				
gp1.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;				//Buttons
GPIO_Init (GPIOA,&gp1);

gp1.GPIO_Mode=GPIO_Mode_AIN; 
gp1.GPIO_Pin=GPIO_Pin_5;				//ADC_IN
GPIO_Init (GPIOA,&gp1); 
  

gp1.GPIO_Mode=GPIO_Mode_IPU; 
gp1.GPIO_Pin=GPIO_Pin_3;				//USART2 RX
GPIO_Init (GPIOA,&gp1); 


 Setup_PWM();
 //enc_config();
 spi_conf();
 //adc_conf();
 GPIOB->BSRR=GPIO_Pin_13;
 //usart_conf();
}

void Setup_PWM(void)
{
        

          RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4, ENABLE);
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
           /* Time Base configuration */
          TIM_TimeBaseStructure.TIM_Prescaler = 3;
          TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
          TIM_TimeBaseStructure.TIM_Period =1200;
          TIM_TimeBaseStructure.TIM_ClockDivision = 0;
          TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

          TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		   TIM_TimeBaseStructure.TIM_Prescaler = 3;
          TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
         

          /* Channel 1, 2,3 and 4 Configuration in PWM mode */
          TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
          TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
          TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
          TIM_OCInitStructure.TIM_Pulse = 0;
          TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
          TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
          TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
          TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

          TIM_OC1Init(TIM2, &TIM_OCInitStructure);	 
          TIM_OC2Init(TIM2, &TIM_OCInitStructure);	 
		  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 
		  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 

	      TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
 	      NVIC_EnableIRQ(TIM2_IRQn);
      
          TIM_Cmd(TIM2, ENABLE);
		  TIM_Cmd(TIM3, ENABLE);

    
          TIM_CtrlPWMOutputs(TIM2, ENABLE); 
		  TIM_CtrlPWMOutputs(TIM3, ENABLE); 
}

void enc_config(void)
{

 TIM_TimeBaseStructure.TIM_Prescaler = 0;
          TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
          TIM_TimeBaseStructure.TIM_Period =16000;
          TIM_TimeBaseStructure.TIM_ClockDivision = 0;
          TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

          TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
          TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		 TIM_EncoderInterfaceConfig (TIM4,TIM_EncoderMode_TI1, TIM_ICPolarity_Rising,TIM_ICPolarity_Falling);
		 TIM_EncoderInterfaceConfig (TIM1,TIM_EncoderMode_TI1, TIM_ICPolarity_Rising,TIM_ICPolarity_Falling);
		 TIM_Cmd(TIM4,ENABLE);
		 TIM_Cmd(TIM1,ENABLE);
}

void usart_conf(void )
{  
usinit.USART_BaudRate=600;
usinit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
usinit.USART_Mode=USART_Mode_Rx ;
usinit.USART_Parity=USART_Parity_No;
usinit.USART_StopBits =USART_StopBits_1 ;
usinit.USART_WordLength=USART_WordLength_8b;
USART_Init(USART2,&usinit);
USART_Cmd (USART2,ENABLE);
USART_ITConfig(USART2,USART_IT_RXNE,ENABLE); 
NVIC_EnableIRQ(USART2_IRQn);

}
void spi_conf(void)
{
 SPI_InitTypeDef  spinit;
  

 RCC_APB2PeriphClockCmd (RCC_APB2Periph_SPI1,ENABLE);


   spinit.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256 ;
   spinit.SPI_CPHA=SPI_CPHA_1Edge;
   spinit.SPI_CPOL=SPI_CPOL_High;
   spinit.SPI_DataSize=SPI_DataSize_8b;
   //spinit.SPI_Direction=SPI_Direction_2Lines_RxOnly  ;	 
   spinit.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
   spinit.SPI_FirstBit=SPI_FirstBit_MSB;
   spinit.SPI_Mode=SPI_Mode_Master;
   spinit.SPI_NSS=SPI_NSS_Soft;
   


//	for(i=0;i<10;i++){encSpiOldValues[i]=512;encRevolutions[i]=5;}


   SPI_Init(SPI1,&spinit);

   SPI_Cmd (SPI1,ENABLE); 
   SPI_I2S_ITConfig (SPI1,SPI_I2S_IT_RXNE,ENABLE);  
   NVIC_EnableIRQ(SPI1_IRQn);
   SPI1->DR=0xff;










}
void adc_conf(void)
{

int i;
ADC_InitTypeDef ADC_InitStructure;
//DMA_InitTypeDef  dmaInitStr;

RCC_APB2PeriphClockCmd ( RCC_APB2Periph_ADC1,ENABLE);


ADC_ResetCalibration(ADC1);
i=3000;
while (ADC_GetResetCalibrationStatus(ADC1)&i--);
ADC_StartCalibration(ADC1);


ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
ADC_InitStructure.ADC_ScanConvMode=ENABLE;
ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;
ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
ADC_InitStructure.ADC_NbrOfChannel=1; 




while (ADC_GetCalibrationStatus(ADC1));



ADC_Init(ADC1, &ADC_InitStructure);
ADC_ExternalTrigConvCmd(ADC1,DISABLE);
ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_239Cycles5);
//ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5);
//ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5);
//ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_239Cycles5);
//ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 5, ADC_SampleTime_239Cycles5);

ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
//NVIC_EnableIRQ(ADC1_2_IRQn);


ADC_Cmd(ADC1,ENABLE);
ADC_SoftwareStartConvCmd(ADC1,ENABLE);

}


