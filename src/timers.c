/* timerrak eta hauen ZE

   1) T4 timerra hasieratzeko funtzioa. Timer hau serboen mugimendu
   suabea, distantzia sentsorerak eta kronorako erabiliko da.
   2) T2 timerra hasieratzeko funtzioa. Timer hau besoaren gurpilen
   mugimendurako erabiliko da.
   3) T6 timerra hasieratzeko funtzioa. Timer hau PWM pultsuak 
   kontrolatzeko erabiliko da.
   4) T9 timerra hasieratzeko funtzioa. Timer hau LCD-aren egora makina
   kontrolatzeko erabiliko da.
   5) T3 timerra hasieratzeko funtzioa. Timer hau ADC laginketa egiteko
   erabiliko da.
   6) Kronoa hasieratzeko funtzioa.
   7) Parametro moduan ms kopurua sartuta, denbora hori itxarongo du.
   8) Parametro moduan us kopurua sartuta, denbora hori itxaringo du.
   9) Kronometroaren balioak eguneratzeaz arduratzen den funtzioa.
   10) T4 timerraren ZE. Bertan kronoa eguneratzeko ms balioa inkrementatzen 
   da. Horretaz aparte besoa suabe mugitzeko eta babes posizioaren 
   egoera makinak daude. Azkenik distantzia neurtzeko prest dagoela 
   adierazteko balioa aldatzen du.
   11) T6 timerraren ZE. PWN pultsuak sortzeko egoera makina.
   12) T9 timerrraren ZE. LCD-ren egoera makina.

Egileak: Iker Galardi eta Unai Fernandez
Data: Maiatza 2021
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "kalk_bihurketak.h"
#include "LCD.h"
#include "memoria.h"
#include "OC_PWM.h"
#include "CN.h"
#include "ADC1.h"

int tdist, dist_prest;

//1) T4 timerra hasieratu
//=======================
void hasieratu_timer4()
{
	TMR4 = 0;			 // Kontatze erregistroa hasieratu
	PR4 = 15625;		 // timerraren periodoa
	T4CONbits.TCKPS = 1; // prescaleraren eskala	1:1
	T4CONbits.TCS = 0;	 // barruko erlojua
	T4CONbits.TGATE = 0; // Gate modua galarazi
	T4CONbits.TON = 1;	 // denboratailea piztu
	IEC1bits.T4IE = 1;	 // Etenak gaitu
}

//2) T2 timerra hasieratu
//=======================
void hasieratu_timer2()
{
	TMR2 = 0;			 // Kontatze erregistroa hasieratu
	PR2 = 2500;			 // timerraren periodoa
	T2CONbits.TCKPS = 0; // prescaleraren eskala	1:1
	T2CONbits.TCS = 0;	 // barruko erlojua
	T2CONbits.TGATE = 0; // Gate modua galarazi
	T2CONbits.TON = 1;	 // denboratailea piztu
}

//3) T6 timerra hasieratu
//=======================
void hasieratu_timer6()
{
	TMR6 = 0;			 // Kontatze erregistroa hasieratu
	PR6 = 62500;		 // timerraren periodoa
	T6CONbits.TCKPS = 1; // prescaleraren eskala	1:1
	T6CONbits.TCS = 0;	 // barruko erlojua
	T6CONbits.TGATE = 0; // Gate modua galarazi
	T6CONbits.TON = 1;	 // denboratailea piztu
	IEC2bits.T6IE = 1;	 // Etenak gaitu
}

//4) T9 timerra hasieratu
//=======================
void hasieratu_timer9()
{
	TMR9 = 0;			 // Kontatze erregistroa hasieratu
	PR9 = 25000;		 // timerraren periodoa
	T9CONbits.TCKPS = 0; // prescaleraren eskala	1:1
	T9CONbits.TCS = 0;	 // barruko erlojua
	T9CONbits.TGATE = 0; // Gate modua galarazi
	T9CONbits.TON = 1;	 // denboratailea piztu
	IEC3bits.T9IE = 1;	 // Etenak gaitu
}

//5) T3 timerra hasieratu
//=======================
void hasieratu_timer3()
{
	TMR3 = 0;			 // Kontatze erregistroa hasieratu
	PR3 = 25000;		 // timerraren periodoa
	T3CONbits.TCKPS = 0; // prescaleraren eskala	1:1
	T3CONbits.TCS = 0;	 // barruko erlojua
	T3CONbits.TGATE = 0; // Gate modua galarazi
	T3CONbits.TON = 1;	 // denboratailea piztu
}

unsigned int ms, ds, s, min, egoeraLCD;

//6) Kronoa hasieratu:
//====================
void hasieratu_krono()
{
	ms = 0;	 // milisegunduak
	ds = 0;	 // segundu hamarrekoak
	s = 0;	 // segunduak
	min = 0; // minutuak
}

//7) Zehaztutako ms itxaron:
//==========================
void Delay_ms(uint16_t ms)
{
	TMR4 = 0;			 // Kontatze erregistroa hasieratu
	T4CONbits.TCKPS = 0; // prescaleraren eskala	1:1

	if (ms > 2)
	{
		T4CONbits.TCKPS = 1; // prescaleraren eskala	1:8
		PR4 = ms * (125000 / 8);
	}

	T4CONbits.TCS = 0;	 // barruko erlojua
	T4CONbits.TGATE = 0; // Gate modua galarazi
	T4CONbits.TON = 1;	 // denboratailea piztu

	// Itxaron denboragailuak beharrezko microsegunduak pasa arte.
	while (IFS1bits.T4IF == 0)
	{
		Nop();
	}
	IFS1bits.T4IF = 0;
}

//8) Zehaztutako us itxaron:
//==========================
void Delay_us(uint16_t us)
{
	TMR4 = 0;				// Kontatze erregistroa hasieratu
	PR4 = (us * 1000) / 40; // timerraren periodoa
	T4CONbits.TCKPS = 0;	// prescaleraren eskala	1:1
	T4CONbits.TCS = 0;		// barruko erlojua
	T4CONbits.TGATE = 0;	// Gate modua galarazi
	T4CONbits.TON = 1;		// denboratailea piztu

	// Itxaron denboragailuak beharrezko microsegunduak pasa arte.
	while (IFS1bits.T4IF == 0)
	{
		Nop();
	}
	IFS1bits.T4IF = 0;
}

//9) Kronometroaren balioak kalkulatu:
//====================================
void kronometroa()
{
	// Begiratu ea 100 milisegundu pasa diren, ahala bada, segundu hamarrenak
	// inkrementatu eta egin denbora bihurketa LCD-ko lerroan agertzeko
	if (ms >= 100)
	{
		ds++;
		ms = 0;
		denbora_bihurketa(&Leihoa_LCD[3][14], &Leihoa_LCD[3][15], ds % 99);
	}

	// Begiratu ea 9 segundu hamarren pasa diren, ahala bada, segunduak inkrementatu
	// eta egin denbora bihurketa LCD-ko lerroan agertzeko
	if (ds >= 9)
	{
		s++;
		ds = 0;
		denbora_bihurketa(&Leihoa_LCD[3][11], &Leihoa_LCD[3][12], s % 60);
	}

	// Begiratu ea 60 segundu pasa diren, ahala bada, minutuak inkrementatu
	// eta egin denbora bihurketa LCD-ko lerroan agertzeko
	if (s >= 60)
	{
		min++;
		s = 0;
		denbora_bihurketa(&Leihoa_LCD[3][8], &Leihoa_LCD[3][9], min % 60);
	}
}

//10) T4 timerraren ZE:
//=====================
void _ISR_NO_PSV _T4Interrupt()
{
	static unsigned int bukatu1 = 0;
	static unsigned int bukatu2 = 0;
	static unsigned int bukatu3 = 0;
	static unsigned int bukatu4 = 0;
	static unsigned int bukatu5 = 0;

	ms += 5;
	tdist++;
	if (tdist == 14)
		dist_prest = 1;

	//========================= potentziometroak suabe mugitzeko ==========================

	if (adc_lag == 0)
	{
		switch (smooth_adc_eg)
		{
		case 0:
			if (duty > duty_lag + 40)
				duty -= 40;
			else if (duty < duty_lag - 40)
				duty += 40;
			else
			{
			}
			smooth_adc_eg = 1;
			break;
		case 1:
			if (duty2 > duty_lag2 + 20)
				duty2 -= 10;
			else if (duty2 < duty_lag2 - 20)
				duty2 += 10;
			else
			{

				adc_lag = 1;
			}
			smooth_adc_eg = 0;
			break;
		}
	}

	//================= serboak segurtasun posziora piskanaka mugitu =============
	if (safe_pos == 1)
	{
		switch (safe_eg)
		{
		case 0:
			if (duty > S1_SAFE + 10)
				duty -= 10;
			else if (duty < S1_SAFE - 10)
				duty += 10;
			else
			{
				if (bukatu1 == 0)
				{
					token++;
					bukatu1 = 1;
				}
			}

			safe_eg = 1;
			break;
		case 1:
			if (duty2 > S2_SAFE + 10)
				duty2 -= 10;
			else if (duty2 < S2_SAFE - 10)
				duty2 += 10;
			else
			{
				if (bukatu2 == 0)
				{
					token++;
					bukatu2 = 1;
				}
			}

			safe_eg = 2;
			break;
		case 2:
			if (duty3 > S3_SAFE + 10)
				duty3 -= 10;
			else if (duty3 < S3_SAFE - 10)
				duty3 += 10;
			else
			{
				if (bukatu3 == 0)
				{
					token++;
					bukatu3 = 1;
				}
			}

			safe_eg = 3;
			break;
		case 3:
			if (duty4 > S4_SAFE + 10)
				duty4 -= 10;
			else if (duty4 < S4_SAFE - 10)
				duty4 += 10;
			else
			{
				if (bukatu4 == 0)
				{
					token++;
					bukatu4 = 1;
				}
			}
			safe_eg = 4;
			break;
		case 4:
			if (duty5 > S5_SAFE + 10)
				duty5 -= 10;
			else if (duty5 < S5_SAFE - 10)
				duty5 += 10;
			else
			{
				if (bukatu5 == 0)
				{
					token++;
					bukatu5 = 1;
				}
			}

			if (token == 5)
			{
				safe_pos = 0;
				token = 0;
				bukatu1 = 0;
				bukatu2 = 0;
				bukatu3 = 0;
				bukatu4 = 0;
				bukatu5 = 0;
				adc_uart = 1;
			}
			safe_eg = 0;
			break;
		}
	}
	IFS1bits.T4IF = 0;
}

//11) T6 timerraren ZE:
//=====================
void _ISR_NO_PSV _T6Interrupt()
{
	//Automataren kudeaketa:
	//	    -Lehen egoeran:
	//		DR0 pinaren irteeran 0-koa idatzi egiten du, gero timerraren
	//		TMR-a hasieratu eta PR2-a birkalkulatzen da geratzen diren tick kopurua jakiteko
	//
	//	    -Bigarren egoera:
	//		DR0 pinaren irteeran 1-koa idatzi. TMR hasieratu eta PR2-ri dutyren balioa ematen zaio
	//

	int pr_metatua = 0;
	switch (PWM_eg)
	{
	case 0:
		LATBbits.LATB10 = 0;
		LATBbits.LATB11 = 0;
		LATBbits.LATB12 = 0;
		LATBbits.LATB13 = 0;
		LATBbits.LATB14 = 0;
		TMR6 = 0;
		PR6 = 62500 - pr_metatua;
		PWM_eg = 1;
		break;
	case 1:
		LATBbits.LATB10 = 1;
		TMR6 = 0;
		PR6 = duty;
		pr_metatua = duty;
		PWM_eg = 2;
		break;
	case 2:
		LATBbits.LATB10 = 0;
		LATBbits.LATB11 = 1;
		LATBbits.LATB12 = 0;
		LATBbits.LATB13 = 0;
		LATBbits.LATB14 = 0;
		TMR6 = 0;
		PR6 = duty2;
		pr_metatua += duty2;
		PWM_eg = 3;
		break;
	case 3:
		LATBbits.LATB10 = 0;
		LATBbits.LATB11 = 0;
		LATBbits.LATB12 = 1;
		LATBbits.LATB13 = 0;
		LATBbits.LATB14 = 0;
		TMR6 = 0;
		PR6 = duty3;
		pr_metatua += duty3;
		PWM_eg = 4;
		break;
	case 4:
		LATBbits.LATB10 = 0;
		LATBbits.LATB11 = 0;
		LATBbits.LATB12 = 0;
		LATBbits.LATB13 = 1;
		LATBbits.LATB14 = 0;
		TMR6 = 0;
		PR6 = duty4;
		pr_metatua += duty4;
		PWM_eg = 5;
		break;
	case 5:
		LATBbits.LATB10 = 0;
		LATBbits.LATB11 = 0;
		LATBbits.LATB12 = 0;
		LATBbits.LATB13 = 0;
		LATBbits.LATB14 = 1;
		TMR6 = 0;
		PR6 = duty5;
		pr_metatua += duty5;
		PWM_eg = 0;
		break;
	}
	IFS2bits.T6IF = 0;
}

//12) T9 timerrraren ZE:
//======================
void _ISR_NO_PSV _T9Interrupt()
{
	static unsigned int ind;

	/// TODO: lerro maximoak konstante baten bidez gorde?????
	const int lehen_lerro_indizea = lerro_indize;
	const int bigarren_lerro_indizea = (lerro_indize + 1) % lerro_kop;

	// Automataren kudeaketa:
	//      - Lehen egoeran:
	//            Lehen lerrora mugitzen du kursorea eta bigarren egoerara
	//            salto egingo du.
	//
	//      - Bigarren egoera:
	//            Lehen lerroa inprimatzen joango da denboragailu
	//            eten bakoitzean, lerroa inprimatu ondoren hirugarren
	//            egoerara salto egingo du.
	//
	//      - Hirugarren egoera:
	//            Bigarren lerrora egingo du salto, eta azken egoerara mugituko
	//            da.
	//
	//      - Laugarren egoera:
	//            Bigarren lerroa inprimatzen joango da denboragailuaren eten
	//            bakoitzean. Hau bukatzean, lehen egoerara egingo du salto.
	switch (egoeraLCD)
	{
	case 0:
		lcd_cmd(0x80); //line_1(); delay gabe
		egoeraLCD = 1;
		ind = 0;
		break;
	case 1:
		lcd_data(Leihoa_LCD[lehen_lerro_indizea][ind]);
		ind++;
		if (ind == 16)
			egoeraLCD = 2;
		break;
	case 2:
		lcd_cmd(0xC0); //line_2(); delay gabe
		egoeraLCD = 3;
		ind = 0;
		break;
	case 3:
		lcd_data(Leihoa_LCD[bigarren_lerro_indizea][ind]);
		ind++;
		if (ind == 16)
			egoeraLCD = 0;
		break;
	}
	IFS3bits.T9IF = 0;
}
