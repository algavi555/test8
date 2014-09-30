#include"main_inc.h"
unsigned short encval;
unsigned short adcval;
unsigned short encold[2]={512};
unsigned short rotbase[2]={0};
unsigned short calval=0;
unsigned short val,val1,par;
unsigned char spibuff[50];
unsigned char spicount=0;
unsigned int adcSum;

unsigned short noDataFromEncoder[5];
unsigned short encAngl[5];

char encnum=0;

char spiHandlerFlag;

//unsigned short adcMiddle;
unsigned char adcSumCount;

//extern  unsigned char errorDetected;


void spiEncHandler(void)
{

spiHandlerFlag=0;

 

val1=  ((spibuff[0]&0x7f)<<8) |(spibuff[1]); 
par= val1^(val1>>1);
par= par^(par>>2);
par= par^(par>>4);
par= par^(par>>8);
if(  (!((par&1)^(spibuff[2]>>7)))&& (spibuff[1]&16 )&& ! (spibuff[1]&8)       )
{
noDataFromEncoder[encnum]=0;
val=val1>>5;

	
if((encold[encnum]>800)&& (val<300)  )rotbase[encnum]++;else
if(	(encold[encnum]<300)&& (val>800)  )rotbase[encnum]--;
encold[encnum]=val;
//position[encnum]=

encAngl[encnum]=val|(rotbase[encnum]<<10);


}else  noDataFromEncoder[encnum]++;

switch(encnum)
{
case 0: {GPIOB->BSRR=GPIO_Pin_9; GPIOC->BRR=GPIO_Pin_13; encnum++; SPI1->DR=0xff; break;}

case 1:	{GPIOC->BSRR=GPIO_Pin_13; GPIOB->BRR=GPIO_Pin_9; encnum=0; SPI1->DR=0xff; break;}


}

}


void  SPI1_IRQHandler(void)
{ int tmp; 

spibuff[spicount]=SPI1->DR;
tmp=SPI1->SR;
spicount++;
		  
if(spicount>=3)spiHandlerFlag=1;


if(spicount<3)SPI1->DR=0xff;else spicount=0;
	  


}
