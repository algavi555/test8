#include "main_inc.h"
#include "i2c.h"
#include "string.h"
I2C_InitTypeDef  i2init; 

#define   I2C_AlIVE_INTERVAL 200

extern unsigned char errorDetected;
extern char firstTime;
signed short acmiddle[5];
signed short actmp;
signed long int acsum[5]={0};
char acCount=0; 


unsigned char  sladdr;
unsigned char  regaddr;
unsigned char  i2cRw;
unsigned char toTransmit;
unsigned char transmited;
unsigned short cr1val;

unsigned char i2cBuff[10]={0x27,0};

unsigned char enclist[]={0x43,0x41,0x40};
//unsigned char encnum=0;
unsigned char encmax=2;

//unsigned short encAngl[5];

unsigned short encTmp1,encTmp2,encOld1,encOld2;
unsigned short encRotations1, encRotations2;

unsigned int angmiddle[5];
unsigned char angmiddlecount[5];

unsigned char i2cToverFlag;
unsigned char i2cToverStatus;
unsigned char registerNotSent;

unsigned char i2cAlive;

unsigned short tmp1;
unsigned short tmp2;

void i2cConfig(void)
{
//RCC_APB1PeriphResetCmd (RCC_APB1Periph_I2C1,ENABLE);
//RCC_APB1PeriphClockCmd (RCC_APB1Periph_I2C1,ENABLE);

I2C_DeInit(I2C1);

i2init.I2C_Ack=I2C_Ack_Enable; 
i2init.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;   
i2init.I2C_DutyCycle=I2C_DutyCycle_2; 
i2init.I2C_ClockSpeed=90000; 
i2init.I2C_Mode=I2C_Mode_I2C; 
i2init.I2C_OwnAddress1=120; 
I2C_Init(I2C1,&i2init);
I2C_ITConfig(I2C1,I2C_IT_EVT|I2C_IT_BUF|I2C_IT_ERR,ENABLE);
NVIC_EnableIRQ(I2C1_EV_IRQn);
NVIC_EnableIRQ(I2C1_ER_IRQn);
I2C_Cmd(I2C1,ENABLE);
i2cAlive=0;
i2cToverStatus=I2C_STATUS_RESET;
cr1val=I2C1->CR1;
firstTime=2;
}





void I2C1_EV_IRQHandler(void)
{
i2cAlive=I2C_AlIVE_INTERVAL;
tmp1=I2C1->SR1;

if(tmp1&I2C_SR1_RXNE){if(transmited+2==(toTransmit)){I2C1->CR1 &= ~I2C_CR1_ACK; I2C1->CR1 |= I2C_CR1_STOP; }i2cBuff[transmited]=I2C1->DR; transmited++;  if(transmited==toTransmit){i2cToverStatus=I2C_STATUS_TRANSMISSION_OK;i2cAlive=0;i2cToverFlag=1;}  }
if(tmp1&I2C_SR1_STOPF){I2C1->CR1=cr1val;}
if(tmp1&I2C_SR1_SB){ if(registerNotSent)I2C1->DR=sladdr<<1;else I2C1->DR=sladdr<<1|i2cRw; i2cToverStatus=I2C_STATUS_ADDR_SENT;}
if(tmp1&I2C_SR1_ADDR){tmp2=I2C1->SR2; I2C1->CR2|=I2C_CR2_ITBUFEN; i2cToverStatus=I2C_STATUS_ADDR_ACK_RECIEVED;  if(i2cRw) {if( (toTransmit==1)){I2C1->CR1 &= ~I2C_CR1_ACK; I2C1->CR1 |= I2C_CR1_STOP;  }else I2C1->CR1 |= I2C_CR1_ACK;  } }
if(tmp1&I2C_SR1_TXE){  if(registerNotSent){registerNotSent=0; I2C1->DR=regaddr; if(i2cRw) {I2C1->CR1 |= I2C_CR1_START; I2C1->CR2&=~I2C_CR2_ITBUFEN;  I2C1->DR=0xff;} }else
{
	I2C1->DR=i2cBuff[transmited];
 if(transmited<toTransmit){transmited++; }else{ I2C1->CR1 |= I2C_CR1_STOP; i2cToverStatus=I2C_STATUS_TRANSMISSION_OK;i2cAlive=0;i2cToverFlag=1; I2C2->CR1&=~I2C_CR2_ITBUFEN;}  }  }

}



void I2C1_ER_IRQHandler(void)
{
unsigned short tmp;
i2cAlive=0;
tmp=I2C1->SR1;
errorDetected|=2;
if(tmp&I2C_SR1_AF){ if(i2cToverStatus==I2C_STATUS_START_SENT)i2cToverStatus=I2C_STATUS_NO_SLAVE;else i2cToverStatus=I2C_STATUS_ERROR_DURING_TRANSMISSION; I2C1->CR1 |= I2C_CR1_STOP;  i2cToverFlag=1; I2C_ClearITPendingBit (I2C1,I2C_IT_AF);   }
if(tmp&I2C_SR1_OVR){i2cToverStatus=I2C_STATUS_ERROR_DURING_TRANSMISSION;I2C1->CR1 |= I2C_CR1_STOP;  i2cToverFlag=1;I2C_ClearITPendingBit (I2C1,I2C_IT_OVR);}
if(tmp&I2C_SR1_BERR){i2cToverStatus=I2C_STATUS_BUS_ERROR;i2cToverFlag=1; I2C1->CR1 |= I2C_CR1_STOP; I2C_ClearITPendingBit (I2C1,I2C_IT_BERR);}
if(tmp&I2C_SR1_ARLO){i2cToverStatus=I2C_STATUS_BUS_ERROR;i2cToverFlag=1;  I2C_ClearITPendingBit (I2C1,I2C_IT_ARLO);  }
}

void I2C_Transmit(unsigned char sla, unsigned char reg, unsigned char number)
{

if( (number>0)&&(i2cAlive==0))
{
sladdr=sla;
regaddr=reg;
toTransmit=number;
i2cRw=0;
registerNotSent=1;
transmited=0;
i2cAlive=I2C_AlIVE_INTERVAL;
I2C1->CR1 |= I2C_CR1_START;
i2cToverStatus=I2C_STATUS_START_SENT;
}
}


void I2C_Recieve(unsigned char sla, unsigned char reg, unsigned char number)
{

if( (number>0)&&(i2cAlive==0))
{
sladdr=sla;
regaddr=reg;
toTransmit=number;
i2cRw=1;
registerNotSent=1;
transmited=0;
i2cAlive=I2C_AlIVE_INTERVAL;
I2C1->CR1 |= I2C_CR1_START;
i2cToverStatus=I2C_STATUS_START_SENT;
}

}



void i2cEncHandler(void)
{

if(i2cToverStatus==I2C_STATUS_RESET){
i2cBuff[0]=0x27; 
I2C_Transmit(0x18,0x20,1);}else
I2C_Recieve(0x18,0x80|0x28,6);

}


void i2cTransmissionOverHandler(void)
{char i,b;
 b=0;
if( (i2cToverStatus==I2C_STATUS_TRANSMISSION_OK)&&i2cRw  )
{
acCount++;
for(i=0;i<3;i++)
{
memcpy(&actmp,i2cBuff+2*i,2);
b|=actmp!=0;
acsum[i]+= actmp;

}

if(!b)I2C_Transmit(0x18,0x20,1);
if(acCount>=8)
{
acCount=0;
for(i=0;i<3;i++)
{
acmiddle[i]=acsum[i]>>3;
acsum[i]=0;
}


}



	  

i2cToverFlag=0;

}

}
