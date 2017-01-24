#Anaerobic Digestion and Pasteurization Latrine (ADPL)

##Projects Websites
* Overview: http://sanitation.pratt.duke.edu/onsite-treatment
* Data Dashboard: http://adpl.colab.duke.edu/new

###Temperature Regulation
* Record temperatures across 5[-7] thermocouples.
* If the heater temperature (``HTR``) is < 68 C, the gas valve opens and the
  ignitor sparks for 1 s.
* The valve remains open until the heater temperature reaches 72 C; the ignitor
  should spark for 1 seconds, turn off for 15 min, and spark again for 1 s.
* Once the temperature is 72 C, the valve should shut and ignitor remain off.
* Temperatures are published to the web server every 2.5 min.
* Valve status will be polled when the dashboard is refreshed.

###Bucket Tips
* Bucket tips are recorded and counted.
* Count events are published to the web server.

###Pump Regulation (India only)
* Pump turns on for 5 minutes, turns off for 55 minutes 
  (sequence starts with pump off so that any disconnection/loss of power does not turn pump on when rebooting)

###Repository Layout
* ```software/ADPL_electron/```: Particle Electron code and libraries
* ```software/logging/```: NodeJS/MongoDB webserver
* ```hardware/```: control box circuit schematic, PCB layout, component spec
  sheets and bill of materials
* ```docs/```: API documentation [FUTURE]
* ```software/ADPL_uno/```: Arduino code and libraries (depracated)

###Licenses
* All software is covered by the Apache v2.0 License.  See
  [software/ADPL_*/LICENSE](software/ADPL_complete/LICENSE).
* All circuit schematics and PCB layouts are covered by a Creative Commons
  Attribution 4.0 International License (CC BY 4.0)
  [http://creativecommons.org/licenses/by/4.0/]
