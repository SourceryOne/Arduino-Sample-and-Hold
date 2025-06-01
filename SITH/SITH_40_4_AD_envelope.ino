void alternateFunction4() {
  /*   AD Envelope generator
   *   Taken and adapted from the Arduino ADSR project.
   *   Lowcut=Attack
   *   HighCut=Decay
   *   HOLD input is the trig/gate         oh riiiiight, if gate, it is an ASR aswell
   *   
   */   

  //Serial.println(envelopePhase);
  //if (digitalRead(trigIn) == LOW) {
  if (digitalRead(trigIn) == LOW && newTrig==1){
    newTrig=0;
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
    if (envelopePhase==20) { envelopePhase=0; } //If you change below so it wont loop. But then it will not retrig until the envelope is done either...
  } //There is no trig... Turn the led off.
  
  if (envelopePhase==12) {
    envelopePhase=2; 
    currentValue = maxValue; //remove this for it to go from the present value
    rampDuration = highCut; //using the value of the highCut knob
    startTime = millis();
  }

  //CALCULATING currentValue1
  unsigned long elapsedTime = millis() - startTime;
  if (elapsedTime < rampDuration) {
    if (envelopePhase==1) { 
      float progress = (float)elapsedTime / rampDuration; 
      currentValue = maxValue * pow(progress, 1); } //change to 0.5 for exponential and 3.5 for logarithmic curves. 1 is linear
    if (envelopePhase==2) { 
      float progress = (float)elapsedTime / rampDuration; 
      currentValue = (maxValue) * pow(1.0 - progress, 1); 
    }
  } else { //ENVELOPE TIME IS UP!
    if (envelopePhase==1) {
      currentValue = maxValue;  //remove this for it to go from the present value
      envelopePhase=12; //move to next phase.
    } else if (envelopePhase==2) { 
      //currentValue1 = sustainValueCh1*sustainMultiplier; 
      envelopePhase=20; //So it wont loop...
      //envelopePhase=0;
      currentValue=0; 
    }
  }

  // Output the current value as a PWM signal on the analog output pin
  //analogWrite(outputPin, currentValue);
  PORTD = lowByte(currentValue);
  PORTB = highByte(currentValue);

}
