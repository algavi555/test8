#include"main_inc.h"
#define INSTRUMENT_DETECTED_VALUE 170
unsigned short encval;
unsigned short adcval;
unsigned short encold=512;
unsigned short rotbase=0;
unsigned short calval=0;
unsigned short val,val1,par;
unsigned char spibuff[50];
unsigned char spicount=0;
unsigned int adcSum;


unsigned char instr_1;
unsigned char instr_2;
unsigned char curr_byte;
unsigned char instrumentNumber;
unsigned char  oldInstrumentNumber;
unsigned char displayedInstrument;
unsigned short instrumentDetected;
extern unsigned short encAngl[];
//unsigned short adcMiddle;
unsigned char adcSumCount;

extern  unsigned char errorDetected;

void  SPI1_IRQHandler(void)
{ int tmp; 

val=(1024-(encAngl[2]>>4) );

if((encold>800)&& (val<300)  )rotbase++;else
if(	(encold<300)&& (val>800)  ){if(rotbase>0)rotbase--;else(calval=val);}

//if(encold>val){ if((encold-val)>10)


//else if((val-encold)>10)

encold=val;

  if( (calval>val)&&(rotbase==0) )calval=val;

   


 encval=(val|(rotbase<<10)) -calval;


}
void ADC1_2_IRQHandler(void)
{
//adcval=ADC1->DR;

adcSumCount++;
adcSum+= ADC1->DR ;
if(adcSumCount>=64){
 adcSumCount=0;
  adcval=adcSum>>6;
   adcSum=0; 
    
if(adcval<3720)
{
calval=val; encval=0; rotbase=0; displayedInstrument=0;}else 
if( (displayedInstrument==0)&&(instrumentNumber>19) ) 
displayedInstrument=instrumentNumber-19;  //detected=0;}else detected=1;
}

ADC_ClearITPendingBit (ADC1,ADC_IT_EOC);
}

void USART2_IRQHandler(void)
{
val1=USART2->SR;

 curr_byte=(USART2->DR)&0xff;
 if(curr_byte&0x80)
 instr_2=curr_byte;
  else 
 instr_1=curr_byte;
 if(  instr_1== ((~instr_2)&0xff)){if(oldInstrumentNumber== instr_1){
  instrumentNumber=instr_1;
  instrumentDetected=INSTRUMENT_DETECTED_VALUE;

   }oldInstrumentNumber=instr_1; }

}

