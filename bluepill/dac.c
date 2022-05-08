/**********************************************************************
 * Секция include и defines
**********************************************************************/
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/assert.h>
#include <libopencm3/stm32/flash.h>
#include <math.h>


#include "xaker.h"
#include "vector_font.h"

static const uint8_t xa[]={0,0,20,0,20,50,0,50};
static const uint8_t ax[]={50,0,30,0,30,50,50,50};
//static const uint8_t test[]={0,0,20,0,20,50,0,50,
//	0xff,50,0,30,0,30,50,50,50,0xff,10,25,40,25};
 uint8_t msin[256]={
	127, 130, 133, 136, 139, 142, 145, 148, 151, 154, 157, 160, 163, 166, 169, 172, 175, 178, 181, 184, 186, 189, 192, 194, 197, 200, 202, 205, 207, 209, 212, 214, 216, 218, 221, 223, 225, 227, 229, 230, 232, 234, 235, 237, 239, 240, 241, 243, 244, 245, 246, 247, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 253, 253, 254, 253, 253, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 247, 246, 245, 244, 243, 241, 240, 239, 237, 235, 234, 232, 230, 229, 227, 225, 223, 221, 218, 216, 214, 212, 209, 207, 205, 202, 200, 197, 194, 192, 189, 186, 184, 181, 178, 175, 172, 169, 166, 163, 160, 157, 154, 151, 148, 145, 142, 139, 136, 133, 130, 127, 123, 120, 117, 114, 111, 108, 105, 102, 99, 96, 93, 90, 87, 84, 81, 78, 75, 72, 69, 67, 64, 61, 59, 56, 53, 51, 48, 46, 44, 41, 39, 37, 35, 32, 30, 28, 26, 24, 23, 21, 19, 18, 16, 14, 13, 12, 10, 9, 8, 7, 6, 5, 4, 3, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 16, 18, 19, 21, 23, 24, 26, 28, 30, 32, 35, 37, 39, 41, 44, 46, 48, 51, 53, 56, 59, 61, 64, 67, 69, 72, 75, 78, 81, 84, 87, 90, 93, 96, 99, 102, 105, 108, 111, 114, 117, 120, 123};

void send_char(char chr, uint8_t x, uint8_t y){
	uint8_t c=(chr<0xe0) ? chr - 0x20 : chr - 0x50;
	uint16_t sh=0;
	uint8_t c_len=0;
	sh=shift[c];
	c_len=ASCII[sh];
	sh++;
	for(uint8_t j=0;j<2;j++)for(uint8_t i=0;i<c_len;i++)send_xy(ASCII[sh+2*i]+x,ASCII[sh+2*i+1]+y);
	}
void send_str(char *str,uint8_t x, uint8_t y){
	//char c;
	while(*str){
		if(*str==0xd0||*str==0xd1) str++;
		if(*str=='\r'){x=0;str++;}
		if(*str=='\n'){y+=8*2;str++;}
		send_char(*str++,x,y);
		x+=6*2;
		}
	}

static void spi1_init(void){
	// Включаем порт и интерфейс	
	rcc_periph_clock_enable(RCC_SPI1);
	rcc_periph_clock_enable(RCC_GPIOA);
	/* Configure GPIOs:
	 * 
	 * SCK=PA5
	 * MOSI=PA7
	 */
	 //MOSI & SCK
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO5|GPIO7);
   
   //Reset SPI, SPI_CR1 register cleared, SPI is disabled
	spi_reset(SPI1);
  /* Set up SPI in Master mode with:
   * Clock baud rate: 1/2 of peripheral clock frequency
   * Clock polarity: Idle High
   * Clock phase: Data valid on 2nd clock pulse
   * Data frame format: 8-bit
   * Frame format: MSB First
   */
	spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_2, 
					SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
					SPI_CR1_CPHA_CLK_TRANSITION_1,
					SPI_CR1_DFF_8BIT, SPI_CR1_LSBFIRST);
	//spi_set_full_duplex_mode(SPI1);
  /*
   * Set NSS management to software.
   *
   * Note:
   * Setting nss high is very important, even if we are controlling 
   * the GPIO
   * ourselves this bit needs to be at least set to 1, otherwise the spi
   * peripheral will not send any data out.
   */
	spi_enable_software_slave_management(SPI1);
	spi_set_nss_high(SPI1);
  /* Enable SPI1 periph. */
	spi_enable(SPI1);
	//gpio_set(GPIOA,GPIO4);
	}
void gpio_init(){
	rcc_periph_clock_enable(RCC_GPIOB);
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
	              GPIO_CNF_OUTPUT_PUSHPULL, GPIO6|GPIO12);
	/*
	 * GPIO4 - ST_CP
	 * GPIO12 - LED
	 */
	gpio_set(GPIOB,GPIO12);
	gpio_clear(GPIOB,GPIO6);
	}
void send_xy(uint8_t x, uint8_t y){
	spi_xfer(SPI1,x);
	spi_xfer(SPI1,y);
	gpio_set(GPIOB,GPIO6);
	gpio_clear(GPIOB,GPIO6);
	
	}
	
void send_data(uint8_t *data,uint16_t size){
	size=size/2;
	do{
		spi_xfer(SPI1,*data++);
		spi_xfer(SPI1,*data++);
		gpio_set(GPIOB,GPIO6);
		gpio_clear(GPIOB,GPIO6);
		//for(uint32_t i=0;i<0xff;i++)__asm__("nop");
	}while(size--);
}
#define NPOT 3
void send_data_new(uint8_t *data,int16_t size,uint8_t dx,uint8_t dy){
	
	static n=0;
	size=size/2;
	data+=2*n;
	n++;
	if(n>NPOT)n=0;
	do{
		spi_xfer(SPI1,(*data++)+dx);
		spi_xfer(SPI1,(*data++)+dy);
		gpio_set(GPIOB,GPIO6);
		gpio_clear(GPIOB,GPIO6);
		data+=(NPOT-1)*2;
		//for(uint32_t i=0;i<0xff;i++)__asm__("nop");
	}while((size-=NPOT)>0);
}

void drawcircle(unsigned char x0, unsigned char y0,
							unsigned char radius) {
	short int x = 0;
	short int y = radius;
	short int delta = 1 - 2 * radius;
	short int error = 0;
	while(y >= 0) {
		send_xy(x0 + x, y0 + y);
		send_xy(x0 + x, y0 - y);
		send_xy(x0 - x, y0 + y);
		send_xy(x0 - x, y0 - y);
		error = 2 * (delta + y) - 1;
		if(delta < 0 && error <= 0) {
			++x;
			delta += 2 * x + 1;
			continue;
			}
		error = 2 * (delta - x) - 1;
		if(delta > 0 && error > 0) {
			--y;
			delta += 1 - 2 * y;
			continue;
			}
		++x;
		delta += 2 * (x - y);
		--y;
		}
	}
	
void drawline(uint8_t x1, uint8_t y1,uint8_t x2,
						           uint8_t y2){
	int16_t deltax,deltay,signx,signy;
    deltax = x2 - x1;
    if(deltax<0) deltax*=-1;
    deltay = y2 - y1;
    if(deltay<0) deltay*=-1;
    signx = x1 < x2 ? 1 : -1;
    signy = y1 < y2 ? 1 : -1;
    //
    int16_t error = deltax - deltay, error2;
    //
    send_xy(x2, y2);
		while(x1 != x2 || y1 != y2){
			send_xy(x1, y1);
			error2 = error * 2;
			//
			if(error2 > -deltay){
				error -= deltay;
				x1 += signx;
				}
			if(error2 < deltax){
				error += deltax;
				y1 += signy;
				}
		}
	}

void plot(uint8_t *data,uint16_t datasize){
	//uint8_t n=datasize/2;
	uint16_t i=0;
	while((i+3)<datasize){
		drawline(data[i],data[i+1],data[i+2],data[i+3]);
		i+=4;
		}
	}

void main(void){
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
	spi1_init();
	gpio_init();
	//uint8_t n=0;
	//float t=0;
	uint8_t x=0, y=0;
	uint8_t t=0,t2=64;
	//**************************************
	uint32_t n=1;
	uint8_t a=128, b=128;
	//double_t X=0,Y=127,X1,Y1;
	while(1){
		send_str("6ЛО1И CRT display\r\n Candidum 2022",0,0);
		send_data_new(xaker,sizeof(xaker),0,0);
		n++;
		if(!(n%2))b++;
		for(uint16_t i=0;i<256;i++){
			send_xy(msin[a],msin[b]);
			a+=1;
			b+=3;
			}
		//send_data_new(art,sizeof(art),0,0);
		}
	}
