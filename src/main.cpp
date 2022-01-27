#include "Arduino.h"
#define FILM_PIN 2

// Stepper
#define DIR_PIN 4
#define STEP_PIN 5
#define ENABLE_PIN 6
#define STEPS 199

// IR Remote
#define IRLED 11
#define DELAY_TIME 7500
#define INTERVAL_TIME 1000

void setup() {
	
	//Pin setup
	pinMode(IRLED, OUTPUT);			// IR Transmitter
	pinMode(ENABLE_PIN, OUTPUT);	// Enable
	pinMode(DIR_PIN, OUTPUT);		// Richtung
	pinMode(STEP_PIN, OUTPUT);		// Step

	digitalWrite(ENABLE_PIN, LOW);	// Stepper anschalten
	digitalWrite(DIR_PIN, HIGH);	// Stepper im Uhrzeigersinn
}

/**
 * @param number number of photos to be taken
 */
void takePhoto(int number) {
	for(int i = 0; i < number; i++) {
		takePicture();				// take the picture
		delay(INTERVAL_TIME);		// delay in milliseconds which allows us to do timelapse photography - 1 second = 1000 milliseconds
	}
}

/**
 * adds the carrier of the IR signal.
 * @param pulseTime the length of the pulse in mikroseconds
 */
void pulseON(int pulseTime) {
	unsigned long endPulse = micros() + pulseTime;	// create the microseconds to pulse for
	while(micros() < endPulse) {
		digitalWrite(IRLED, HIGH);		// turn IR on
		delayMicroseconds(13);			// half the clock cycle for 38Khz (26.32×10-6s) - e.g. the 'on' part of our wave
		digitalWrite(IRLED, LOW);		// turn IR off
		delayMicroseconds(13);			// delay for the other half of the cycle to generate wave/ oscillation
	}
}

/**
 * the counterpart to pulseON
 * @param pulseTime the length of the pulse in mikroseconds
 */
void pulseOFF(int pulseTime) {
	unsigned long endDelay = micros() + pulseTime;	 // create the microseconds to delay for
	while(micros() < endDelay);
}

/**
 * trigger the camera
 */
void takePicture() {
	for (int i=0; i < 2; i++) {	// loop the signal twice.
		pulseON(2000);			// pulse for 2000 uS (Microseconds)
		pulseOFF(27850);		// turn pulse off for 27850 us
		pulseON(400);			// and so on
		pulseOFF(1580);
		pulseON(400);
		pulseOFF(3580);
		pulseON(400);
		pulseOFF(63200);
	}
}

void stepperMotor () {
	for(int stepCounter = 0; stepCounter < STEPS; stepCounter++) {
		digitalWrite(STEP_PIN,HIGH);
		delayMicroseconds(500);
		digitalWrite(STEP_PIN,LOW);
		delayMicroseconds(500);
	}
}

/**
 * @return true when film ended, false else
 */
bool lichtSchranke () {
	bool film = digitalRead(FILM_PIN); 
	return film;
}

void loop () {
	bool film = lichtSchranke ();
	if (film) {
		stepperMotor();
		takePhoto(1);
	}
}