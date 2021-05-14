
#define lerro_kop 24

// Aldagaiak
extern const char Testua_LCD_1[16],Testua_LCD_2[16], Testua_LCD_3[16],Testua_LCD_4[16], Testua_LCD_5[16],Testua_LCD_6[16];
extern const char * board_info;
extern unsigned char Leihoa_LCD[lerro_kop][18];

// Funtzioak

/*
 *  Flash RAM-era informazioa kopiatzeko
 */
void Kopiatu_FLASH_RAM (const char *testua);

