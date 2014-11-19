Attached are two sets of Arduino code. The "ADPL_labsetup" is the more complete
file. This was made using the level sensor as a digital output, only sending a
signal when the minimum or maximum levels were exceeded. The "Pump_analog" was
created to test the level sensor output when used as an analog output ranging
from 4-20 mA, depending on depth.

To explain what I am trying to do (as simply as possible):

* Operate pump - When the level in a collection tank is between a minimum and
  maximum level, the pump should be on for 3 minutes every hour. When the level
  is below minimum, pump should be off. When the level is above maximum, the
  pump should remain on. If I can run the level sensor on analog output, I want
  to record the readings to know the depth.

* Record temperatures at 5 points.

* Operate burner - If temperature 3 is below 68C, a gas valve should open and
  an ignitor should spark for 5 seconds. Until the temperature reaches 72C, the
  valve should remain open, and the ignitor should spark for 5 seconds, turn
  off for 15 minutes, and spark again for 5 seconds. Once the temperature is
  72C, the valve should shut and ignitor remain off.
