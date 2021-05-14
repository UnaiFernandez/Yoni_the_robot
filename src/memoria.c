/*
   1) Funtzio honen bidez flash RAM-era pantailaratu nahi den informazioa 
   pasako da.

Egileak: Iker Galardi eta Unai Fernandez
Data: Maiatza 2021
*/

#include "p24HJ256GP610A.h"
#include "memoria.h"


//Pantailaratu nahi den informazioaren formatua:
//=============================================
const char *board_info = {" YONI THE ROBOT ""\x0d\x0a" //0
						  "================""\x0d\x0a" //1
						  "MODE: ----      ""\x0d\x0a" //2
						  "KRONO:  --:--,--""\x0d\x0a" //3
						  "===== PWM ======""\x0d\x0a" //4
						  "POT:--- TMP:--- ""\x0d\x0a" //5
						  "=== Serboak ====""\x0d\x0a" //6
						  "D1:---- D2:---- ""\x0d\x0a" //7
						  "D3:---- D4:---- ""\x0d\x0a" //8
						  "D5:----         ""\x0d\x0a" //9
						  "=== Joystick ===""\x0d\x0a" //10
						  "X txikia: ----  ""\x0d\x0a" //11
						  "Y txikia: ----  ""\x0d\x0a" //12
						  "Botoia: -       ""\x0d\x0a" //13
						  "POT handia: --- ""\x0d\x0a" //14
						  "X handia: ----  ""\x0d\x0a" //15
						  "Y handia: ----  ""\x0d\x0a" //16
						  "=== Motorrak ===""\x0d\x0a" //17
						  "M1 fwd: ----    ""\x0d\x0a" //18
						  "M1 rev: ----    ""\x0d\x0a" //19
						  "M2 fwd: ----    ""\x0d\x0a" //20
						  "M2 rev: ----    ""\x0d\x0a" //21
						  "==== srf 08 ====""\x0d\x0a" //22
						  "Dist: ---       ""\x0d\x0a"}; //23

unsigned char Leihoa_LCD[lerro_kop][18];


//1) Flash RAM-era kopiatzeko funtzioa:
//=====================================
void Kopiatu_FLASH_RAM (const char *testua)
{
    unsigned int j;
    for (j=0;j<18*lerro_kop;j++) Leihoa_LCD[0][j]=testua[j];
}
