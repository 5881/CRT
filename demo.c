//#include "opencv2/imgproc.hpp"
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
#include "vector_font.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cstdint>
#include "xaker-logo.h"
//#include <omp.h>
//#define N 3 //число заходов в финальном массиве
//#define SAVE 1
//#ifndef _OPENMP
//static_assert(false, "openmp support required");
//#endif

#define FTDI 1
#ifdef FTDI
#include <stdlib.h>
extern "C"{
#include <mpsse.h>
}
#define SPEED 10000000
#endif

uint8_t buffer[10000];
uint16_t buf_shift=0;

uint16_t send_xy(uint8_t x, uint8_t y){
	buffer[buf_shift++]=x;
	buffer[buf_shift++]=y;
	return buf_shift;
	}

void send_char(uint8_t chr, uint8_t x, uint8_t y){
	uint8_t c=(chr<0xe0) ? chr - 0x20 : chr - 0x50;
	uint16_t sh=0;
	uint8_t c_len=0;
	sh=shift[c];
	c_len=ASCII[sh];
	sh++;
	for(uint8_t j=0;j<3;j++)for(uint8_t i=0;i<c_len;i++)send_xy(ASCII[sh+2*i]+x,ASCII[sh+2*i+1]+y);
	//for(uint8_t i=0;i<c_len;i++)send_xy(ASCII[sh+2*i]+x,ASCII[sh+2*i+1]+y);
	}
void send_str(char *str,uint8_t x, uint8_t y){
	//char c;
	while(*str){
		//printf("%x\r\n",(uint8_t)*str);
		if((uint8_t)*str==0xd0||(uint8_t)*str==0xd1) str++;
		if(*str=='\r'){x=0;str++;}
		if(*str=='\n'){y+=8*2;str++;}
		send_char((uint8_t)*str++,x,y);
		x+=6*2;
		}
	}



int main( int argc, const char** argv )
{
    send_str("Candidum 2022",1,80);
	send_str("Eritis sicut Deus,\r\n scientes bonum\r\n   et malum.",1,100);
	//send_str((char*)argv[1],1,100);
#ifdef FTDI
/*****************************************************************
 * А теперь выведем сходу на экран через FT232H
 *****************************************************************/
	struct mpsse_context *spi = NULL;
	spi = MPSSE(SPI0, SPEED, LSB);
	//for(uint8_t i=0;i<20;i++)printf("%u\r\n",out_mix[i]);
	while(1){
	//for(uint64_t i=0; i<60000;i++){
		Start(spi);
		
		FastWrite(spi, (char*)xaker_logo,sizeof(xaker_logo));
		FastWrite(spi, (char*)buffer, buf_shift);
		Stop(spi);
		}
	Close(spi);
#endif
	
	

}


