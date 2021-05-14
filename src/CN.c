/*
Azalpena: CN = Change Notification sarrerak tratatzeko

1) CN sarreren konfigurazioa, tentsio-aldaketak detektatu ahal izateko
(pultsadoreak sakatzean, alegia). Horretarako, etenak gaitu behar dira.

2) CN sarreren zerbitzu-errutina.
Kasu honetan, S6 (RD7) pultsadorearen tratamendua:
S6 sakatzean, D7 LEDaren egoera kommutatu behar da:
piztuta baldin bazegoen, itzali,
eta itzalita baldin bazegoen, piztu.

Egileak: Iker Galardi eta Unai Fernandez
Data: Maiatza 2021
*/

#include "p24HJ256GP610A.h"
#include "commons.h"
#include "memoria.h"
#include "CN.h"
int adc_uart = 1;
int cotadoredoCN = 0;
int lerro_indize = 0;
int safe_pos = 0;
int token = 0;
int safe_eg = 0;

// 1) Konfigurazioa:
//==================
void konfiguratu_CN()
{
	//etenen bidez tratatu nahi ditugun pultsadoreen etenak gaitu:
	CNEN2bits.CN16IE = 1;
	CNEN2bits.CN19IE = 1;
	CNEN2bits.CN23IE = 1;
	CNEN1bits.CN5IE = 1; //Joiystick-eko pultsadorearen etenak gaitu.

	//S6 pultsadorearen etena gaitzeko (CN16 sarrera, CNEN2 erregistroan)
	//gogoan izan: CNEN1 erregistroan CN0 - CN15 sarrerak, CN0IE - CN15IE bitetan
	//gogoan izan: CNEN2 erregistroan CN16 - CN23 sarrerak, CN16IE - CN23IE bitetan

	IEC1bits.CNIE = 1; //CN sarreren etenak gaitzeko (sarrerako tentsioaren aldaketa detektatzeko)

	IFS1bits.CNIF = 0; //Ezabatu CN eten-eskaeren adierazlea
}

//2) Etenaren zerbitzu-errutina:
//==============================
void _ISR_NO_PSV _CNInterrupt()
{
	cotadoredoCN++;

	//================= UART eta ADC artean aldatu ==================

	if (!PORTDbits.RD7) // S6 pultsadorea sakatzerakoan
	{
		Nop();
		Nop();
		Nop();
		Nop();
		if (adc_uart == 1)
			adc_uart = 0;
		else
			adc_uart = 1;
		Nop();
		Nop();
		Nop();
		Nop();
	}

	//========================= scroll ===========================
	// Goruntz joateko
	if (!PORTDbits.RD13)
	{
		Nop();
		Nop();
		Nop();
		Nop();
		lerro_indize = (lerro_indize + 1) % lerro_kop;
		Nop();
		Nop();
		Nop();
		Nop();
	}
	// Behera joateko

	if (!PORTAbits.RA7)
	{
		// Lehen puntuan badago, lerro indizea azkenera jarri
		Nop();
		Nop();
		Nop();
		Nop();
		if (lerro_indize != 0)
		{
			lerro_indize = (lerro_indize - 1) % lerro_kop;
		}
		else
		{
			lerro_indize = lerro_kop - 1;
		}
		Nop();
		Nop();
		Nop();
		Nop();
	}

	//===================== Joystick-eko botoia, babes posiziora joateko =======================

	if (!PORTBbits.RB3)
	{
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		safe_pos = 1;
		safe_eg = 0;
		adc_uart = 2;

		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
		Nop();
	}

	IFS1bits.CNIF = 0; //jarri berriro 0ra CN eten-eskaeren adierazlea
}
