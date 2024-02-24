// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// set prescale to 16
sbi(ADCSRA,ADPS2) ;
cbi(ADCSRA,ADPS1) ;
cbi(ADCSRA,ADPS0) ;
//above not tested, for faster analogRead...

// Define the pin numbers
const int trigIn = A3;  // Pin connected to the button
const int clipLed = 13;     // Pin connected to the LED
const int trigLed = A5;     // Pin connected to the LED

//variable to store the current sample
int sample;
int highCut=1023;
int lowCut=0;

void setup() {
  // Set the appropriate pins as output for port manipulation
  DDRD = 0b11111111;
  DDRB = 0b11111111;

  pinMode(trigIn, INPUT_PULLUP);
  
  //start random/noise
  randomSeed(random(1023));
}  
  
void loop() {
  if (digitalRead(trigIn) == LOW) {
    //light up the trigLed! We have a trig!
    digitalWrite(trigLed, HIGH);

    //Check the pots for value range
    lowCut = analogRead(A1);
    highCut = analogRead(A2);

    //get a random sample
    sample=random(lowCut,highCut);

    //send the sample to the DAC
    PORTD = lowByte(sample);
    PORTB = highByte(sample);

    while (digitalRead(trigIn) == LOW) {
      // Do nothing, except check if the button is still pressed
    }  
  
    if (sample >= 1023) {
      digitalWrite(13,HIGH);
    } else {
      digitalWrite(13,LOW);
    }
  } else {
    digitalWrite(trigLed, LOW); //no more trig...
  }//end trigIn == LOW
  
}
