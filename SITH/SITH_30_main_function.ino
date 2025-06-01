void mainFunction() {
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
      getButtonPressDuration();
    }
       
  } else { //if trigIn
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
