#include <EEPROM.h> // Include EEPROM library for saving selected function


//constants edit for functionality:
const int cyclesBetweenReadPots=50; //how often the pots max and min should be checked, and intorext switch.
//const int numberOfFunctions=2; //so counting the main sample and hold and all other functions.
const byte eepromAddress=0; //the place in the eeprom to save the selectedFunction, if changing too often (100000+ times) then change this adress because that address might have been worned out ;)
const unsigned int maxValue=1023;
// Timing constants (in milliseconds)
const unsigned long holdTime = 2000; // Time to select alternateFunctions
//const unsigned long holdTime2 = 5000; // Time to select alternateFunction 2 (5 seconds)
//const unsigned long holdTime3 = 8000; // Time to select mainFunction (8 seconds)
//const unsigned long holdTime4 = 11000; // Time to select mainFunction (11 seconds), selected at start. 

// Define the pin numbers
const int trigIn = A3;  // Pin connected to the button and the trig in jack
//const int clipLed = 13;     // Pin connected to the LED
const int trigLed = A5;     // Pin connected to the LED
const int signalInput = A6; // Input signal to sample or track.
const int sampleOrTrackPin = A0; //switch between 5v and GND to decide sample or track.
const int intOrExtPin = A4;
//unsigned long startTime = 0;  // Stores start time of button press

//const int bypassSignalPin = for now just use the trigLed?
//variables to read selected from start from EEPROM!
byte alternateCodeFromEEPROM;    //to make sure we dont write to eeprom too often.

//variable to store the current sample
int sample;
int highCut=1023;
int lowCut=0;
int timeToReadPots=0; //only reads the low and high pots once every 50 cycles.
int intOrExt=1; //if we are reading from input or using internal noise source 0=internal, 1=external... Read some pin.
int sampleOrTrack=0; //if we are in sample or track mode. 0=sample, 1=track
//int gndCountSample=0; //counter to check if we are in sample or track mode.
byte alternateCode=0;


//variables for sample and slide.
unsigned long lastPulseTime = 0; // Stores the time of the last trigger pulse
unsigned long pulseInterval = 1000; // Default interval between pulses (ms)
int currentValue = 0;  // Current output value that gradually changes
int targetValue = 0;   // Target value that the output slides towards
int lastTrigState=0; //checking if the trig is active or not.
unsigned long currentTime = millis();
int lastValue; //last value for the slide to slide FROM.
float slideFactor = 1.0;        // Factor controlling slide duration (0.0 to 1.0)


//Variables for LFO and AD Envelope Generator
byte envelopePhase=0;
int rampDuration;
unsigned long startTime=0;
unsigned int ledStartTime=0;

//Variables for the HiHat
int filterCounter=0;
byte hihatfilter;
byte newTrig=1; //to make sure you can make a new trig as soon as you let go of the HOLD.

void setup() {
  //Serial.begin(115200);
  // Set the appropriate pins as output for port manipulation
  DDRD = 0b11111111;
  DDRB = 0b11111111;
  pinMode(sampleOrTrackPin, INPUT_PULLUP);
  pinMode(intOrExtPin, INPUT_PULLUP);
  pinMode(trigIn, INPUT_PULLUP);

  alternateCode = EEPROM.read(eepromAddress);
  alternateCodeFromEEPROM=alternateCode; //to make sure we dont write to eeprom without needing to.
  if (alternateCode < 0 || alternateCode > 5) {
    alternateCode = 0; // Default to Main Function if the value is invalid
  }
  //start random/noise
  //randomSeed(random(1023));
}  
