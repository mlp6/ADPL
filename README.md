#Anaerobic Digestion Pasteurization Latrine (ADPL) 

##Functional Specifications
##Collection Tank
* When the level in a collection tank is between a minimum and maximum level,
  the pump should be on for 5 minutes every hour. 
  
* When the level is below minimum, pump should be off. 
  
* When the level is above maximum, the pump should remain on. If I can run the
  level sensor on analog output, I want to record the readings to know the
  depth.

##Temperature Regulation
* Record temperatures at 5 points.

* If temperature 3 is below 68 C, a gas valve should open and an ignitor should
  spark for 5 seconds. Until the temperature reaches 72C, the valve should
  remain open, and the ignitor should spark for 5 seconds, turn off for 15
  minutes, and spark again for 5 seconds. Once the temperature is 72 C, the
  valve should shut and ignitor remain off.

##Repository Layout
* ```software/ADPL_complete/```: Arduino code and libraries
* ```hardware/```: DipTrace circuit schematic and PCB layout
* ```docs/```: spec sheets for components

##Licenses
* All software is covered my the MIT License.  See
  [software/ADPL_complete/LICENSE](software/ADPL_complete/LICENSE).

* All circuit schematics and PCB layouts are covered by a Creative Commons
  Attribution 4.0 International License (CC BY 4.0)
  [http://creativecommons.org/licenses/by/4.0/]
