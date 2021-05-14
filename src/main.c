/*
   Proiektu honen helburua da, UART erabiliz, aurreko praktikan egindako kronometroa pantailaratzea da.
   LCDko datuak pantailan ikusteko gnu-screen programa erabili dugu, Linux-en lan egiten baitugu.

   programa exekutatzeko eta "baud-rate" zuzena izateko honako komando hau higikaritu beharko da:

   $ screen /dev/ttyUSB0 38400

Egileak: Iker Galardi eta Unai Fernandez
Data: Maiatza 2021
*/

#include "p24HJ256GP610A.h"

#include "GPIO.h"
#include "pultsadoreak.h"
#include "oscilator.h"
#include "timers.h"
#include "memoria.h"
#include "kalk_bihurketak.h"
#include "LCD.h"
#include "UART2_RS232.h"
#include "ADC1.h"
#include "OC_PWM.h"
#include "CN.h"
#include "i2c_funtzioak.h"
#include "srf08.h"

unsigned char dist[2];
unsigned char ds_helb = 0xE4; //distantzia sentsorearen helbidea
unsigned int dist_osoa;
unsigned char helb;

void main()
{
    hasieratu_osziladorea();  // Erlojuaren aukeraketa eta hasieraketa (80 MHz-etan)
    hasieratu_GPIO();         // GPIO-ren hasieraketa
    hasieratu_pultsadoreak(); //Pultsadoreen hasieraketa

    Init_LCD();       // LCDaren hasieraketa
    prestatu_UART2(); // UARTaren haiseraketa
    prestatu_ADC1();  //ADCren hasieraketa
    prestatu_OC1();   //OC moduluaren hasieraketa

    ////////////////////////////////////////////////////////////
    // 2. lerroa kokatu eta denbora bistaratu

    //lehen lerroan kokatu
    line_1();
    Kopiatu_FLASH_RAM(board_info);
    puts_lcd(Leihoa_LCD[0], sizeof(Leihoa_LCD[0]));

    bidali_UART2_leihoa();

    //S3 pultsadorea sakatu arte, ez atera proiektuaren mezua
    while (PORTDbits.RD6)
        ;

    // Begizta nagusia
    hasieratu_krono();
    hasieratu_timer2();
    hasieratu_timer4();
    hasieratu_timer3();
    hasieratu_timer9();
    hasieratu_timer6();
    konfiguratu_CN();
    U2TXREG = 0; //null karakterea eskuz bidali behar da sistema funtzionatzeko, bestela ez luke etenik sortuko

    //============ I2C hasieraketak ============
    InitI2C_1();
    Neurketa_berria_hasi(ds_helb);
    dist_prest = 0;
    tdist = 0;
    Helbide_zehaztu(&helb);
    while (1)
    {
        //=========== safe pos botoiaren egoera ===========
        mode_bihurketa(adc_uart, &Leihoa_LCD[2][6]);
        if (safe_pos == 0)
            Leihoa_LCD[13][8] = '0';
        else
            Leihoa_LCD[13][8] = '1';

        kronometroa();
        jaso_ADC1balioa();
        bistaratu_duty();

        //============== I2C ===============
        if (dist_prest == 1)
        {
            Irakurri_neurria(ds_helb, dist);
            //============ prozesatu ============
            dist_osoa = (dist[0] << 8) | (dist[1]);
            hex_bihurketa(dist_osoa, &Leihoa_LCD[23][6]);
            Neurketa_berria_hasi(ds_helb);
            tdist = 0;
            dist_prest = 0;
        }
    }
}
