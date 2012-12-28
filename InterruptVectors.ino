volatile byte overflow;

// this will be called as an interrupt service routine for the native ATMega timer
// In this method, we'll drive our PWM cycles

ISR(TIMER2_OVF_vect) 
{
  // this is for a manual overflow handling. If this byte reaches 255, the next
  // increment will move it to zero and the processing of this ISR will continue.
  // The next value of overflow will be set at the end of this method depending
  // on the current speed.
  overflow ++; 
  if (overflow != 0) return;

  if (manualSteps!=0)
  {
    currentPosition+=manualSteps;
    manualSteps = 0;
    movePosition();
  }
  else
  {
    if (preRunAdjustments!=0) return; // if we are still in pre-run mode

    boolean moveOnlyToMarker = (z_button);

    if (moveOnlyToMarker && destinationPosition!=0) 
    {
      // if the user holds button Z and has selected a target position, we'll only
      // move towards the target position without taking the "real" direct
      // direction into account.
      
      //AC couldn't figure out how to use this bit of code so coded belt slack adjustment
      //in mainLoop
      long adjustedDestination = destinationPosition;
      if (destinationDirection != dir) 
      {
        adjustedDestination += (dir * slackAdjustment);
      }

      if ((currentPosition >= destinationPosition && dir != DIRECTION_DECREMENT) ||
        (currentPosition <= destinationPosition && dir != DIRECTION_INCREMENT))
      {
        return;
      }
    }

    overflow = overflows[speed]; //original statement: overflow = overflows[speed]; 
    long multiplier = multipliers[speed];

    long delta = dir*multiplier;

    long newPos = currentPosition+delta;
    
    // just to make sure that we aren't moving outside of our min/max values
    if (newPos < minPosition || newPos > maxPosition) 
    {
      return;
    }

    currentPosition=newPos; // that's were we want to be right
    movePosition();
  }
}

void movePosition()
{
  // we're checking the value of currentPosition and determine in which of the four
  // phases of our stepper *and* at what position inside of this particular phase
  // we are right now. We will then output these analog values via our PWMming
  
  int posIndex = ((int)currentPosition)  & (PHASE_MODULO -1); //modulus (PHASE_MODULO)
  int phaseNum = posIndex >> NUMBER_OF_MICROSTEPS_BIT; //divide by NUMBER_OF_MICROSTEPS
  int substep = posIndex & (NUMBER_OF_MICROSTEPS -1); // modulus (NUMBER_OF_MICROSTEPS)  
  
  if (enableHighSpeed && speed == 19)
  {
    switch (phaseNum)
    {
      case 0:
        fastAnalogWriteB(0);   
        fastAnalogWriteD(0);   
        fastAnalogWriteA(255);
        fastAnalogWriteC(255);
        break;
      case 1:
        fastAnalogWriteB(0);
        fastAnalogWriteC(0);
        fastAnalogWriteA(255);
        fastAnalogWriteD(255);
        break;
      case 2:
        fastAnalogWriteA(0);
        fastAnalogWriteC(0);
        fastAnalogWriteB(255);
        fastAnalogWriteD(255);
        break;
      case 3:
        fastAnalogWriteA(0);
        fastAnalogWriteD(0);
        fastAnalogWriteB(255);
        fastAnalogWriteC(255);
        break;
    } 
  }
  else
  {
    switch (phaseNum)
    {
      
      case 0:
        fastAnalogWriteB(0);   
        fastAnalogWriteD(0);   
        fastAnalogWriteA(sinValues[substep]);
        fastAnalogWriteC(sinValues[NUMBER_OF_MICROSTEPS-substep-1]);
        break;
      case 1:
        fastAnalogWriteB(0);
        fastAnalogWriteC(0);
        fastAnalogWriteA(sinValues[NUMBER_OF_MICROSTEPS-substep-1]);
        fastAnalogWriteD(sinValues[substep]);
        break;
      case 2:
        fastAnalogWriteA(0);
        fastAnalogWriteC(0);
        fastAnalogWriteB(sinValues[substep]);
        fastAnalogWriteD(sinValues[NUMBER_OF_MICROSTEPS-substep-1]);
        break;
      case 3:
        fastAnalogWriteA(0);
        fastAnalogWriteD(0);
        fastAnalogWriteB(sinValues[NUMBER_OF_MICROSTEPS-substep-1]);
        fastAnalogWriteC(sinValues[substep]);
        break;
    } 
  }

}


void setupAndStartTimers()
{
  // Added by AC
      TIMSK2 |= (1<<TOIE2);                             // set TOIE2 bit to enable the overflow interrupt on Timer 2 

      TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1); // Clear OC1A on Compare Match when up- counting. Set OC1A on Compare Match when downcounting.
      TCCR1B = _BV(CS10);                              // Clear OC1B on Compare Match when up- counting. Set OC1B on Compare Match when downcounting.
      
      TCCR2A = _BV(WGM20) | _BV(COM2A1) | _BV(COM2B1); // Clear OC2A on compare match up-counting, set OC2A on compare match down-counting
      TCCR2B = _BV(CS20);         // Clear OC1B on Compare Match when up- counting. Set OC1B on Compare Match when downcounting.
  // Added by AC     
}

