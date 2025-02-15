
// Define the pin numbers
const int trigIn = A3;  // Pin connected to the button and the trig in jack
//const int clipLed = 13;     // Pin connected to the LED
const int trigLed = A5;     // Pin connected to the LED
const int signalInput = A6; // Input signal to sample or track.
const int sampleOrTrackPin = A0; //switch between 5v and GND to decide sample or track.
const int intOrExtPin = A4;
//unsigned long startTime = 0;  // Stores start time of button press

bool debug = true;
//const int bypassSignalPin = for now just use the trigLed?

//variable to store the current sample
int sample;
int highCut=1023;
int lowCut=0;
int timeToReadPots=0; //only reads the low and high pots once every 50 cycles.
int intOrExt=1; //if we are reading from input or using internal noise source 0=internal, 1=external... Read some pin.
int sampleOrTrack=0; //if we are in sample or track mode. 0=sample, 1=track
//int gndCountSample=0; //counter to check if we are in sample or track mode.
bool alternateCode=0;

void setup() {
  // Set the appropriate pins as output for port manipulation
  DDRD = 0b11111111; //DDRD = DDRD | B11111100; 
  DDRB = 0b11111111;
  pinMode(sampleOrTrackPin, INPUT_PULLUP);
  pinMode(intOrExtPin, INPUT_PULLUP);
  pinMode(trigIn, INPUT_PULLUP);

    if (debug) {
    Serial.begin(57600);
    Serial.println(F("Started"));
  }
  
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


//MAIN FUNCTION
if (alternateCode == 0) {
  //Check if we should use internal noise or signal input.
  if (intOrExt == HIGH) { //just check directly on pin.
    //Reading from external source
    //Below reads the signal and maps it to low and highcut in one go.
    sample=map(analogRead(signalInput), 0,1023,lowCut,highCut); 
    Serial.println(sample);
  } else {
    //Using internal noise source
    sample=random(lowCut,highCut);
  }

  //Serial.println(sample);
  
  //================= TRIG ================
  if (digitalRead(trigIn) == LOW) {
    //We have a trig! Lets lock the sample value and send it.
    //light up the trigLed! We have a trig!
    digitalWrite(trigLed, HIGH);
    
    //send the sample to the DAC
    PORTD = lowByte(sample);
    PORTB = highByte(sample);

    while (digitalRead(trigIn) == LOW) {
        //CHECK TO SEE IF WE GO INTO ALTERNATE FUNCTION.
        unsigned long pressDuration = getButtonPressDuration();
        if (pressDuration > 5000) {
          // Button held for more than 5 seconds
          if (lowCut == 0 && highCut== 0 ) {
            alternateCode=1;
            digitalWrite (trigLed, LOW);
            delay(1000);
          }
        }
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




 //ALTERNATE FUNCTION - happens if both lowCut and highCut is set to 0 and hold is pushed for 5 seconds.
  } else { 
    //digitalWrite (trigLed, HIGH); //Just to show we are in alternate mode. 
    /*Suggestions for alternate functions:
      - 1. Random gates, either totally random, with one of the knobs setting the probability of a gate.
      - 2. Random gates, bernoulli type, input on the hold for example, and one pot deciding if the gate should go through or not.
      - 3. Random gates, controlled/synced passthrough, random gates into signal input, and then check the hold gates, if there is a high signal when the hold gate comes, then output a gate, otherwise not.
      - 4. Random gates, controlled/synced using internal noise. using the internal noise, and one knob as probability, or signal in as CV probability, then output a gate when hold is triggered if the threshold is higher than the probability value.
      - 5. Sample & Hold of a CV signal segment... So using a ring buffer, save as many values as possible, and when hold is triggered playback the ring buffer. One pot for length/size of buffer, and one for speed of sample and playback. S/T switch to choose between looped or back-and-forth? or something similar. 
      - 6. Sample & Hold of an audio signal... Same as above but maybe use faster analogRead for audio signals (8bits only 255), for a chopper like effect.
      - 7. HiHat, would be easy to implement with the internal noise, one knob for noise "speed"/filter and one for decay. One of the parameters (or other) Could also be cv controlled.
      - 8. Suggestion for any function: Connect the nc pin of the signal input to +5v. That way the input knob becomes another value pot even when nothing is connected to the CV-input. A simple mod.
      
    */

    //3 and 4 above. Random gates, they were kindof the same...
    lowCut = analogRead(A1);
    highCut = analogRead(A2);
    intOrExt=digitalRead(intOrExtPin);
     //================= TRIG ================
    if (digitalRead(trigIn) == LOW) {
      //light up the trigLed! We have a trig!
      digitalWrite(trigLed, HIGH);
      //We have a trig! Lets check if there is anything on the signal input.
      //sample=map(0,1023,lowCut,highCut,analogRead(signalInput)); 
      //also add internal noise
      
      if (intOrExt == HIGH) { //just check directly on pin.
        //Reading from external source
        //Below reads the signal and maps it to low and highcut in one go.
        sample=analogRead(signalInput); 
      } else {
        //Using internal noise source
        sample=random(0,1023);
      } 
      
      //if there was a value larger than the HIGHCUT value then output a trig
      if (sample>highCut) {
        //send all ones (5V) to the DAC
        PORTD = lowByte(1023);
        PORTB = highByte(1023);
      } else {
        PORTD = lowByte(0);
        PORTB = highByte(0);
      }
      //Then we wait for the trig to end, so no more trigs will come out of sync with the hold/trig.
      while (digitalRead(trigIn) == LOW) {
        //ESCAPE ALTERNATE FUNCTION:
        //for example both pots=1023 and hold held for 10 seconds..
        unsigned long pressDuration = getButtonPressDuration();
        if (pressDuration > 5000) {
          // Button held for more than 5 seconds
          if (lowCut >= 1020 && highCut>= 1020 ) {
            alternateCode=0;
            digitalWrite (trigLed, LOW);
            delay(1000);
          }
        }
      }
      //Reset the output/release the gate out. If we want trigs we do it inside the while loop.
      digitalWrite(trigLed, LOW);
      PORTD = lowByte(0);
      PORTB = highByte(0); 
    }
    
    
   
  } //Alternate function
} //void loop


unsigned long getButtonPressDuration() {
  static unsigned long startTime = 0;  // Use static variable to retain state
  static bool buttonPressed = false;   // Use static variable to track button state

  if (digitalRead(trigIn) == LOW) {
    if (!buttonPressed) {  // Button press detected (LOW to HIGH transition)
      startTime = millis();
      buttonPressed = true;
    }
  } else {
    buttonPressed = false;  // Button released (HIGH to LOW transition)
  }

  if (buttonPressed) {
    return millis() - startTime;  // Return elapsed time if button is still pressed
  } else {
    return 0;  // Return 0 if button is not pressed
  }
}
