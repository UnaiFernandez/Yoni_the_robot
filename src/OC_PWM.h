
// Aldagaiak eta funtzioak erazagutu beste moduluetatik erabiltzeko

//KONSTANTEAK (.c fitxategian dauden define berak dira)
//=========================================================  
#define MINPWM 0.5     // pultsuaren iraupen minimoa (milisegundo)
#define MAXPWM 2.5     // pultsuaren iraupen maximoa (milisegundo)

#define T20ms  62500 		 // Timerraren periodoa, 20 ms iraun dezan

#define T_MINPWM (T20ms/20)*MINPWM    // pultsuaren iraupen minimoa (ticks)
#define T_MAXPWM (T20ms/20)*MAXPWM    // pultsuaren iraupen maximoa (ticks)

#define T_MINPWM1 1848    // pultsuaren iraupen minimoa, OC1-en kasuan (ticks)
#define T_MAXPWM1 7727    // pultsuaren iraupen minimoa, OC1-en kasuan (ticks)


#define S1_SAFE 4603
#define S2_SAFE 7737
#define S3_SAFE 6859
#define S4_SAFE 3338
#define S5_SAFE 4287

extern int PWM_eg, duty, duty2, duty3, duty4, duty5;

//FUNTZIOAK
//=========================================================  

/*
 *  OC moduluak eta PWM hasieratzeko
 */
void prestatu_OC1 ();

/*
 *  Duty-ak bistaratzeko
 */
void bistaratu_duty();
