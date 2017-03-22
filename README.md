# Anaerobic Digestion and Pasteurization Latrine (ADPL)
<a href="https://travis-ci.org/mlp6/ADPL"><img src="https://travis-ci.org/mlp6/ADPL.svg?branch=master" /></a>


## Projects Websites
* Overview: http://sanitation.pratt.duke.edu/onsite-treatment
* Data Dashboard: http://adpl.colab.duke.edu

### Temperature Regulation
* Record temperatures across 5[-7] thermocouples.
* If the heater temperature (``HTR``) is < 68 C, the gas valve opens and the
  ignitor is on for 10 s.
* The valve remains open until the heater temperature reaches 72 C; the ignitor
  should spark for 1 seconds, turn off for 15 min, and spark again for 1 s.
* Once the temperature is 72 C, the valve should shut and ignitor remain off.
* Temperatures are published to the web server every 2.5 min.
* Valve status will be polled when the dashboard is refreshed.

### Bucket Tips
* Bucket tips are recorded and counted.
* Count events are published to the web server.

### Pump Regulation
* Pump turns on for 10 s every 30 minutes 

### Repository Layout
* ```software/ADPL_electron/```: Particle Electron code and libraries
* ```software/cloud-dashboard/```: NodeJS/MongoDB server; ReactJS client
* ```hardware/```: control box circuit schematic, PCB layout, component spec
  sheets and bill of materials
* ```docs/```: API documentation [FUTURE]
* ```software/ADPL_uno/```: Arduino code and libraries (depracated)
* ```software/electron_system_firmware/```: system firmware on different field units

### Licenses
* All software is covered by the Apache v2.0 License.  See
  [software/ADPL_*/LICENSE](software/ADPL_complete/LICENSE).
* All circuit schematics and PCB layouts are covered by a Creative Commons
  Attribution 4.0 International License (CC BY 4.0)
  [http://creativecommons.org/licenses/by/4.0/]
