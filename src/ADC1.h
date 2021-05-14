//  Aldagien erazagupena beste moduluetatik erabiltzeko
extern int adc_lag;
extern int duty_lag;
extern int duty_lag2;
extern int smooth_adc_eg;

// Funtzioen erazagupena beste moduluetatik erabiltzeko

/**
 * ADC1 modulua hasieratzeko funtzioa
 */
void prestatu_ADC1 ();

/**
 * Laginketa egiten hasteko funtzioa 
 */
void laginketa_hasi ();

/**
 * Lortutako balio analogikoak eskuratzeko eta gordetzeko funtzioa
 */
void jaso_ADC1balioa ();


