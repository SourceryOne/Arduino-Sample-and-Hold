
// Define the pin numbers
const int trigIn = A3;  // (A6 on the PCB) Pin connected to the button
//const int clipLed = 13;     // Pin connected to the LED
const int trigLed = A5;     // Pin connected to the LED
const int signalInput = A6; // Input signal to sample or track.
const int sampleOrTrackPin = A4; //switch between 5v and GND to decide sample or track.
const int intOrExtPin = A0;
//const int bypassSignalPin = for now just use the trigLed?

//variable to store the current sample
int sample;
int highCut=1023;
int lowCut=0;
int timeToReadPots=0; //only reads the low and high pots once every 50 cycles.
int intOrExt=1; //if we are reading from input or using internal noise source 0=internal, 1=external... Read some pin.
int sampleOrTrack=0; //if we are in sample or track mode. 0=sample, 1=track
//int gndCountSample=0; //counter to check if we are in sample or track mode.

void setup() {
  // Set the appropriate pins as output for port manipulation
  DDRD = 0b11111111;
  DDRB = 0b11111111;
  pinMode(sampleOrTrackPin, INPUT_PULLUP);
  pinMode(intOrExtPin, INPUT_PULLUP);
  pinMode(trigIn, INPUT_PULLUP);
  
  //start random/noise
  //randomSeed(random(1023));
}  
  
void loop() {
  //Check the pots for value range
  if (timeToReadPots>=50) {
    lowCut = analogRead(A1);
    highCut = analogRead(A2);
    intOrExt=digitalRead(intOrExtPin);
    timeToReadPots=0;
  } else {
    timeToReadPots++;
  }
  
  //Check if we should use internal noise or signal input.
  if (intOrExt == LOW) { //just check directly on pin.
    //Reading from external source
    //Below reads the signal and maps it to low and highcut in one go.
    sample=map(0,1023,lowCut,highCut,analogRead(signalInput)); 
  } else {
    //Using internal noise source
    sample=random(lowCut,highCut);
  }
  //================= TRIG ================
  if (digitalRead(trigIn) == LOW) {
    //We have a trig! Lets lock the sample value and send it.
    //light up the trigLed! We have a trig!
    digitalWrite(trigLed, HIGH);
    
    //send the sample to the DAC
    PORTD = lowByte(sample);
    PORTB = highByte(sample);

    while (digitalRead(trigIn) == LOW) {
      // Do nothing, except check if the button is still pressed
    }  
  } else {    
  //================= NO TRIG ================
    //There is no trig...
    digitalWrite(trigLed, LOW); //no more trig...
    if (digitalRead(sampleOrTrackPin)==LOW) {
      //============= TRACKMODE ==============
      //Set the ...pin that outputs the input signal directly.
      //ACTUALLY ALSO IS IT INTERNAL OR EXT SIGNAL!
      //let the external signal passthrough (for now through the arduino, later with a bypass)
      //send the sample to the DAC
      PORTD = lowByte(sample);
      PORTB = highByte(sample);
    } 
  }
}
