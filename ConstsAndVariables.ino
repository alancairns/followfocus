// general constants
#define MODE_NOTHING 0
#define MODE_MANUAL_POSITION 1
#define MODE_MANUAL_POSITION_WITH_STOP 2
#define DIRECTION_INCREMENT 1
#define DIRECTION_DECREMENT -1
#define DIRECTION_STOP 0

// communication between main program and interrupt service routine
volatile int mode = MODE_NOTHING;
volatile long dir = DIRECTION_STOP;
volatile int lastDirection = DIRECTION_STOP;
volatile int speed = 0;
volatile int timerEventsSinceLastPosition = 0;

boolean nunchuck_present = false; // does a nunchuck exist at the system

volatile long manualSteps = 0;
volatile int preRunAdjustments = 5;
volatile long destinationPosition = 0;
volatile int destinationDirection = DIRECTION_INCREMENT; // to compensate for belt slack
volatile long oldDestinationPosition = 0;

//added by AC; useed to compensate for belt slack
volatile boolean slackAdjustmentApplied = false;
volatile int positionDifference = 0;

volatile long slackPoint1 = -1;
volatile long slackPoint2 = -1;
volatile long slackAdjustment = 1024; // 1024 works with my setup

volatile int currentMemoryPosition = 1;

// current position and min/max before stepper stops
volatile long currentPosition = 500000;
volatile long minPosition = 0;
volatile boolean minCalibrated = false;
volatile long maxPosition = 500000; //1000000;
 
void setupSpeedTables()
{
  // the actual speed is determined by two values: overflow and multiplier
  // (255 - overflow) is the number of ISR cycles have to pass before an action 
  // is triggered.
  // multiplier is the number of microsteps which should be advanced as soon as
  // the action is triggered.
  
  // the reason for this is that we've been trying to reduce the noise for each
  // speed level this has been done empirically using the desired stepper motor.
  // Your values might differ.

  // If you find out the your setup is too loud at a paricular speed level, you
  // might want to try with a different value (or just eliminate the particular
  // speed level by setting its values to the same values of a less noisy
  // neighbour speed)
  
  // it could also be the case that your motor stalls at high speeds. In this 
  // case, you can either decrease all the overflow-values here *OR* you could
  // go to the InterruptVectors file, look at the interrupt method 
  // ISR(TIMER2_OVF_vect) and change the overflow line from
  // "overflow = overflows[speed]" to "overflow = overflows[speed] - X" 
  // whereas X is the adjustment factor necessary for your motor.

  overflows  [0] = 250;
  multipliers[0] = 0;

  overflows  [1] = 100;
  multipliers[1] = 1;
  
  overflows  [2] = 190;
  multipliers[2] = 1;

  overflows  [3] = 222;
  multipliers[3] = 1;

  overflows  [4] = 233;
  multipliers[4] = 1;

  overflows  [5] = 239;
  multipliers[5] = 1;

  overflows  [6] = 245;
  multipliers[6] = 1;

  overflows  [7] = 248;
  multipliers[7] = 1;

  overflows  [8] = 250;
  multipliers[8] = 1;

  overflows  [9] = 251;
  multipliers[9] = 1;

  overflows  [10] = 252;
  multipliers[10] = 1;

  overflows  [11] = 245;
  multipliers[11] = 3;

  overflows  [12] = 246;
  multipliers[12] = 3;

  overflows  [13] = 253;
  multipliers[13] = 1;

  overflows  [14] = 248;
  multipliers[14] = 3;

  overflows  [15] = 249;
  multipliers[15] = 3;

  overflows  [16] = 254;
  multipliers[16] = 1;

  overflows  [17] = 251;
  multipliers[17] = 3;

  overflows  [18] = 250;
  multipliers[18] = 4;

  // if highSpeed has been enabled, we'll set speed 19 (max speed as of later) to 
  // a regular "digitial" output without any additional microsteps. (In turn, 
  // we have to massively increase the wait time to avoid any stalling of the motor)
  if (enableHighSpeed)
  {
    overflows  [19] = 160;
    multipliers[19] = NUMBER_OF_MICROSTEPS;
  }
  else
  {
    overflows  [19] = 250;
    multipliers[19] = 4;  
  }

}
