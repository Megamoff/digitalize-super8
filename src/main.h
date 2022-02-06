void setup();

/**
 * @param number number of photos to be taken
 */
void captureFrame(int number);

/**
 * adds the carrier(~38kHz) of the IR signal.
 * @param pulseTime the length of the pulse in mikroseconds
 */
void pulseOn(int pulseTime);

/**
 * the counterpart to pulseOn
 * @param pulseTime the length of the pulse in mikroseconds
 */
void pulseOff(int pulseTime);

/**
 * trigger the camera
 */
void takePhoto();

void nextFrame();

void loop ();