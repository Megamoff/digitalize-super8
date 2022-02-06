#include "Arduino.h"
#include "main.h"

#define LIGHT_BARRIER_PIN 2
#define FILM_PIN A0

// Stepper
#define DIR_PIN 4
#define STEP_PIN 5
#define ENABLE_PIN 6
#define STEPS 190

// IR Remote
#define IRLED 11
#define INTERVAL_TIME 1000

void setup() {
	Serial.begin(9600);
	pinMode(IRLED, OUTPUT);			// IR Transmitter
	pinMode(ENABLE_PIN, OUTPUT);	// Enable
	pinMode(DIR_PIN, OUTPUT);		// Richtung
	pinMode(STEP_PIN, OUTPUT);		// Step

	pinMode(LIGHT_BARRIER_PIN, INPUT);

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
	long endPulse = micros() + pulseTime;	// create the microseconds to pulse for
	while(micros() < endPulse) {
		digitalWrite(IRLED, HIGH);
		delayMicroseconds(13);
		digitalWrite(IRLED, LOW);
		delayMicroseconds(13);
	}
}

void pulseOff(int pulseTime) {
	long endDelay = micros() + pulseTime;	 // create the microseconds to delay for
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
	// 199 Steps are one full revolution. We do 190 and until the shutter is open again.
	// This prevents desyncronization by missed steps
	for(int stepCounter = 0; stepCounter < STEPS || digitalRead(LIGHT_BARRIER_PIN); stepCounter++){
		digitalWrite(STEP_PIN, HIGH);
		delayMicroseconds(1000);
		digitalWrite(STEP_PIN, LOW);
		delayMicroseconds(1000);
	}
}

void loop () {
	int film = analogRead(FILM_PIN);
//	Serial.println(film);	// print brightness for adjustment
	if(film > 150) {
		nextFrame();
		captureFrame(1);	// here you can change how many Photos to make of every frame
	}
}