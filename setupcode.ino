double pi = 3.1415926535897932384;
  
// Added by AC; Uses port C (analog in) pins as power & ground for Nunchuck
static void nunchuck_setpowerpins()
{
#define pwrpin PORTC3
#define gndpin PORTC2
    DDRC |= _BV(pwrpin) | _BV(gndpin);
    PORTC &=~ _BV(gndpin);
    PORTC |=  _BV(pwrpin);
    delay(100);  // wait for things to stabilize        
}
//Added by AC

void setupSinValues()
{
  for (int i =0;i< NUMBER_OF_MICROSTEPS;i++)
  {
    float rad = (pi/((float) NUMBER_OF_MICROSTEPS * 2.0))*(float)i;
    double s = sin(rad);
    double radVal = rad;
    int sval = (255 * s);
    sinValues[i] = sval;
  }
}


void setup()   
{ 
//Added by AC to enable/disable L6226 H-Bridge in order to save power
  pinMode(bridgeEnableA, OUTPUT);
  pinMode(bridgeEnableB, OUTPUT);
//Added by AC  
  
//Added by AC
  nunchuck_setpowerpins();
//Added by AC
    
  setupSinValues();
  setupSpeedTables();

  // we do this to setup the infrastructure for later fastAnalogWrite-calls
  analogWrite(pinA, 0);
  analogWrite(pinB, 0);
  analogWrite(pinC, 0);
  analogWrite(pinD, 0);
 
  setupAndStartTimers();

  Wire.begin(); // start I2C
 
  nunchuck_init ();    

}

// the following three methods are not yet called. They have initially been triggered 
// by a keypad command, but the keypad has been removed in this release.
void setMinPosition()
{
  currentPosition = 0; // this is the new zero point
  minCalibrated = true;
}

void setMaxPosition()
{
  maxPosition = currentPosition;
}

void resetMinMaxPosition()
{
  maxPosition = 1000000;
  currentPosition=500000;
  minCalibrated=false;
}

//Added by AC
//IsTime() function - David Fowler, AKA uCHobby, http://www.uchobby.com 01/21/2012
int IsTime(unsigned long *timeMark, unsigned long timeInterval)
  {
    unsigned long timeCurrent;
    unsigned long timeElapsed;
    int result=false;
  
    timeCurrent=millis();
    timeElapsed=timeCurrent-*timeMark;  

  if(timeElapsed>=timeInterval)
     {
       *timeMark=timeCurrent;
      result=true;
     }
  return(result);  
 }
 //Added by AC
