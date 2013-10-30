/*
 * servo.c
 *
 * Created: 30.10.2013 15:40:31
 *  Author: magnealv
 */ 
/*
void servo_init(){
	
	// OC1A set to output
	DDRB |= (1<<PB5);
	
	// non-inverting mode for OC1A
	TCCR1A |= (1<<COM1A1) | (1<<WGM11);
	
	// Mode 14
	TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<WGM11);
	
	ICR1 = 20000;
	
	//Enabling TimerInterrupt
	TIMSK= (1 << TOIE1);
	
	OCR1A = (dutycycle)/100.0)*255.0;
	
	sei();
	
	//prescaler 8
	TCCR1B |= (1<<CS11);

}

*/