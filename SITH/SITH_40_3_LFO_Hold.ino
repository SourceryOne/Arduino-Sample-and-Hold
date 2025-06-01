void alternateFunction3() {
   /*
    * LFO & Hold
    * lowCut = Rising Edge
    * highCut = Falling Edge
    * The led blinks in time with the lfo, but also when hold is active.
    * 
    * more ideas
    * sample input as CV signal to adjust the speed.
    * OR Just use one pot as speed (for both), and the other for shape (lin/log/exp).
    */
    
    //Serial.println(ledStartTime);
  if (envelopePhase==0) { 
    digitalWrite(trigLed, HIGH);
    ledStartTime=millis();
    envelopePhase=1; //Set the phase to Attack 
    rampDuration = lowCut; //using the value of the lowCut knob
    startTime = millis();  // Record the start time
    currentValue = 0;   
  } else if (envelopePhase==12) {
    envelopePhase=2; 
    currentValue = maxValue;
    rampDuration = highCut; //using the value of the lowCut knob
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
      currentValue = maxValue;  // Ensure it stays at maxValue after 1000ms OR SHOULD IT BE THE TOP OF SUSTAIN IF DECAY IS OFF!
      envelopePhase=12; //move to next phase.
    } else if (envelopePhase==2) { 
      //currentValue1 = sustainValueCh1*sustainMultiplier; 
      envelopePhase=0;
    }
  }

  if (ledStartTime>0) {
    if (millis()-ledStartTime>50) { 
      digitalWrite(trigLed, LOW);
      ledStartTime=0;
    }
  }
  
  // Output the current value as a PWM signal on the analog output pin
  //analogWrite(outputPin, currentValue);
  PORTD = lowByte(currentValue);
  PORTB = highByte(currentValue);
  
  if (digitalRead(trigIn) == LOW) {
    //We have a trig! Lets lock the sample value and send it.
    //light up the trigLed! We have a trig!
    digitalWrite(trigLed, HIGH);

    while (digitalRead(trigIn) == LOW) {
      //CHECK TO SEE IF WE GO INTO ALTERNATE FUNCTION.
      getButtonPressDuration();
    }

  } else { //if trigIn
  //================= NO TRIG ================
    //There is no trig...
    if (ledStartTime==0) { digitalWrite(trigLed, LOW); } //but only turn the led of if there is no high from the lfo. no more trig...
  }

  
}
