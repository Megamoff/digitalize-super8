#include "Arduino.h"
#include "main.h"

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
	pinMode(IRLED, OUTPUT);			// IR Transmitter
	pinMode(ENABLE_PIN, OUTPUT);	// Enable
	pinMode(DIR_PIN, OUTPUT);		// Richtung
	pinMode(STEP_PIN, OUTPUT);		// Step

	digitalWrite(ENABLE_PIN, LOW);	// Enable stepper
	digitalWrite(DIR_PIN, HIGH);	// Stepper will turn clockwise
}

void captureFrame(int number) {
	for(int i = 0; i < number; i++) {
		takePhoto();			// take the picture
		delay(INTERVAL_TIME);	// delay in milliseconds which allows us to do timelapse photography
	}
}

void pulseOn(int pulseTime) {
	unsigned long endPulse = micros() + pulseTime;	// create the microseconds to pulse for
	while(micros() < endPulse) {
		digitalWrite(IRLED, HIGH);
		delayMicroseconds(13);
		digitalWrite(IRLED, LOW);
		delayMicroseconds(13);
	}
}

void pulseOff(int pulseTime) {
	unsigned long endDelay = micros() + pulseTime;	 // create the microseconds to delay for
	while(micros() < endDelay);
}

void takePhoto() {
	for (int i=0; i < 2; i++) {	// loop the signal twice.
		pulseOn(2000);
		pulseOff(27850);
		pulseOn(400);
		pulseOff(1580);
		pulseOn(400);
		pulseOff(3580);
		pulseOn(400);
		pulseOff(63200);
	}
}

void nextFrame() {
	for(int stepCounter = 0; stepCounter < STEPS; stepCounter++) {
		digitalWrite(STEP_PIN, HIGH);
		delayMicroseconds(1000);
		digitalWrite(STEP_PIN, LOW);
		delayMicroseconds(1000);
	}
}

bool lichtSchranke() {
	bool film = digitalRead(FILM_PIN); 
	return film;
}

void loop () {
	bool film = lichtSchranke();
	if(film) {
		nextFrame();
		captureFrame(1);	// here you can change how many Photos to make of every frame
	}
}