/**********************************************************\
* SCDTR - Projeto - stage 2 (Distributed Lighting Control) *
*                                                          *
* i2c.cpp                                                  *
*                                                          *
* José Carlos Vieira - 90900                               *
* Pedro Carmo        - 90989                               *
*                                                          *
\**********************************************************/

// g++ -std=c++0x pig2i2c.c -pthread -o pig2i2c
// sudo killall pigpiod
// sudo pigpiod -s 2
// pigs no
// pigs nb 0 0xC0000
// ./pig2i2c 18 19 </dev/pigpio0

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <thread>
#include <sstream>
#include <string>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include "pigpio.h"
#include <iostream>

#define RS (sizeof(gpioReport_t))

#define SCL_FALLING 0
#define SCL_RISING  1
#define SCL_STEADY  2
#define SDA_FALLING 0
#define SDA_RISING  4
#define SDA_STEADY  8

#define I2C_SCL 18
#define I2C_SDA 19

#define MAX_SIZE_I2C_IN 25
#define SOM 253
#define EOM 254

extern void ArduinoCall(char*);

bool flag_i2c = false;
char cont;
char i2c_input[MAX_SIZE_I2C_IN];

static char * timeStamp(){
   static char buf[32];

   struct timeval now;
   struct tm tmp;

   gettimeofday(&now, NULL);

   localtime_r(&now.tv_sec, &tmp);
   strftime(buf, sizeof(buf), "%F %T", &tmp);

   return buf;
}

void parse_I2C(void){
   int gSCL, gSDA, SCL, SDA, xSCL, xSDA, r;
   uint32_t level, changed, bI2C, bSCL, bSDA;
   gpioReport_t report;

   static int in_data=0, byte=0, bit=0;
   static int oldSCL=1, oldSDA=1;

   bool flag_i2c = false;

   gSCL = I2C_SCL;
   gSDA = I2C_SDA;

   bSCL = 1<<gSCL;
   bSDA = 1<<gSDA;

   bI2C = bSCL | bSDA;

   /* default to SCL/SDA high */
   SCL = 1;
   SDA = 1;
   level = bI2C;

   int pp = open("/dev/pigpio0",O_RDONLY);
   while ((r=read(pp, &report, RS)) == RS){
      report.level &= bI2C;

      if (report.level != level){
         changed = report.level ^ level;

         level = report.level;

         if (level & bSCL) SCL = 1; else SCL = 0;
         if (level & bSDA) SDA = 1; else SDA = 0;

            if (SCL != oldSCL){
                oldSCL = SCL;
                if (SCL) xSCL = SCL_RISING;
                else xSCL = SCL_FALLING;
            }
            else xSCL = SCL_STEADY;

            if (SDA != oldSDA){
                oldSDA = SDA;
                if (SDA) xSDA = SDA_RISING;
                else xSDA = SDA_FALLING;
            }
            else xSDA = SDA_STEADY;

            switch (xSCL+xSDA){
                case SCL_RISING + SDA_RISING:

                case SCL_RISING + SDA_FALLING:

                case SCL_RISING + SDA_STEADY:
                    if (in_data){
                        if (bit++ < 8){
                            byte <<= 1;
                            byte |= SDA;
                        }else{
			    //printf("%2x\n", byte);
                            if(byte == SOM && !flag_i2c){ //start of transmission
				flag_i2c = true;
				cont=0;
				i2c_input[cont] = byte;
			    }else if(byte != EOM && flag_i2c){
                                cont++;
				if(cont == MAX_SIZE_I2C_IN-1)
				    flag_i2c = false;
				i2c_input[cont] = byte;
                            }else if(byte == EOM && flag_i2c){ //end of transmission
				flag_i2c = false;
				ArduinoCall(i2c_input);
			    }
                            bit = 0;
                            byte = 0;
                        }
                    }
                    break;

               case SCL_FALLING + SDA_RISING:
                    break;

               case SCL_FALLING + SDA_FALLING:
                    break;

               case SCL_FALLING + SDA_STEADY:
                    break;

               case SCL_STEADY + SDA_RISING: // stop
                    if (SCL){
                        in_data = 0;
                        byte = 0;
                        bit = 0;
                        fflush(NULL);
                    }
                    break;

               case SCL_STEADY + SDA_FALLING: // start
                    if (SCL){
                        in_data = 1;

                        byte = 0;
                        bit = 0;
                    }
                break;

                case SCL_STEADY + SDA_STEADY:
                break;
         }
      }
   }
}
