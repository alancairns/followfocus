/*
 * This sketch was rewritten by AC using code from Tod E. Kurt. The WiichuckDemo code can be 
 * found at https://github.com/downloads/todbot/wiichuck_adapter/WiichuckDemo.zip
 *
 * This library is from the Bionic Arduino course : 
 *                          http://todbot.com/blog/bionicarduino/
 *
 * 2007-11 Tod E. Kurt, http://todbot.com/blog/
 *
 * The Wii Nunchuck reading code originally from Windmeadow Labs
 *   http://www.windmeadow.com/node/42
 */

void processNunchuckData()
{
  if(!nunchuck_present)
  {
    nunchuck_init();
  }

    int cnt=0;
    Wire.requestFrom (0x52, 6);// request data from nunchuck
    while (Wire.available ()) {
        // receive byte as an integer
        nunchuck_buf[cnt] = nunchuk_decode_byte( Wire.read() );
        cnt++;

    joy_x_axis = nunchuck_joyx();
    joy_y_axis = nunchuck_joyy();
    z_button = nunchuck_zbutton();
    c_button = nunchuck_cbutton();
    nunchuck_present = true; 

    send_zero();  // send request for next data payload
   }
}

// returns zbutton state: 1=pressed, 0=notpressed
static int nunchuck_zbutton()
{
    return ((nunchuck_buf[5] >> 0) & 1) ? 0 : 1;  // voodoo
}

// returns zbutton state: 1=pressed, 0=notpressed
static int nunchuck_cbutton()
{
    return ((nunchuck_buf[5] >> 1) & 1) ? 0 : 1;  // voodoo
}

// returns value of x-axis joystick
static int nunchuck_joyx()
{
    return nunchuck_buf[0]; 
}

// returns value of y-axis joystick
static int nunchuck_joyy()
{
    return nunchuck_buf[1];
}

// Encode data to format that most wiimote drivers except
// only needed if you use one of the regular wiimote drivers
static char nunchuk_decode_byte (char x)
{
    x = (x ^ 0x17) + 0x17;
    return x;
}

// initialize the I2C system, join the I2C bus,
// and tell the nunchuck we're talking to it

static void nunchuck_init()
{ 
    Wire.begin();                // join i2c bus as master
    Wire.beginTransmission(0x52);// transmit to device 0x52
    Wire.write((uint8_t)0x40);// sends memory address
    Wire.write((uint8_t)0x00);// sends sent a zero.  
    Wire.endTransmission();// stop transmitting
}

void send_zero ()
{
  Wire.beginTransmission (0x52); // transmit to device 0x52
  Wire.write((uint8_t)0x00);// sends one byte
  Wire.endTransmission ();	 // stop transmitting
}

