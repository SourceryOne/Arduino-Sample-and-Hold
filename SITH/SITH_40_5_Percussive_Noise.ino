void alternateFunction5() {
      /*
       * Hi-Hat well, more of a Percussive Noise...
       * 
       * Lowcut=decay
       * Highcut=filter/speed
      
      - 7. HiHat, would be easy to implement with the internal noise, one knob for noise "speed"/filter and one for decay. One of the parameters (or other) Could also be cv controlled.
      
    */
  if (digitalRead(trigIn) == LOW && newTrig==1){
    newTrig=0;
    hihatfilter=map(highCut,0,1023,0,20);
    //We have a trig! Lets start the Envelope
    //light up the trigLed! We have a trig!
    digitalWrite(trigLed, HIGH);
    //if (envelopePhase==0) {
      //Only set once.
      envelopePhase=1; //Set the phase to Attack 
      rampDuration = lowCut; //using the value of the lowCut knob
      startTime = millis();  // Record the start time
      currentValue = 0;
    //}   
  } else { 
    digitalWrite(trigLed, LOW); 
    newTrig=1;
    if (envelopePhase==10) { envelopePhase=0; }
    //if (envelopePhase==20) {
    //envelopePhase=0;
    //} //If you change below so it wont loop. But then it will not retrig until the envelope is done either...
    //PORTD = lowByte(0);
    //PORTB = highByte(0);
  } //There is no trig... Turn the led off.
  
  
  
  unsigned long elapsedTime = millis() - startTime;
  if (elapsedTime < rampDuration) {
    if (envelopePhase==1) { 
      float progress = (float)elapsedTime / rampDuration; 
      currentValue = (sample) * pow(1.0 - progress, 1); } //change to 0.5 for exponential and 3.5 for logarithmic curves. 1 is linear
  } else { //ENVELOPE TIME IS UP!
    if (envelopePhase==1) { 
      envelopePhase=10; //So it wont loop...
      //envelopePhase=0;
      currentValue=0;  
    }
  }
  // Output the current value as a PWM signal on the analog output pin
  //analogWrite(outputPin, currentValue);
  if (filterCounter<=hihatfilter) {
      filterCounter++;
    } else {
      filterCounter=0;
      PORTD = lowByte(currentValue);
      PORTB = highByte(currentValue);
    }
   /* if (filterCounter<=hihatfilter) {
      filterCounter++;
    } else {
      filterCounter=0;
      PORTD = lowByte(hihat);
      PORTB = highByte(hihat);
    }*/
}
