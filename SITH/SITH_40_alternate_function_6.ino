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
      -11. TAP TEMPO LFO
      - DONE 12. AD Envelope generator - max=A, min=D, hold=trig.
      -13. Delayed trig. Using the sample input as a CV for length of delay, or one of the knobs.
      -14. CVRecorder, low=length of pattern, track=record, sample=play. or high=lowest=play, highest=record.
    */
}
