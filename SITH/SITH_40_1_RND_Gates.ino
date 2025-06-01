void alternateFunction1() {
    /* RANDOM GATES
     * HI knob = Probability
     * IDEA: LOW knob = Probability of 2nd gate on the downbeat up to half the knob which is 100% then probability of 4th gate on the fourths.
     */
    //3 and 4 above. Random gates, they were kindof the same...
    //================= TRIG ================
    if (digitalRead(trigIn) == LOW) {
      //light up the trigLed! We have a trig!
      digitalWrite(trigLed, HIGH);
 
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
        getButtonPressDuration();
      }
      //Reset the output/release the gate out. If we want trigs we do it inside the while loop.
      digitalWrite(trigLed, LOW);
      PORTD = lowByte(0);
      PORTB = highByte(0); 
    }
} //Alternate function
//========================================================================================
