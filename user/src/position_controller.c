#include "main_inc.h"
#define MOTOR_MAX_PWM 1200
#define MOTOR_MAX_PWMx 1200
#define MOTOR_MAX_PWMy 1100
#define MOTOR_MAX_PWMz 1000
#define ENCODER_ROT 1
#define ENCODER_C	0

char t,t2,t3;
extern unsigned short encAngl[2];
signed short velocity[2];
extern unsigned char spicount;
extern unsigned	char buf[10];
extern char motor_handler_flag;
extern char i2c_handler_flag;
extern unsigned	char data_buffer[];
extern unsigned	char rx_buffer[];
extern unsigned char main_handler_flag;
const signed char *m1 =(const signed char *)rx_buffer;
const signed char *m2 =(const signed char *)(rx_buffer+1);

 signed short *m11 =(signed short *)data_buffer;
 signed short *m22 =(signed short *)(data_buffer+4);

unsigned char mode;
signed short targedSpd1,targedSpd2;

unsigned char bd,td;

unsigned char speedGradeStep1=25,speedGradeStep2=25;
//unsigned int adcSum;
//unsigned char adcSumCount;
//unsigned short adcval;
//unsigned short adcOldVal;
signed short mot_temp;
extern signed short motionVals[];
extern unsigned char i2cResetFlag;
extern unsigned char i2cAlive;
int motor_power1=0;
int motor_power2=0;
int motor_power3=0;
int motor_power4=0;
extern unsigned short encval;
extern unsigned short noDataFromEncoder[5];

signed short posOld1, posOld2;
signed short spd1, spd2, spd1old, spd2old, dSpd1, dSpd2;







unsigned short old1;
unsigned short old2;

signed short p1=8;
signed short d1=12;
signed short p2=4;
signed short d2=7;
 void motor_handler(void)
{						
//	   p1=			 *((signed short *)(rx_buffer+2));
	//   d1=			 *((signed short *)(rx_buffer+4));
spd1= encAngl[ENCODER_ROT]-posOld1;
posOld1= encAngl[ENCODER_ROT];
dSpd1=spd1-spd1old;
spd1old=spd1;

spd2= encAngl[ENCODER_C]-posOld2;
posOld2= encAngl[ENCODER_C];
dSpd2=spd1-spd1old;
spd2old=spd2;

motionVals[0]=spd1;

targedSpd1=*m1;
targedSpd2=*m2;
motionVals[1]=motor_power1;
motor_power1+=p1*(targedSpd1-spd1)-(d1*dSpd1);

if((targedSpd1==0)&&(motor_power1<100)&&(motor_power1>-100) )motor_power1=0;
motor_power2+=p1*(targedSpd2-spd2)-(d2*dSpd2);

if(	targedSpd2!=0)motor_power1=targedSpd2*9;
//*m11=motor_power1;
//*m22=motor_power2;

/*

if(velocity[0]<targedSpd1)motor_power1+=speedGradeStep1;else
if(velocity[0]>(targedSpd1+speedGradeStep1))motor_power1-=speedGradeStep1;	else motor_power1=targedSpd1;

if(velocity[1]<targedSpd2 )motor_power2+=speedGradeStep2;else
if(velocity[1]>(targedSpd2+speedGradeStep1))motor_power2-=speedGradeStep2;	else motor_power2=targedSpd2;
  */
//if((motor_power1+speedGradeStep1)<targedSpd1)motor_power1+=speedGradeStep1;else
//if(motor_power1>(targedSpd1+speedGradeStep1))motor_power1-=speedGradeStep1;	else motor_power1=targedSpd1;
//
//if(motor_power2>(targedSpd2+speedGradeStep2) )motor_power2-=speedGradeStep2;else
//if(motor_power2+speedGradeStep2<targedSpd2)motor_power2+=speedGradeStep2;	else motor_power2=targedSpd2;
if(noDataFromEncoder[ENCODER_ROT]>10)motor_power1=0; 
if(noDataFromEncoder[ENCODER_C]>10)motor_power2=0; 

if(	motor_power1> MOTOR_MAX_PWMx) motor_power1=MOTOR_MAX_PWMx;
if(	motor_power1< -MOTOR_MAX_PWMx) motor_power1=-MOTOR_MAX_PWMx;
if(	motor_power2> MOTOR_MAX_PWMx) motor_power2=MOTOR_MAX_PWMx;
if(	motor_power2< -MOTOR_MAX_PWMx) motor_power2=-MOTOR_MAX_PWMx;



if(motor_power1>0){

	TIM3->CCR3=motor_power1;
	GPIOB->BRR= GPIO_Pin_1;
	 }  
else if(motor_power1<0){
	TIM3->CCR3=-motor_power1;
	
	GPIOB->BSRR= GPIO_Pin_1;}
else {	TIM3->CCR3=0;}


if(motor_power2>0){TIM3->CCR1=motor_power2; GPIOA->BRR= GPIO_Pin_7;   }else if(motor_power2<0) {TIM3->CCR1=-motor_power2;GPIOA->BSRR= GPIO_Pin_7;} else {TIM3->CCR1=0;}//if(motor_power3>0){TIM2->CCR2=motor_power3;GPIOC->BRR= GPIO_Pin_14; GPIOC->BSRR= GPIO_Pin_15; }else if(motor_power3<0) {TIM2->CCR2=MOTOR_MAX_PWM+motor_power3;GPIOC->BSRR= GPIO_Pin_14;GPIOC->BSRR= GPIO_Pin_15;} else {GPIOC->BRR= GPIO_Pin_15;}


}



void TIM2_IRQHandler(void)
{
 t++;
 t2++;
 t3++;
  if(t>200){t=0;motor_handler_flag=1;}
  if(t3>50){t3=0; i2c_handler_flag=1;}
 //if(t2>111){GPIOB->BRR=GPIO_Pin_9; spicount=0; SPI1->DR=0xff; t2=0;};
  if(t2>100){ t2=0;main_handler_flag=1;  }
  
  
  
 if(i2cAlive>0)	 { i2cAlive--; if(i2cAlive==0)i2cResetFlag=1; }
//if(motor_power4>=0){TIM2->CCR2=motor_power4;GPIOC->BRR= GPIO_Pin_15;  }else {TIM3->CCR2=MOTOR_MAX_PWM-motor_power4;GPIOC->BSRR= GPIO_Pin_15;}


rs485periodicHandle();

TIM_ClearITPendingBit(TIM2,TIM_IT_Update); 

}


