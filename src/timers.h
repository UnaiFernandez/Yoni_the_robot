extern int tdist, dist_prest;

//_________________ Timer Funtzioak ________________

/**
 * T4 timerra hasieratzeko funtzioa 
 */
void hasieratu_timer4 ();

/**
 * T2 timerra hasieratzeko funtzioa
 */
void hasieratu_timer2 ();

/**
 * T6 timerra hasieratzeko funtzioa
 */
void hasieratu_timer6 ();

/**
 * T9 timerra hasieratzeko funtzioa
 */
void hasieratu_timer9 ();


/**
 * T3 timerra hasieratzeko funtzioa
 */
void hasieratu_timer3();

/**
 * Kronometroan erabili beharko ditugun aldagaien hasieraketa
 */
void hasieratu_krono();

/**
 * ms balio bat emanda erlojua konfiguratu egiten da, denbora hori neurtzeko
 * 
 * @param ms
 */
void Delay_ms(uint16_t ms);

/**
 * us parametroa emanda, erlojua konfiguratuko du, denbora hori neurtzeko
 * 
 * @param us
 */
void Delay_us(uint16_t us);

/**
 * Kronometroaren funtzioa. lehen hasieratutako aldagaiak eguneratzen doa periodikoki
 */
void kronometroa();
