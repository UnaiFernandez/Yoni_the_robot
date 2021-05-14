/**********************************************************************
  LCD erabiltzeko funtzioak:
  Hasieraketa
  Komando bat bidali
  Datu bat bistaratu
  Testu bat bistaratu
  Funtzio bereziak: 1. lerroan kokatu // 2. lerroan kokatu
 **********************************************************************/

#include "p24HJ256GP610A.h"
#include "timers.h"

// Kontrol seinaleen pinak
#define RW LATDbits.LATD5
#define RS LATBbits.LATB15
#define E LATDbits.LATD4

// Kontrol seinaleen norabidea
#define RW_TRIS TRISDbits.TRISD5
#define RS_TRIS TRISBbits.TRISB15
#define E_TRIS TRISDbits.TRISD4

// Datu seinaleak
#define DATA LATE
#define DATAPORT PORTE
#define TRISDATA TRISE

/*****	LCD SUBROUTINES  *****/
void lcd_cmd(char cmd) // subroutiune for lcd commands
{
    RW = 0;         // RW 0ra jarri, idazketa
    RS = 0;         // RS 0ra jarri, komando idazteko
    DATA &= 0xFF00; // Ezabatu datuaren 8 bitak (prepare RD0 - RD7)
    DATA |= cmd;    // Komandoa (parametroan) kopiatu (command byte to lcd)
    E = 1;          // E aktibatu (toggle E line)
    Nop();          // 10 NOP. ZERTARAKO??
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    E = 0;  // E desaktibatu
    RW = 1; // Idazketa desaktibatu
}

void lcd_data(char data) // subroutine for lcd data
{
    RW = 0;         // RW 0ra jarri, idazketa
    RS = 1;         // RS 1era jarri, datua idazteko
    DATA &= 0xFF00; // Ezabatu datuaren 8 bitak (prepare RD0 - RD7)
    DATA |= data;   // Datua (parametroan) kopiatu (data byte to lcd)
    E = 1;          // E aktibatu (toggle E line)
    Nop();          // 10 NOP. ZERTARAKO
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    E = 0;  // E desaktibatu
    RS = 0; // RS desaktibatu
    RW = 1; // Idazketa desaktibatu
}

void Init_LCD() // initialize LCD display
{
    // 15mS delay after Vdd reaches nnVdc before proceeding
    // with LCD initialization
    // not always required and is based on system Vdd rise rate
    Delay_ms(15); // 15 ms delay

    // Hasieratu LCD-aren erregistroak
    DATA &= 0xFF00;
    RW = 0;
    RS = 0;
    E = 0;

    // Irteera moduan jarri kontrol pinak
    TRISDATA &= 0xFF00;
    RW_TRIS = 0;
    RS_TRIS = 0;
    E_TRIS = 0;

    // Hasieraketa komandoak
    lcd_cmd(0x38); // function set (3 aldiz)
    Delay_ms(5);   // 5 ms delay
    lcd_cmd(0x38);
    Delay_us(100); // 100 us delay
    lcd_cmd(0x38);
    Delay_us(40); // ???? delay (taulan begiratu)
    lcd_cmd(0x38);
    Delay_us(40);  // ???? delay (taulan begiratu)
    lcd_cmd(0x0C); // Display on/off control, cursor blink off (0x0C)
    Delay_us(40);  // ???? delay (taulan begiratu)
    lcd_cmd(0x06); // Entry mode set (0x06)
    Delay_us(40);  // ???? delay (taulan begiratu)
}

void puts_lcd(unsigned char *data, unsigned char count)
{
    while (count)
    {
        lcd_data(*data++);
        Delay_us(40);
        count--;
    }
}

void line_1()
{
    lcd_cmd(0x80); // Set DDRAM address (@0)
    Delay_us(40);  // ???? delay (taulan begiratu)
}

void line_2()
{
    lcd_cmd(0xC0); // Set DDRAM address (@40)
    Delay_us(40);  // ???? delay (taulan begiratu)
}
