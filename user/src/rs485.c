#include <string.h>
#include "rs485.h"

extern unsigned char deviceRxBuffer[RX_485_BUFFER_SIZE];
extern unsigned char deviceTxBuffer[TX_485_BUFFER_SIZE];
unsigned char rs485event;

unsigned char r485Active;

struct function485 funcs[MAX_485_FUNCTION_NUMBER];
unsigned char funcNumber=0;
unsigned char i,b,fsize;
unsigned short fid;
void *fdata;




unsigned char lngth=8;
unsigned short s;
extern unsigned	char data_buffer[];
void rs485RegisterFunction(struct function485 *fnc)
{
if(fnc->data!=NULL){memcpy(funcs+funcNumber,fnc,sizeof(struct function485) );funcNumber++;}
}


void rs485handler()
{

//if(deviceRxBuffer[0]&0x80==0)
if(Crc8(deviceRxBuffer,4)==deviceRxBuffer[4])
{
b=0xff;
i=0;
for(i=0;i<funcNumber;i++)
if( ( ( (funcs[i]).id)==deviceRxBuffer[2]))//&&( ((funcs[i]).id)>>8==deviceRxBuffer[1]) )
{
b=i;
break;
}
if(b!=0xff){
fdata=funcs[b].data;
fid=funcs[b].id;
fsize=funcs[b].size;

switch( deviceRxBuffer[0])
{
case READ_FUNCTION:
//if(  fsize==deviceRxBuffer[3] )
if(fdata!=NULL){
 r485Active=1;
memcpy(deviceTxBuffer+5,fdata,fsize);
deviceTxBuffer[0]=READ_FUNCTION_OK;
deviceTxBuffer[1]=fid>>8;
deviceTxBuffer[2]=fid&0xff;
deviceTxBuffer[3]=fsize+1;
deviceTxBuffer[4]=Crc8(deviceTxBuffer,4);
deviceTxBuffer[fsize+5]=Crc8(deviceTxBuffer+5,fsize);
sendDeviceInterface(fsize+6);
}


break;
case WRITE_FUNCTION:
//i=bytes-7;
if( fsize==deviceRxBuffer[3]-1 )
{
//if(Crc8(deviceRxBuffer+5,fsize)==deviceRxBuffer[5+fsize])
//if(fdata!=NULL)
memcpy(fdata,deviceRxBuffer+5,fsize);
///*if(deviceRxBuffer[3]&REPLY_SUCCES_WRITE_FLAG)
//{
deviceTxBuffer[0]=WRITE_FUNCTION_OK;
deviceTxBuffer[1]=fid>>8;
deviceTxBuffer[2]=fid&0xff;
deviceTxBuffer[3]=0;
deviceTxBuffer[4]=Crc8(deviceTxBuffer,4);
//sendDeviceInterface(5);
}

//  */
break;
case TEST_FUNCTION:
//if(fdata!=NULL)
{
deviceTxBuffer[0]=TEST_FUNCTION_OK;
deviceTxBuffer[1]=fid>>8;
deviceTxBuffer[2]=fid&0xff;
deviceTxBuffer[3]=fsize;	
deviceTxBuffer[4]=Crc8(deviceTxBuffer,4);
sendDeviceInterface(5);
}


break;
default:
break;
}
}

}
clearDeviceRxBuffer();
rs485event=EVENT_NONE;
}
