void loop() {
  //Serial.println(alternateCode);
  //Check the pots for value range
  if (timeToReadPots>=cyclesBetweenReadPots) {
    lowCut = analogRead(A1);
    highCut = analogRead(A2);
    intOrExt=digitalRead(intOrExtPin);
    timeToReadPots=0;
  } else {
    timeToReadPots++;
  }
  
  //CHECK TO SEE IF WE GO INTO ALTERNATE FUNCTION.
  getButtonPressDuration();

  //Check if we should use internal noise or signal input.
  if (intOrExt == HIGH) { //just check directly on pin.
    //Reading from external source
    //Below reads the signal and maps it to low and highcut in one go.
    if (alternateCode==0) {
      //Alternate code doesnt restrict the input with the pots (probably)...
      //SUPER WRONG CODE, BUT SOUNDS VERY COOL! THE TWIDDLING SOUND: sample=map(0,1023,lowCut,highCut,analogRead(signalInput)); 
      sample=map(analogRead(signalInput),0,1023,lowCut,highCut); 
    } else {
      sample=analogRead(signalInput); //for the functions where the input signal should not be tied to min and max.
    }
  } else {
    //Using internal noise source
    if (alternateCode==0) {
      //Same as above, alternate code doesnt restrict the input with the pots (probably)...
      sample=random(lowCut,highCut);
    } else {
      sample=random(0,1023); //for the functions where the input signal should not be tied to min and max. (same as above on analogRead).
    }
  }
  
//========================================================================================
//MAIN FUNCTION
  switch(alternateCode) {
    case 0:
      mainFunction();
      break;
//========================================================================================
//ALTERNATE FUNCTION 1 - happens if both lowCut and highCut is set to 0 and hold is pushed for 5 seconds.
    case 1: 
      alternateFunction1();
      break;
    case 2: 
      alternateFunction2();
      break;
    case 3: 
      alternateFunction3();
      break;
    case 4: 
      alternateFunction4();
      break;
    case 5: 
      alternateFunction5();
      break;
  }
  
} //void loop
