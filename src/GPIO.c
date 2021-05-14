/*
   konfiguratu eta hasieratu LEDak eta sakagailuak

Egileak: Iker Galardi eta Unai Fernandez
Data: Maiatza 2021
*/

#include "p24HJ256GP610A.h"
#include "commons.h"

//Konfigurazioa:
//==============
void hasieratu_GPIO()
{
    //========== RA7 eta RA0 irteera pin bezala jarri (LED-ak erabili ahal izateko) =============
    TRISA = 0xff00;
    TRISAbits.TRISA7 = 1;

    TRISD = 0xff00;
    TRISDbits.TRISD9 = 0;
    TRISDbits.TRISD12 = 0;

    //========= Itxaron aldaketak efektua hartu arte ===========
    Nop();
    Nop();

    //======== LED-ak itzali ==========
    LATA = LATA & 0xff00;
    LATDbits.LATD9 = 1;
    LATDbits.LATD12 = 1;
}
