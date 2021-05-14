/* ADC1 moduluaren funtzioak

   1) Funtzio honen bidez, ADC moduluaren hasierako beharrezko konfigurazioak egingo dira.
   2) Laginketa software bitartez abiarazteko
   3) Array bat pasata eta array  horren elementu kopurua jakinda bataz bestekoa kalkulatzen du.
   4) ADC1 balioa jaso
   Sentsore edo gailu bakoitzari dagokion array-an dauden datuak hartuta, 
   bataz besteko kalkulatzen du eta pantailaratzeko hex_bihurketa funtzioari egiten dio dei.
   5) ADC1-ren eten errutina:
   bertan ADCvalue aldagaian gordeko da jasotako datua.
   Txandakatzen joango da datua eskuratzen den gailua switcharen bidez.
   8 lagin jasotzerakoan ez da errutinan berriro sartu prest aldagaia 0 izan arte

Egileak: Iker Galardi eta Unai Fernandez
Data: Maiatza 2021
*/

#include <p24HJ256GP610A.h>
#include "kalk_bihurketak.h"
#include "memoria.h"
#include "commons.h"
#include "timers.h"
#include "OC_PWM.h"
#include "CN.h"
#include "srf08.h"
#include "i2c_funtzioak.h"

int ADCvalue;											  //Lortzen den balio analogikoa gordertzeko
int pot[8], tmp[8], x[8], y[8], j_pot[8], j_x[8], j_y[8]; //Hartutako laginak gordetzeko
int ind = 0;											  //indizea
int prest = 0;											  //bataz bestekoa kalkulatzeko prest dagoela adierazten duen aldagaia
int kont = 0;											  //kontagailua, ADC1-ren zerbitzu errutinara sartzen den aldiak kontatzeko
int adc_lag = 1;										  //adc_lag == 0 bada suabe mugituko da bestela ez.
int duty_lag;											  //duty-aren balioa zuzenean ez aldatzeko laguntzaile bat sortu, gero mugimendua suabe egiteko funtzioan erabiliko da.
int duty_lag2;											  //duty-aren balioa zuzenean ez aldatzeko laguntzaile bat sortu, gero mugimendua suabe egiteko funtzioan erabiliko da.
int smooth_adc_eg = 0;									  //ADC bidezko kontrola suabe egiteko egoera aukeraketa

//1) Konfigurazioa:
//=================

void prestatu_ADC1(void)
{
	// AD1CON1 kontrol-erregistroaren hasieraketa
	AD1CON1 = 0; // Eremu guztiak 0ra jarri

	// 1= 12 bitekin funtzionatzeko / 0= 10 bitekin
	//AD1CON1bits.AD12B = 0;

	// Bukatu laginketa eta hasi bihurketa
	// 111=Auto-convert / 010=TMR3 ADC1 eta TMR5 ADC2 / 001=INT0 / 000= SAMP ezabatu behar da
	AD1CON1bits.SSRC = 0b010;

	// 1= aldi bereko laginketa / 0= laginketa sekuentziala
	//AD1CON1bits.SIMSAM = 0;

	// 1= Laginketa hasi aurreko bihurketaren ondoren
	// 0= Laginketa hasi SAMP 1era pasatzen denean
	AD1CON1bits.ASAM = 1;

	// AD1CON2 kontrol-erregistroaren hasieraketa
	AD1CON2 = 0;

	// AD1CON3 kontrol-erregistroaren hasieraketa
	AD1CON3 = 0;
	//AD1CON3bits.ADRC = 0;  // 1= RC erlojua / 0= CPUren erlojua
	AD1CON3bits.SAMC = 31; // Auto laginketarako denbora = Tad kopurua
	AD1CON3bits.ADCS = 1;  // TAD >75ns = Tcy(ADCS+1)

	// AD1CON4 kontrol-erregistroaren hasieraketa
	AD1CON4 = 0;

	//AD1CHS123 erregistroaren hasieraketa
	AD1CHS123 = 0; //: kanala aukeratzeko: 1,2 eta 3

	//AD1CHS0 erregistroaren hasieraketa
	AD1CHS0 = 0;
	AD1CHS0bits.CH0SA = 5; // Aukeratu zer sarrera analogiko konektatzen den
	//AD1CHS0bits.CH0SB = 0;

	//AD1CSS erregistroen hasieraketa
	//sarrerak modu sekuentzialean eskaneatzeko:  1= aukeratua /0= EZ
	AD1CSSH = 0; // 16-31
	AD1CSSL = 0; // 0-15

	//AD1PCFG erregistroen hasieraketa
	AD1PCFGH = 0xFFFF; // Konfiguratu sarrerak: 1= digital / 0= Analog
	AD1PCFGL = 0xFFFF; // PORTB guztiak digitalak (1)

	// Analogiko gisa funtzionatzea nahi ditugunak banan banan hasieratu
	AD1PCFGLbits.PCFG4 = 0; // tenperatura-sentsorea
	AD1PCFGLbits.PCFG5 = 0; // potentziometroa
	AD1PCFGLbits.PCFG0 = 0; // x ardatza
	AD1PCFGLbits.PCFG1 = 0; // y ardatza
	AD1PCFGLbits.PCFG2 = 0; //joystick handiko pot
	AD1PCFGLbits.PCFG9 = 0; //joystick handiko x
	AD1PCFGLbits.PCFG8 = 0; //joystick handiko y

	//Eten-eskaerei lotutako biten hasieraketa
	IPC3bits.AD1IP = 4; // Lehentasun-maila ezartzeko, 0 eta 7 artean
	IFS0bits.AD1IF = 0; // ADC1aren eten-adierazlea
	IEC0bits.AD1IE = 1; // ADC1aren etena gaitzeko edo desgaitzeko

	//AD1CON
	AD1CON1bits.ADON = 1; // Bihurketaren hasiera
}

//2) Laginketa abiarazi:
//======================

void laginketa_hasi()
{
	AD1CON1bits.SAMP = 1;
}

//3) Bataz bestekoa kalkulatu:
//============================
int bbkalk(int arr[], int kop)
{
	int i, sum = 0, bb = 0;

	for (i = 0; i < kop; i++)
	{
		sum += arr[i];
	}
	bb = sum / kop;

	return bb;
}

//4) ADC1 balioa jaso:
//====================
void jaso_ADC1balioa()
{
	int bbpot, bbtmp, bbx, bby, bbj_pot, bbj_x, bbj_y;
	bbpot = 0;
	bbtmp = 0;
	bbx = 0;
	bby = 0;
	bbj_pot = 0;
	bbj_x = 0;
	bbj_y = 0;

	if (prest == 1)
	{

		//================ batazbestekoen kalkulua ================
		bbpot = bbkalk(pot, 8);
		bbtmp = bbkalk(tmp, 8);
		bbx = bbkalk(x, 8);
		bby = bbkalk(y, 8);
		bbj_pot = bbkalk(j_pot, 8);
		bbj_x = bbkalk(j_x, 8);
		bbj_y = bbkalk(j_y, 8);

		if (adc_uart == 0)
		{

			//============ ADC bidez serboak mugitzeko =============
			duty_lag = ((T_MAXPWM1 - T_MINPWM1) / 1023.0) * bbpot + T_MINPWM1;
			duty_lag2 = ((T_MAXPWM1 - T_MINPWM1) / 978.0) * bbj_pot + T_MINPWM1;
			adc_lag = 0;

			duty3 += (bbx - 515) / 20;
			duty4 += (bby - 513) / 20;

			//============ Gurpilak mugitzeko ==============
			int abiadura = (bbj_y - 475) * 4;
			if (dist_osoa >= 20)
			{
				if (bbj_y <= 475)
				{
					OC2RS = -abiadura;
					OC4RS = -abiadura;
				}
				else
				{
					OC3RS = abiadura;
					OC1RS = abiadura;
				}
			}
			else
			{
				OC2RS = 0;
				OC4RS = 0;
				if (bbj_y > 475)
				{
					OC3RS = abiadura;
					OC1RS = abiadura;
				}
			}
		}

		//================ duty-en balioak pantailaratu bakoitzari dagokion posizioan ===============
		hex_bihurketa(bbpot, &Leihoa_LCD[5][4]);
		hex_bihurketa(bbtmp, &Leihoa_LCD[5][12]);
		int_bihurketa(bbx, &Leihoa_LCD[11][10]);
		int_bihurketa(bby, &Leihoa_LCD[12][10]);
		hex_bihurketa(bbj_pot, &Leihoa_LCD[14][12]);
		int_bihurketa(bbj_x, &Leihoa_LCD[15][10]);
		int_bihurketa(bbj_y, &Leihoa_LCD[16][10]);
		prest = 0;
	}
}

//5) ADC1-ren eten errutina:
//==========================
void _ISR_NO_PSV _ADC1Interrupt()
{
	ADCvalue = ADC1BUF0;
	kont++;

	// ADC gailuaren bitartez hainbat seinale analogiko
	// analizatzen dira, eta horretarako egoera makina bat
	// erabiltzen da
	if (prest == 0)
	{
		switch (AD1CHS0bits.CH0SA)
		{
		case 5:
			pot[ind] = ADCvalue;
			AD1CHS0bits.CH0SA = 4;
			break;
		case 4:
			tmp[ind] = ADCvalue;
			AD1CHS0bits.CH0SA = 0;
			break;
		case 0:
			x[ind] = ADCvalue;
			AD1CHS0bits.CH0SA = 1;
			break;
		case 1:
			y[ind] = ADCvalue;
			AD1CHS0bits.CH0SA = 9;
			break;
		case 9:
			j_x[ind] = ADCvalue;
			AD1CHS0bits.CH0SA = 8;
			break;
		case 8:
			j_y[ind] = ADCvalue;
			AD1CHS0bits.CH0SA = 2;
			break;
		case 2:
			j_pot[ind] = ADCvalue;
			AD1CHS0bits.CH0SA = 5;
			ind++;
			if (ind >= 8)
			{
				ind = 0;
				prest = 1;
			}
			break;
		}
	}
	IFS0bits.AD1IF = 0;
}
