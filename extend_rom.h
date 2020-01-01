#ifndef __EXTEND_ROM_H__
#define __EXTEND_ROM_H__

#include <reg51.h>

sbit SCL=P2^1;
sbit SDA=P2^0;

void RomStart();
void RomStop();
unsigned char RomSendByte(unsigned char dat);
unsigned char RomReadByte();
void RomWrite(unsigned char addr,unsigned char dat);
unsigned char RomRead(unsigned char addr);

#endif