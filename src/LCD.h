// Funtzioak

extern int pantaila_lehen_lerroa;

/**
 * LCD hasieratzeko funztioa
 */
void Init_LCD();	

/**
 * LCD komandoetarako azpierrutina
 */
void lcd_cmd(char cmd);	

/**
 * LCD-ko datuetarako azpierrutina 
 */
void lcd_data(char data);

/**
 * LCD-ko datuak pantailaratzeko 
 */
void puts_lcd (unsigned char *data, unsigned char count);

/**
 * Kustsorea LCD-aren lehenengo lerroan kokatzeko funtzioa
 */
void line_1();

/**
 * Kurtsorea LCD-aren bigarren lerroan kokatzeko funtzioa
 */
void line_2();

