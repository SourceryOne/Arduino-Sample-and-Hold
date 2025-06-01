





  


unsigned long getButtonPressDuration() {
  static unsigned long startTime = 0;  // Use static variable to retain state
  static bool buttonPressed = false;   // Use static variable to track button state

  if (digitalRead(trigIn) == LOW) {
    if (!buttonPressed) {  // Button press detected (LOW to HIGH transition)
      startTime = millis();
      buttonPressed = true;
    }

    if (millis() - startTime > (holdTime *6)) {
      // Button held for more than 11 seconds
      if (lowCut == 0 && highCut== 0 ) {
        digitalWrite(trigLed, HIGH);
        alternateCode=5;
      }
    } else if (millis() - startTime > (holdTime *5)) {
      // Button held for more than 11 seconds
      if (lowCut == 0 && highCut== 0 ) {
        digitalWrite(trigLed, LOW);
        alternateCode=4;
      }
    } else if (millis() - startTime > (holdTime *4)) {
      // Button held for more than 11 seconds
      if (lowCut == 0 && highCut== 0 ) {
        digitalWrite(trigLed, HIGH);
        alternateCode=3;
      }
    } else if (millis() - startTime > (holdTime *3)) {
      // Button held for more than 8 seconds
      if (lowCut == 0 && highCut== 0 ) {
        digitalWrite(trigLed, LOW);
        alternateCode=2;
      }
    } else if (millis() - startTime > (holdTime *2)) {
      // Button held for more than 5 seconds
      if (lowCut == 0 && highCut== 0 ) {
        digitalWrite(trigLed, HIGH);
        alternateCode=1;
      }
    } else if (millis() - startTime > holdTime) {
      // Button held for more than 2 seconds
      if (lowCut == 0 && highCut== 0 ) {
        digitalWrite(trigLed, LOW);
        alternateCode=0;
      }
    }
    
  } else {
    buttonPressed = false;  // Button released (HIGH to LOW transition)
    digitalWrite(trigLed, LOW);
    if (alternateCode!=alternateCodeFromEEPROM) {
      EEPROM.write(eepromAddress, alternateCode);
      alternateCodeFromEEPROM=alternateCode;
    }
  }
}




// Simple function to blink an LED a specified number of times
void blinkLED(int blinkCount) {
  for (int i = 0; i < blinkCount; i++) {
    digitalWrite(trigLed, HIGH); // Turn the LED on
    delay(250);                 // Wait 250ms
    digitalWrite(trigLed, LOW);  // Turn the LED off
    delay(250);                 // Wait 250ms
  }
}
