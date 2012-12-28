long storedPositions[5];
int storedDirections[5];

void loop()                     
{
 //Added by AC to disable H-Bridge if motor stopped in order to save power.  The IsTime function
 //provides a 5sec delay before disabling to stop occasional motor vibration.
  if (oldcurrentPosition == currentPosition)
  {
      if(IsTime(&motorStoppedTimeMark, motorStoppedTimeInterval)) 
     {  
        digitalWrite(bridgeEnableA, LOW);
        digitalWrite(bridgeEnableB, LOW);
        slackAdjustmentApplied = false;
      }
  }
  oldcurrentPosition = currentPosition;
 //Added by AC

  int joy_posindex = -1;
  int adj_x = -1;
  int adj_y = -1;
  
  preRunAdjustments--;  // sometimes, when firing up, I get flaky input. I'm
                        // filtering out the first five cycles.
  if (preRunAdjustments == 1) 
  {
    speed = 0;
    dir = 0;
  }
  if (preRunAdjustments<0) preRunAdjustments = 0;

  processNunchuckData();
  
  int absSpeed = 0;
  int newdir = 0;
  int moveSpeed = 0;
     
  if (nunchuck_present) // if a nunchuck is currently connected
  {
    adj_x = joy_x_axis - 127;
    adj_y = joy_y_axis - 131; // original statement adj_y = joy_y_axis - 127 but my joy_y_axis
                              // at rest returns value between 131 - 132
    
    int abs_adj_x = abs(adj_x);
    int abs_adj_y = abs(adj_y);
    
    int joy_move_axis = joy_y_axis;
    
 //Added by AC to set mimimum positon when button c and z are pressed together
    if (c_button && z_button)
    {
      minPosition = currentPosition;
    }
     
  
    // let's find out if the user has pressed one of the buttons while moving 
    // the joystick at least 10 points to either direction. This would constitute
    // one of the nunchuck commands (press + direction)
    if (abs_adj_x>10 || abs_adj_y>10)
    {    
      if (abs_adj_x > abs_adj_y)   // user moved the joystick on the horizontal axis
      {

        if (z_button) // and the z_button has been clicked
        {
          joy_move_axis = joy_x_axis;
        }
        
        if (adj_x>0)
        {
          joy_posindex = 2; // storage position 2 will be stored/retrieved
        }
        else
        {
          joy_posindex = 1; // storage position 1 will be stored/retrieved
        }
      }
      else
      {
        if (adj_y>0)
        {
          joy_posindex = 3; // storage position 3 will be stored/retrieved
        }
        else
        {
          joy_posindex = 4; // storage position 4 will be stored/retrieved
        }
      }

      if (z_button && joy_posindex != -1)
      {
        // retrieve button has been held while moving the joystick. we are
        // retrieving the values for this position.
        currentMemoryPosition = joy_posindex;
        lastDirection = storedDirections[joy_posindex];
        
        //Added by AC to compensate for belt slack        
        if (slackAdjustmentApplied == false)
        {
          positionDifference = destinationPosition - currentPosition;
          
          if ((positionDifference >= 10) && (lastDirection != DIRECTION_INCREMENT))
          {
           destinationPosition = storedPositions[joy_posindex] + slackAdjustment;
           slackAdjustmentApplied = true;
          } 
          else if ((positionDifference <= -10) && (lastDirection != DIRECTION_DECREMENT))
          {
           destinationPosition = storedPositions[joy_posindex] + slackAdjustment;
           slackAdjustmentApplied = true;
          }
          else
          {
            destinationPosition = storedPositions[joy_posindex];
          }
        }
        //Added by AC
      }
    }
    
    
    // regular speed calculation based on the value of the input device. 
    // We'll turn the 0-255 read from the nunchuck into a speed value of 0 to 20
    // *and* a direction information (STOP/INCREMENT/DECREMENT)
    if (joy_move_axis < 123) //originally <120
    {
      moveSpeed = joy_move_axis - 123; //originally <120
    }
    if (joy_move_axis>133) //originally <130
    {
      moveSpeed = joy_move_axis - 133; //originally <130
    }

    absSpeed = abs(moveSpeed);
    newdir = DIRECTION_STOP;
      
  
    if (absSpeed>80) absSpeed = 80;
    absSpeed = absSpeed / 4; 
    
     // (Speed is now 1-20)
     
    if (moveSpeed < 0)
    {
      newdir = DIRECTION_DECREMENT;
    }
    else if (moveSpeed > 0)
    { 
      newdir = DIRECTION_INCREMENT;
    }

    if (c_button)
    {
      // if the c button ("store position") has been held, while the joystick has
      // been pointed to one of the directions, we'll store the current
      // focus position
      if (joy_posindex != -1)
      { 
        currentMemoryPosition = joy_posindex;
        destinationPosition = currentPosition;
        storedPositions[currentMemoryPosition] = currentPosition;
        storedDirections[currentMemoryPosition] = lastDirection;
      }
    }
  } 
  
  // this was just for testing. I didn't dare to remove it for highspeed mode
  if (absSpeed == 20) absSpeed = 19;

  //Added by AC enable H-Bridge when motor is required to move
  if (absSpeed != 0 && oldcurrentPosition != currentPosition)
  {
      digitalWrite(bridgeEnableA, HIGH); //added by me
      digitalWrite(bridgeEnableB, HIGH); //added by me
      delay(10);
  //Added by AC
  }

  if (z_button)
  {
    // drive to destination mode
    if (destinationPosition > currentPosition)
    {
      newdir = DIRECTION_INCREMENT;
    }
    else if (destinationPosition < currentPosition)
    {
      newdir = DIRECTION_DECREMENT;
    }
    else
    {
      newdir = DIRECTION_STOP;
    }
    speed = absSpeed;
    dir = newdir;
  }
  else if (!c_button)
  {
    // regular drive mode, if the user didn't hold the C button
    speed = absSpeed;
    dir = newdir;
    
    if (dir!=DIRECTION_STOP)
    {
      lastDirection = dir;
    }
  }
  else
  {
    speed = 0;
    dir = DIRECTION_STOP;
  }

  oldDestinationPosition = destinationPosition;
}



