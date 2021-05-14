/*
   PWM pultsuak sortzeko modulua (OC erabiliz)

   1) OC1 modulua eta PWM pultsuen hasieraketa egingo da funtzio honetan.
   Kasu honentan OC moduluak besoaren gurpilak aurrera edo atzera mugitzeko
   erabiliko dira. Bestalde Timer bat erabiliz PWM pultso bat sortuko da 
   besoa osatzen duten serboak mugitzeko.
   2) Funtzio honen bitartez serbo bakoitzaren duty-a dagokion posizioan
   pantailaratuko da.

Egileak: Iker Galardi eta Unai Fernandez
Data: Maiatza 2021
*/

#include "p24HJ256GP610A.h"
#include "kalk_bihurketak.h"
#include "OC_PWM.h"
#include "memoria.h"

// DEFINIZIOAK
//=========================================================
#define MINPWM 0.5 // pultsuaren iraupen minimoa (milisegundo)
#define MAXPWM 2.5 // pultsuaren iraupen maximoa (milisegundo)

#define T20ms 62500 // Timerraren periodoa, 20 ms iraun dezan
// Adierazi argi nola egiten den kalkulua iruzkinetan
// eta zein den erabilitako prescaler-a

#define T10KHz 2500

//#define T_MINPWM (T20ms / 20) * MINPWM // pultsuaren iraupen minimoa (ticks)
//#define T_MAXPWM (T20ms / 20) * MAXPWM // pultsuaren iraupen maximoa (ticks)

#define T_MINPWM1 1848 // pultsuaren iraupen minimoa, OC1-en kasuan (ticks)
#define T_MAXPWM1 7727 // pultsuaren iraupen minimoa, OC1-en kasuan (ticks)

//FUNTZIOAK
//=========================================================

// Kasu honetan OC moduluaren funtzionamendu berdian lortu behar da, PWM moduko pultso bat sortzen timer baten bitartez
int PWM_eg, duty, duty2, duty3, duty4, duty5;

//1) OC1 moduluaren eta PWM pultsuak hasieratu:
//=============================================
void prestatu_OC1()
{
    //============================== serboak hasieratzeko ==============================
    PWM_eg = 1;
    //Serbo1
    TRISBbits.TRISB10 = 0;
    LATBbits.LATB10 = 1;
    //Serbo2
    TRISBbits.TRISB11 = 0;
    LATBbits.LATB11 = 0;
    //Serbo3
    TRISBbits.TRISB12 = 0;
    LATBbits.LATB12 = 0;
    //Serbo4
    TRISBbits.TRISB13 = 0;
    LATBbits.LATB13 = 0;
    //Serbo5
    TRISBbits.TRISB14 = 0;
    LATBbits.LATB14 = 0;

    // Hasierako posizioa posizio segurua bezala jarri
    duty = S1_SAFE;
    duty2 = S2_SAFE;
    duty3 = S3_SAFE;
    duty4 = S4_SAFE;
    duty5 = S5_SAFE;

    //======================== gurpilak hasieratu ========================

    OC1CON = 0;
    //OC1CONbits.OCM=0b000;     // OC1 modulua desgaitu
    OC1CONbits.OCTSEL = 0; // timer 2 aukeratu da OCrekin lan egiteko

    OC1R = 0;     // Kalkulatu pultsuaren iraupen ertaina
    OC1RS = OC1R; // OC1RS hasieratu

    OC1CONbits.OCM = 0b110; // gaitu OC1 modulua PWM moduan

    OC2CON = 0;
    //OC1CONbits.OCM=0b000;     // OC1 modulua desgaitu
    OC2CONbits.OCTSEL = 0; // timer 2 aukeratu da OCrekin lan egiteko

    OC2R = 0;     // Kalkulatu pultsuaren iraupen ertaina
    OC2RS = OC2R; // OC1RS hasieratu

    OC2CONbits.OCM = 0b110; // gaitu OC1 modulua PWM moduan

    OC3CON = 0;
    //OC1CONbits.OCM=0b000;     // OC1 modulua desgaitu
    OC3CONbits.OCTSEL = 0; // timer 2 aukeratu da OCrekin lan egiteko

    OC3R = 0;     // Kalkulatu pultsuaren iraupen ertaina
    OC3RS = OC3R; // OC1RS hasieratu

    OC3CONbits.OCM = 0b110; // gaitu OC1 modulua PWM moduan

    OC4CON = 0;
    //OC1CONbits.OCM=0b000;     // OC1 modulua desgaitu
    OC4CONbits.OCTSEL = 0; // timer 2 aukeratu da OCrekin lan egiteko

    OC4R = 0;     // Kalkulatu pultsuaren iraupen ertaina
    OC4RS = OC4R; // OC1RS hasieratu

    OC4CONbits.OCM = 0b110; // gaitu OC1 modulua PWM moduan
}

//2) Serbo bakoitzaren duty-a bistaratu:
//======================================
void bistaratu_duty()
{
    int_bihurketa(duty, &Leihoa_LCD[7][3]);
    int_bihurketa(duty2, &Leihoa_LCD[7][11]);
    int_bihurketa(duty3, &Leihoa_LCD[8][3]);
    int_bihurketa(duty4, &Leihoa_LCD[8][11]);
    int_bihurketa(duty5, &Leihoa_LCD[9][3]);
    int_bihurketa(OC1R, &Leihoa_LCD[18][8]);
    int_bihurketa(OC2R, &Leihoa_LCD[19][8]);
    int_bihurketa(OC3R, &Leihoa_LCD[20][8]);
    int_bihurketa(OC4R, &Leihoa_LCD[21][8]);
}
