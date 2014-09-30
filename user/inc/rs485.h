#ifndef __RS485_H
#define __RS485_H





#define RX_485_BUFFER_SIZE 200
#define TX_485_BUFFER_SIZE 200
#define MAX_485_FUNCTION_NUMBER 5


#define READ_FUNCTION 0x01
#define READ_FUNCTION_OK 0x81
#define WRITE_FUNCTION 0x02
#define WRITE_FUNCTION_OK 0x82
#define TEST_FUNCTION 0x03
#define TEST_FUNCTION_OK 0x83
#define REPLY_SUCCES_WRITE_FLAG 4

#define EVENT_DEVICE_INTERFACE_DATA_RECIEVED 2
#define EVENT_NONE	   0

void rsRecieved( unsigned char bytes);
void sendDeviceInterface(unsigned char bytes);
void clearDeviceRxBuffer(void);
void rs485handler(void);
unsigned char Crc8(unsigned char*,unsigned char);
struct function485
{
unsigned short id;
unsigned char type;
unsigned char size;
void *data;
};

void rs485RegisterFunction(struct function485*);





#endif
