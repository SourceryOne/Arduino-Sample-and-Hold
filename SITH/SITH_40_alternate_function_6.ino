void alternateFunction6() {
      /*Suggestions for alternate functions:
      - 1. Random gates, either totally random, with one of the knobs setting the probability of a gate.
      - DONE 2. Random gates, bernoulli type, input on the hold for example, and one pot deciding if the gate should go through or not.
      - DONE 3. Random gates, controlled/synced passthrough, random gates into signal input, and then check the hold gates, if there is a high signal when the hold gate comes, then output a gate, otherwise not.
      - 4. Random gates, controlled/synced using internal noise. using the internal noise, and one knob as probability, or signal in as CV probability, then output a gate when hold is triggered if the threshold is higher than the probability value.
      - 5. Sample & Hold of a CV signal segment... So using a ring buffer, save as many values as possible, and when hold is triggered playback the ring buffer. One pot for length/size of buffer, and one for speed of sample and playback. S/T switch to choose between looped or back-and-forth? or something similar. 
      - 6. Sample & Hold of an audio signal... Same as above but maybe use faster analogRead for audio signals (8bits only 255), for a chopper like effect.
      - DONE 7. HiHat, would be easy to implement with the internal noise, one knob for noise "speed"/filter and one for decay. One of the parameters (or other) Could also be cv controlled.
      - 8. Suggestion for any function: Connect the nc pin of the signal input to +5v. That way the input knob becomes another value pot even when nothing is connected to the CV-input. A simple mod.
      - DONE! 9. Sample and slide. Slides between the values with the time of the previous pulse. Or with max and min? Did it with the highCut tellnig how long time for the slide to finish.
      - DONE 10. LFO & Hold 
      - DONE 11. AD Envelope generator - max=A, min=D, hold=trig.
      -12. Delayed trig. Using the sample input as a CV for length of delay, or one of the knobs.
      -13. CVRecorder, low=length of pattern, track=record, sample=play. or high=lowest=play, highest=record.
      -14. TAP LFO, that sets the slopes to the time between taps
      -15 TAP TEMPO LFO, just gets the tap tempo and starts the LFO/AD each Cycle not setting slopes.
    
    
    if max and min is maxvalue then record otherwise play. or record when sample, play when track?
    could also have more melodies if max is max record, otherwise play the one min is at.
    if gate input then record cv from sample, and save in ring buffer.
    LOWCUT=1023=Record next note in chain
    LOWCUT=0=Remove first note in chain
    LOWCUT any other value=play the notes.
    */

bool trigState = digitalRead(trigIn);
  // Keep LED on while trigIn is LOW
  if (trigState == LOW) {
    digitalWrite(trigLed, HIGH);
  } else {
    digitalWrite(trigLed, LOW);
  }
  
  // Detect falling edge on trigIn
  if (wasTrigHigh && trigState == LOW) {
    //light up the trigLed! We have a trig!
    digitalWrite(trigLed, HIGH);
    if (lowCut == 1023) {
      // === RECORD MODE ===
      if (bufferLength < BUFFER_SIZE) {
        //ringBuffer[bufferLength] = analogRead(signalInput);
        ringBuffer[bufferLength] = sample;
        //Output the value while recording.
        PORTD = lowByte(sample);
        PORTB = highByte(sample);
        bufferLength++;
      }
    } else if (lowCut == 0) {
      // === REMOVE-FIRST MODE ===
      if (bufferLength > 0) {
        // Shift all elements down by one
        for (int i = 0; i < bufferLength - 1; i++) {
          ringBuffer[i] = ringBuffer[i + 1];
        }
        bufferLength--; // Reduce stored length
        if (bufferIndex >= bufferLength) {
          bufferIndex = 0; // Reset playback index if needed
        }
      }

    } else {
      // === PLAYBACK MODE ===
      int outputValue = ringBuffer[bufferIndex];      
      PORTD = lowByte(outputValue);
      PORTB = highByte(outputValue);
      // Use outputValue however you want here (e.g. analogWrite/DAC/output pin)
      bufferIndex++;
      if (bufferIndex >= bufferLength) {
        bufferIndex = 0; // Loop when reaching end
      }
    }
  } else {
    digitalWrite(trigLed, LOW);
  }

  // Update previous trig state
  wasTrigHigh = trigState;

    
}

void alternateFunction7() {
  /* Same as above but for trigs instead. so the trigIn is the "Tempo" and each trigIn if 
   *  the sample input is high, then add a 1 or high to the "drumarray".
   */
   
// ===== VARIABLES =====
// bool ringBuffer[BUFFER_SIZE];
// int bufferLength = 0;          
// int startIndex = 0;            
// int bufferIndex = 0;           
// bool wasTrigHigh = false;      
// bool latchedHigh = false;      // Tracks high during current trig period
// const int trigIn, signalInput, trigLed, lowCut;

//void handleGateRecorder() {
  bool trigState = digitalRead(trigIn);

  // Keep LED on while trigIn is LOW
  digitalWrite(trigLed, trigState == LOW ? HIGH : LOW);

  // Latch if signalInput goes high during this period
  if (digitalRead(signalInput) == HIGH) {
    latchedHigh = true;
  }

  // Detect falling edge on trigIn
  if (wasTrigHigh && trigState == LOW) {

    bool slotValue = latchedHigh;  // Capture latched state for this slot
    latchedHigh = false;           // Reset for next period

    if (lowCut == 1023) {
      // === RECORD MODE ===
      if (bufferLength < BUFFER_SIZE) {
        int insertIndex = (startIndex + bufferLength) % BUFFER_SIZE;
        ringBuffer[insertIndex] = slotValue;
        bufferLength++;
      }
      // Optional: overwrite oldest if buffer full

    } else if (lowCut == 0) {
      // === REMOVE-FIRST MODE ===
      if (bufferLength > 0) {
        startIndex = (startIndex + 1) % BUFFER_SIZE;
        bufferLength--;
        if (bufferIndex >= bufferLength) {
          bufferIndex = 0;
        }
      }

    } else {
      // === PLAYBACK MODE ===
      if (bufferLength > 0) {
        int playIndex = (startIndex + bufferIndex) % BUFFER_SIZE;
        bool outputValue = ringBuffer[playIndex];
        
        // TODO: send outputValue to output pin
        // e.g. digitalWrite(outPin, outputValue);

        bufferIndex++;
        if (bufferIndex >= bufferLength) {
          bufferIndex = 0; // Loop playback
        }
      }
    }
  }

  // Save last trigger state
  wasTrigHigh = trigState;
}
