void alternateFunction2() {
  /* SAMPLE AND SLIDE
   * HI knob = Slide value as part of the whole length between the last two pulses.
   * IDEA: LOW knob = Probability of 2nd gate on the downbeat up to half the knob which is 100% then probability of 4th gate on the fourths.
   */
  // Active TRIG IN, read the time and values for slide
  unsigned long currentTime = millis(); // Ensure currentTime is declared and updated at the start

  if (digitalRead(trigIn) == LOW) {
    // We have a trig! Lock the sample value and send it.
    digitalWrite(trigLed, HIGH); // Light up the trig LED

    if (lastTrigState == 0) { // Check the previous state
      lastValue = currentValue;
      //currentValue = sample;                // Lock the new sample value
      targetValue = sample;
      pulseInterval = currentTime - lastPulseTime; // Update pulse interval
      lastPulseTime = currentTime;          // Update last pulse time
    }
    lastTrigState = 1; // Set the current trigger state
  } else { // TrigIn is HIGH (not active)
    digitalWrite(trigLed, LOW);
    lastTrigState = 0; // Reset the trigger state
  }

  //slideFactor = float(highCut) / 1023.0; // Map potentiometer value to 0.0 - 1.0
  //Use below instead to get one knob for upwards slide and one for downward slide... :)
  if (lastValue < targetValue) {
    slideFactor = float(lowCut) / 1023.0; // Map potentiometer value to 0.0 - 1.0
  } else {
    slideFactor = float(highCut) / 1023.0; // Map potentiometer value to 0.0 - 1.0
  }
  
  slideToTarget(currentTime); // Smoothly transition to the next value

  // Output the current value as a PWM signal on the analog output pin
  //analogWrite(outputPin, currentValue);
  PORTD = lowByte(currentValue);
  PORTB = highByte(currentValue);
  
  // Check to see if we go into an alternate function
  getButtonPressDuration();
}

// Smoothly slide from lastValue to targetValue
void slideToTarget(unsigned long currentTime) {
  if (pulseInterval == 0) return; // Avoid division by zero

  // Calculate the progress as a fraction of pulseInterval, adjusted by slideFactor
  unsigned long elapsedTime = currentTime - lastPulseTime;
  float progress = float(elapsedTime) / (pulseInterval * slideFactor);

  if (progress >= 1.0 || slideFactor == 0.0) {
    currentValue = targetValue; // Instantly jump to target if slideFactor is 0
  } else {
    currentValue = lastValue + (targetValue - lastValue) * progress;
  }
}
