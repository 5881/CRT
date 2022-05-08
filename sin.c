#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cstdint>
#define FTDI 1
#ifdef FTDI
#include <stdlib.h>
extern "C"{
#include <mpsse.h>
}
#define SPEED 1000000
#endif

uint8_t buffer[10000];
uint16_t buf_shift=0;
uint8_t msin[256]={
	127, 130, 133, 136, 139, 142, 145, 148, 151, 154, 157, 160, 163, 166, 169, 172, 175, 178, 181, 184, 186, 189, 192, 194, 197, 200, 202, 205, 207, 209, 212, 214, 216, 218, 221, 223, 225, 227, 229, 230, 232, 234, 235, 237, 239, 240, 241, 243, 244, 245, 246, 247, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 253, 253, 254, 253, 253, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 247, 246, 245, 244, 243, 241, 240, 239, 237, 235, 234, 232, 230, 229, 227, 225, 223, 221, 218, 216, 214, 212, 209, 207, 205, 202, 200, 197, 194, 192, 189, 186, 184, 181, 178, 175, 172, 169, 166, 163, 160, 157, 154, 151, 148, 145, 142, 139, 136, 133, 130, 127, 123, 120, 117, 114, 111, 108, 105, 102, 99, 96, 93, 90, 87, 84, 81, 78, 75, 72, 69, 67, 64, 61, 59, 56, 53, 51, 48, 46, 44, 41, 39, 37, 35, 32, 30, 28, 26, 24, 23, 21, 19, 18, 16, 14, 13, 12, 10, 9, 8, 7, 6, 5, 4, 3, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 16, 18, 19, 21, 23, 24, 26, 28, 30, 32, 35, 37, 39, 41, 44, 46, 48, 51, 53, 56, 59, 61, 64, 67, 69, 72, 75, 78, 81, 84, 87, 90, 93, 96, 99, 102, 105, 108, 111, 114, 117, 120, 123};
uint8_t buffer2[512];

void singen(uint8_t sa,uint8_t sb,uint8_t fs){
	static uint8_t a=0;
	static uint8_t b=0;
	b+=fs;
	uint16_t n=0;
	while(n<512){
			buffer2[n++]=msin[a];
			buffer2[n++]=msin[b];
			a+=sa;
			b+=sb;
			}
}

uint16_t send_xy(uint8_t x, uint8_t y){
	buffer[buf_shift++]=x;
	buffer[buf_shift++]=y;
	return buf_shift;
	}

int main( int argc, const char** argv )
{
#ifdef FTDI
/*****************************************************************
 * А теперь выведем сходу на экран через FT232H
 *****************************************************************/
	struct mpsse_context *spi = NULL;
	spi = MPSSE(SPI0, SPEED, LSB);
	while(1){
		singen(4,5,1);
		Start(spi);
		//FastWrite(spi, (char*)buffer, buf_shift);
		for(uint16_t i=0;i<10;i++)FastWrite(spi, (char*)buffer2, 512);
		Stop(spi);
		}
	Close(spi);
#endif

}

