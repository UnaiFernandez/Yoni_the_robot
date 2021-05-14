/*
   UART2 moduluarekin lan egiteko definizioak eta funtzioak

   1) UART hasieratzeko funtzioa.
   2) UART-aren datu jasotzearen eten kudeatzailea. Hemen ondorego funtzionamendua
   inplementatzen da:
 * g tekla zapaltzean denboragailua gelditu
 * m tekla zapaltzean denboragailua martxan jarri
 * h tekla zapaltzean denboragailua hasieratu egiten da
 * r tekla zapaltzean serbomotorea, eskuinera mugitzen du
 * l tekla zapaltzean serbomotorea, ezkerrera mugitzen da.
 3) UART-aren bidalketa etenaren kudeatzailea. Hemen ondorengo automata dago 
martxan: 
 * Lehen egoera: Egoera honetan home "komandoa" bidaltzen da PC-an
 kursorea pantilaren hasieran jartzeko. Ondoren,
 bigarren egoerara egiten da salto.

 * Bigarren egoera: Egoera honeta lehen lerroa bidaltzen da PC-ra, eta 
 ondoren hirugarren negoerara egiten da salto

 * Hirugarren egoera: Egoera honetan hurrengo lerrora mugitzen da 
 kursorea, hau egin ondorenn laugarren egoerara
 egiten da salto.

 * Laugarren egoera: Azken egoera honetan bigarren lerroa, eta ondoren,
 lehen egoerara egite da salto.

 4) Bidali datu bat UART2 modulutik (inkestaren bidez)
 5) Itxaron datu bat UART2 moduluan (inkestaren bidez)
 6) Leihoa_LCD egituraren informazioa bidali PCra UART bitartez
 7) Parametroak adierazten duen tekla kopurua jaso UART bitartez
 Gero bistara daitezen, Leihoa_LCD egituran gorde tekla horiek
 8) Prozedura honek hiru kode bidaltzen dizkio PCari pantailaren hasieran
 kokatzeko kurtsorea

Egileak: Iker Galardi eta Unai Fernandez
Data: martxoa 2021
*/

#include "p24HJ256GP610A.h"
#include "timers.h"
#include "commons.h"
#include "memoria.h"
#include "OC_PWM.h"
#include "CN.h"

// UART2 moduluaren hankatxoekin lotutako definizioak
//#define UART2_TX_TRIS   TRISFbits.TRISF5
//#define UART2_RX_TRIS   TRISFbits.TRISF4

// PCko pantailarekin lan egiteko zenbait definizio
#define clrscr "\x1b[2J" // 4 character: \x1b, [, 2, J
#define home "\x1b[H"	 // 3 character: \x1b, [, H
#define CR 0x0D			 // carriage return
#define LF 0x0A			 // line feed

// Transmisio-abiadurarekin lotutako definizioak
#define Fosc 50000000	// CPUaren erloju-abiadura (oscillator) (50MHz)
#define Fcy Fosc / 2	// aginduen exekuzio-abiadura
#define BAUDRATE2 38400 // transmisio-abiadura (baudetan)
#define BIT1DENB_US (1000000 / BAUDRATE2) + 1

#define BAUD_RATEREG_2_BRGH1 ((Fcy / BAUDRATE2) / 4) - 1  // BRGH = 1 denean
#define BAUD_RATEREG_2_BRGH0 ((Fcy / BAUDRATE2) / 16) - 1 // BRGH = 0 denean

int egoeraUART = 0;
//1) Hasieratu UART2:
//===================
void prestatu_UART2()
{
	// Transmisio-abiadura
	// Hurrengo bi lerroetatik, aukeratu bat, BRGH balioaren arabera
	U2BRG = BAUD_RATEREG_2_BRGH1;
	//U2BRG = BAUD_RATEREG_2_BRGH0;

	// U2MODE: modulua gaitu (UARTEN), 8bit, paritaterik gabe (PDSEL)
	// 1 stop bit (STSEL), BRGH ...
	U2MODE = 0;			 //
	U2MODEbits.BRGH = 1; //1 jarrita BRG1 erabiki 0 bada BRGH0

	// U2STA: bidalketarako eten-modua finkatu (UTXISEL),
	// bidalketak gaitu (UTXEN), hartzailearen eten-modua finkatu (URXISEL)
	U2STA = 0;

	// etenak eta flagak hasieratu: IE eta IF flagak
	IEC1bits.U2RXIE = 1; //etenen bidez kudeatzeko 1 (hartzailea)
	IEC1bits.U2TXIE = 1; //etenen bidez kudeatzeko 1 (igorlea)
	IFS1bits.U2RXIF = 0;
	IFS1bits.U2TXIF = 0;
	IPC7bits.U2RXIP = 4; //defektuz lehentasuna 4 (erdiko balio bat)
	IPC7bits.U2TXIP = 4;

	// erroreen etenak + bug
	IEC4bits.U2EIE = 0;
	U2STAbits.OERR = 0;

	// Gaitu UART2 eta UART2_TX. Beti bukaeran eta ordena horretan!!
	U2MODEbits.UARTEN = 1; // UART2 gaitu
	U2STAbits.UTXEN = 1;   // transmisioa gaitu. UARTmodula gaitu ondoren

	Delay_us(BIT1DENB_US); // Bit bat bidaltzeko denbora itxaron
}
//2) UART hartzailea:
//===================
void _ISR_NO_PSV _U2RXInterrupt()
{
	switch (U2RXREG)
	{
	case 'b':
	case 'B':
		T4CONbits.TON = 0;
		break;
	case 'm':
	case 'M':
		T4CONbits.TON = 1;
		break;
	case 'h':
		hasieratu_krono();
		Leihoa_LCD[1][7] = '-';
		Leihoa_LCD[1][8] = '-';
		Leihoa_LCD[1][10] = '-';
		Leihoa_LCD[1][11] = '-';
		Leihoa_LCD[1][13] = '-';
		Leihoa_LCD[1][14] = '-';
	case 'H':
		hasieratu_krono();
		Leihoa_LCD[1][7] = '-';
		Leihoa_LCD[1][8] = '-';
		Leihoa_LCD[1][10] = '-';
		Leihoa_LCD[1][11] = '-';
		Leihoa_LCD[1][13] = '-';
		Leihoa_LCD[1][14] = '-';
		break;
	case 'q':
	case 'Q':
		if (adc_uart == 1)
		{
			if (duty + 20 < T_MAXPWM1)
				duty += 20;
		}
		break;
	case 'a':
	case 'A':
		if (adc_uart == 1)
		{
			if (duty - 20 > T_MINPWM1)
				duty -= 20;
		}
		break;
	case 'w':
	case 'W':
		if (adc_uart == 1)
		{
			if (duty2 + 20 < T_MAXPWM1)
				duty2 += 20;
		}
		break;
	case 's':
	case 'S':
		if (adc_uart == 1)
		{
			if (duty2 - 20 > T_MINPWM1)
				duty2 -= 20;
		}
		break;
	case 'e':
	case 'E':
		if (adc_uart == 1)
		{
			if (duty3 + 20 < T_MAXPWM1)
				duty3 += 20;
		}
		break;
	case 'd':
	case 'D':
		if (adc_uart == 1)
		{
			if (duty3 - 20 > T_MINPWM1)
				duty3 -= 20;
		}
		break;
	case 'r':
	case 'R':
		if (adc_uart == 1)
		{
			if (duty4 + 20 < T_MAXPWM1)
				duty4 += 20;
		}
		break;
	case 'f':
	case 'F':
		if (adc_uart == 1)
		{
			if (duty4 - 20 > T_MINPWM1)
				duty4 -= 20;
		}
		break;
	case 't':
	case 'T':
		if (adc_uart == 1)
		{
			if (duty5 + 20 < T_MAXPWM1)
				duty5 += 20;
		}
		break;
	case 'g':
	case 'G':
		if (adc_uart == 1)
		{
			if (duty5 - 20 > T_MINPWM1)
				duty5 -= 20;
		}
		break;
	}
	IFS1bits.U2RXIF = 0;
}

//3) UART igorlea:
//================
void _ISR_NO_PSV _U2TXInterrupt()
{
	static unsigned int ler = 0;
	static unsigned int ind = 0;
	switch (egoeraUART)
	{
	case 0:
		if (ind == 0)
		{
			U2TXREG = '\x1b';
			ind++;
		}
		else if (ind == 1)
		{
			U2TXREG = '[';
			ind++;
		}
		else
		{
			U2TXREG = 'H';
			egoeraUART = 1;
			ind = 0;
		}
		break;
	case 1:
		U2TXREG = Leihoa_LCD[ler][ind];
		ind++;
		if (ind == 18)
		{
			ler++;
			ind = 0;
			if (ler == lerro_kop)
			{
				egoeraUART = 0;
				ler = 0;
			}
			else
				egoeraUART = 1;
		}
		break;
	}
	IFS1bits.U2TXIF = 0;
}

//4) Bidali datu bat UART2 modulutik (inkestaren bidez)
//=====================================================
void put_UART2(char c)
{
	U2TXREG = c;
	while (!IFS1bits.U2TXIF)
		;
	IFS1bits.U2TXIF = 0;
}

//5) Itxaron datu bat UART2 moduluan (inkestaren bidez)
//=====================================================
char get_UART2()
{
	//Osatu beharreko funtzioa
	while (!IFS1bits.U2RXIF)
		;
	IFS1bits.U2RXIF = 0;
	return U2RXREG; // Itzuli hartutako datua
}

//6) Leihoa_LCD egituraren informazioa bidali PCra UART bitartez
//==============================================================
void bidali_UART2_leihoa()
{
	int i, j;
	for (i = 0; i < 7; i++)
	{
		for (j = 0; j < 16; j++)
		{
			put_UART2(Leihoa_LCD[i][j]);
		}
		//hurrengo lerrora salto
		put_UART2(CR);
		put_UART2(LF);
	}
}

//7) Teklak jaso:
//===============
// Parametroak adierazten duen tekla kopurua jaso UART bitartez
// Gero bistara daitezen, Leihoa_LCD egituran gorde tekla horiek
void jaso_teklak(unsigned int zenbat)
{
	int i;
	for (i = 0; i < zenbat; i++)
		Leihoa_LCD[0][i] = get_UART2();
}

//8) Home posizioan kokatu.
//=========================
// Prozedura honek hiru kode bidaltzen dizkio PCari pantailaren hasieran
// kokatzeko kurtsorea
void UART_home()
{
	put_UART2('\x1b');
	put_UART2('[');
	put_UART2('H');
}
