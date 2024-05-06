
// Define the pin numbers
const int trigIn = A3;  // Pin connected to the button
//const int clipLed = 13;     // Pin connected to the Clip LED, removed this code as the port manipulation messed with the led.
const int trigLed = A5;     // Pin connected to the LED
const int signalInput = A6; // Input signal to sample or track and hold.
const int sampleOrTrackPin = A4; //switch between 5v and GND to decide sample or track.
const int intOrExtPin = A0;
//const int bypassSignalPin = for now just use the trigLed? Skipped this aswell as I found to many fun things to do with the signal in track mode.

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
  
  

  //MAIN FUNCTION
  if (alternateCode == 0) {
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
        //CHECK TO SEE IF WE GO INTO ALTERNATE FUNCTION.
        unsigned long pressDuration = getButtonPressDuration();
        if (pressDuration > 5000) {
          // Button held for more than 5 seconds
          if (lowCut == 0 && highCut== 0 ) {
            alternateCode=1;
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
    digitalWrite (trigLed, HIGH); //Just to show we are in alternate mode. 
    /*Suggestions for alternate functions:
      - Random gates, either totally random, with one of the knobs setting the probability of a gate.
      - Random gates, bernoulli type, input on the hold for example, and one pot deciding if the gate should go through or not.
      - Random gates, controlled/synced passthrough, random gates into signal input, and then check the hold gates, if there is a high signal when the hold gate comes, then output a gate, otherwise not.
      - Random gates, controlled/synced using internal noise. using the internal noise, and one knob as probability, or signal in as CV probability, then output a gate when hold is triggered if the threshold is higher than the probability value.
      - Sample & Hold of a CV signal segment... So using a ring buffer, save as many values as possible, and when hold is triggered playback the ring buffer. One pot for length/size of buffer, and one for speed of sample and playback. S/T switch to choose between looped or back-and-forth? or something similar. 
      - Sample & Hold of an audio signal... Same as above but maybe use faster analogRead for audio signals (8bits only 255), for a chopper like effect.
    */
    //ESCAPE ALTERNATE FUNCTION:
    //for example both pots=1023 and hold held for 10 seconds.

  }  
}


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


