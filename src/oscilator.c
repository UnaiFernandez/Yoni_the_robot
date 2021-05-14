#include "p24HJ256GP610A.h"

// Configuracion
#pragma config FNOSC = PRIPLL // Osziladore nagusia (XT, HS, EC) w/PLL
#pragma config FCKSM = CSECMD // Erlojuen aldaketa aktibatu
#pragma config OSCIOFNC = OFF // OSC2 jarri erloju moduan
#pragma config POSCMD = XT    // XT osziladorea
#pragma config FWDTEN = OFF   // Watchdog Timerra desaktibatu

void hasieratu_osziladorea()
{
    PLLFBD = 25 - 2;        // M=PLLFBD+2
    CLKDIVbits.PLLPOST = 0; // N2=(PLLPOST+1)*2
    CLKDIVbits.PLLPRE = 0;  // N1=PLLPRE+2
    RCONbits.SWDTEN = 0;    // Disable Watch Dog Timer

    __builtin_write_OSCCONH(0x03); // Hasi erloju aldaketa nagusian
    __builtin_write_OSCCONL(0x01); // Hasi erloju aldaketa

    while (OSCCONbits.COSC != 0b011)
        ; // Erloju aldaketari itxaron

    // Wait for PLL to lock
    while (OSCCONbits.LOCK != 1);
}
