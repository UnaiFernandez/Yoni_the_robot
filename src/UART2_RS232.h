/*
 ** UART2_RS232.h
 */

// funtzioak

/*
 * UART hasieratzeko
 */
void prestatu_UART2();

/*
 *  Bidali datu bat UART-etik.   
 */
void bidali_UART2_leihoa ();

/*
 *  Itxaron datu bat jasotzera UART-etik.
 */
void jaso_teklak (unsigned int zenbat);

/*
 *  Home posizioan kokatu.
 */
void UART_home ();

