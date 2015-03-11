#Anaerobic Digestion Pasteurization Latrine (ADPL) 

##Overview
What I am trying to do is operate two systems. 

One system runs a pump using an input for a level sensor. 
* If below the minimum, the pump remains off. 
* If above the maximum, the pump remains on. 
* If in between, the pump runs on a timer. 

The other system controls the temperature in a heating tank using temperature
probes. 
* If the temperature is below the minimum, a gas valve should open and an
  ignitor should go off on a timer. 
* Once the temperature goes above the maximum, the valve should close and
  ignitor stop.

##Operation Specs 
* Operate pump - When the level in a collection tank is between a minimum and
  maximum level, the pump should be on for 3 minutes every hour. When the level
  is below minimum, pump should be off. When the level is above maximum, the
  pump should remain on. If I can run the level sensor on analog output, I want
  to record the readings to know the depth.

* Record temperatures at 5 points.

* Operate burner - If temperature 3 is below 68 C, a gas valve should open and
  an ignitor should spark for 5 seconds. Until the temperature reaches 72C, the
  valve should remain open, and the ignitor should spark for 5 seconds, turn
  off for 15 minutes, and spark again for 5 seconds. Once the temperature is
  72 C, the valve should shut and ignitor remain off.

#Licenses
* All software is covered my the MIT License.  See software/ADPL_complete/LICENSE.
* All circuit schematics and PCB layouts are covered by a Creative Commons Attribution 4.0 International License (CC BY 4.0) [http://creativecommons.org/licenses/by/4.0/]
