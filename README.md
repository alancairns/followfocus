# Arduino Follow Focus with Wii Nunchuck

When looking for a DIY Follow Focus system I stumbled upon [this](http://weblogs.thinktecture.com/digitalfollowfocus/) amazing, and I believe now deprecated, digital follow focus project using an Arduino, stepper motor and a Wii Nunchuck.

Here is a modified working version of the original project, which is truly a remarkable piece of work. Ingo Rammer, the author has produced an amazing project. However when I initially downloaded the project I could not get it to work. This was due in part to changes to the Arduino IDE. Ingo Rammer posted the simplified version of the code in May 2010 (I am writing this in December 2012) and as he said, he had been unable to test this simplified version before posting it. 

As a retired computer support engineer I have little more than a basic appreciation of the C language and until recently had never heard of Arduino.  So the changes made here work for me but could likely be coded more efficiently by someone proficient in the C language. I can only apologise to Ingo Rammer if my changes here are amateurish. 

The Arduino IDE version I used was 1.0.1 and have also included [code](http://todbot.com/blog/2008/02/18/wiichuck-wii-nunchuck-adapter-available/ ) from Tod E Kurt at to resolve a Nunchuck problem and [code](http://www.uchobby.com/index.php/2012/01/21/replacing-delay-in-arduino-sketches-istime-to-the-rescue/) from uC Hobby at to measure elapsed time. The code changes I made have a start and end comment “Added by AC”

The following is a summary of the main changes and additions:
	
	- Replaced the setupAndStartTimes code as original seemed incomplete;
	- Reworked the Nunchuck sketch using code from Tod E Kurt as I couldn’t get the original to work;
	- Changed the value subtracted from joy_move_axis in the mainLoop to compensate for the Nunchuck values returned when my joystick was at rest.  Presumably this will vary from Nunchuck to Nunchuck;
	- Added code to disable the L6226 H-Bridge output once the motor had stopped in order to conserve battery power.  This also uses the IsTime() function from uC Hobby;
	- I focus the lens at infinity before powering up the Arduino and this is recorded as the maximum position;
	- Minimum position is recorded by pressing the C and Z buttons together, as Ingo suggested;
	- I had to add my own code to compensate for belt slack, as I couldn’t understand why Ingo’s original code wouldn’t work. My code works but I am sure this could have been coded more efficiently by a ‘real’ C programmer;
	- I did make up the L6226 H-bridge according to page 17 of the L6226N’s datasheet using the Arduino protoshield but the finished product looked messy. Then I found the [STMicroelectronics demonstration board](http://uk.futureelectronics.com/en/technologies/development-tools/analog-power/Pages/8295875-EVAL6226QR.aspx?IM=0) using a dual full-bridge L6226Q, which is ideal. 

To attach the stepper motor to the camera using the toothed belt I made a simple L-shaped bracket and attached it to a rail block from [Amazon](http://www.amazon.co.uk/gp/product/B0084LIX2O/ref=oh_details_o01_s00_i00) and the Gearbox Video Accessory cage and GearBox GB-R Kit - Rod Adapter w/ 15mm Rails from [photographyandcinema.com](http://www.photographyandcinema.com/). The Gearbox is a brilliant device for mounting the motor and other devices and at a very attractive price.  Speaking of suppliers I live in the UK and found many of the sundry items such as on/off switches, heat-shrink tubing, etc at [ESR](http://www.esr.co.uk/).

Lastly, I had quite a struggle to get the motor working correctly. A combination of incorrect wiring and a lack of understanding about how the motor windings were supposed to be energised (Ingo Rammers code is very sophisticated).  The diagram below may help clarify things and shows the connections from the Sparkfun stepper motor through the L6226 to the Arduino Uno and the mapping of the Arduino output pins, the ATMega counter registers and the ATMega output pins.

![Diagram]()

## License

[Creative Common 3.0, Attribution, Share Alike, Non-Commercial](http://creativecommons.org/licenses/by-nc-sa/3.0/). Original author [Ingo Rammer](http://weblogs.thinktecture.com/)digitalfollowfocus/)