/* kalkulu bihurketak moduluko funtzioak

   1) Bi digituko balio baten karaktereak lortzeko funtzioa.
Parametroak:
bal, ASCII kodera itzuli behar den balioa adierazten du
d1, helbide bat non pisu handieneko digituaren ASCII kodea itzuliko duen
d0, helbide bat non pisu handieneko digituaren ASCII kodea itzuliko duen
2) Balio oso bat sartuta, balio hamaseitarra kalkulatzen da.
3) Balio bat parametro moduan sartuta, honen balio osoa kalkulatzen da.
4) ADC eta UART karaktere kateak osatzen dira, gero pantailaratu ahal izateko.

Egileak: Iker Galardi eta Unai Fernandez
Data: Maiatza 2021
*/

#include "p24HJ256GP610A.h"

//Kronoko digituak kalkulatzeko:
//==============================
void denbora_bihurketa(unsigned char *d1, unsigned char *d0,
                       unsigned int bal)
{
    *d1 = bal / 10;
    *d0 = bal % 10;
    *d1 |= 0x30;
    *d0 |= 0x30;
}

const unsigned char TAULAHEX[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

//Balio hamaseitarra kalkulatu:
//=============================
void hex_bihurketa(unsigned int bal, unsigned char *kar)
{
    unsigned int x, y, z;

    x = bal & 0x0300;
    x = x >> 8;
    kar[0] = TAULAHEX[x];

    y = bal & 0x00F0;
    y = y >> 4;
    kar[1] = TAULAHEX[y];

    z = bal & 0x000F;
    kar[2] = TAULAHEX[z];
}

//Balio osoa kalkulatu:
//=====================
void int_bihurketa(unsigned int bal, unsigned char *kar)
{
    unsigned int x, y, z, t;

    x = bal / 1000;
    bal = bal % 1000;
    kar[0] = TAULAHEX[x];

    y = bal / 100;
    bal = bal % 100;
    kar[1] = TAULAHEX[y];

    z = bal / 10;
    bal = bal % 10;
    kar[2] = TAULAHEX[z];

    t = bal;
    kar[3] = TAULAHEX[t];
}

//ADC eta UART karaktere kateak sortu:
//====================================
void mode_bihurketa(unsigned int bal, unsigned char *kar)
{
    if (bal == 0)
    {
        kar[0] = 'A';
        kar[1] = 'D';
        kar[2] = 'C';
        kar[3] = ' ';
    }
    else
    {
        kar[0] = 'U';
        kar[1] = 'A';
        kar[2] = 'R';
        kar[3] = 'T';
    }
}
