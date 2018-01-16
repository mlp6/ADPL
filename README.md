Anaerobic Digestion and Pasteurization Latrine (ADPL) 
=====================================================

Project Overview: http://sanitation.pratt.duke.edu/onsite-treatment

Particle Electron (MCU)
-----------------------
The active MCU software is in `software/ADPL_electron` using the `v6.2.0` Electron firmware.

Web Server
----------
Live data streaming is avaialble to view at http://adpl.suyash.io .

The web application is responsible for listening to real-time streaming data from different ADPL stations, saving that data to a database, and making those data available for inspection and analysis via a RESTful API and dashboard-like front end. The web server performs several additional functions like integrating with Twilio to allow for text-based alerts and interaction (with many features yet to be implemented). 

## Setup
To install this web application on a vanilla debian-flavored linux server:
  1. Install git: ```sudo apt-get install git```
  2. Clone & run Suyash's provision script (a subtree is also available in ``provision/``): 
  
  ```bash
  git clone https://github.com/suyashkumar/provision.git
  cd provision
  ./provision.sh
  ```

## Run the Application
After provisioning a server as noted above:
  1. Clone this repository: `git clone https://github.com/mlp6/ADPL.git`
  2. Travel to the logging folder: `cd ADPL/software/logging`
  3. Install dependencies: `npm install`
  4. Set the port to run on: `export PORT=80`
  5. Run using authbind if running on a privliged port: `authbind node server.js`

## Automate Server Start on Boot
Once the kinks in the server setup are sorted out, the server can be automatically started on boot by adding the
following line to ``/etc/rc.local``:

```
sudo -iu bitnami /usr/bin/screen -dmS server /bin/bash /home/bitnami/ADPL/software/logging/start_adpl_server.sh
```

## Docker Container
We also have a working Docker image that can be used to start a server
container.  Right now the image is hosted on Docker Hub at ``mlp6/adpl``.  The
image can be built in the ``software/cloud-dashboard/node-server`` directory using:
```
docker build -t mlp6/adpl .
```
This will read the ``Dockerfile`` in that directory to set everything up.  The
`supervisord.conf` file is used to concurrently run the MongoDB and Node JS 
services.

Once the image is built, you can push this to Docker Hub using:
```
docker push mlp6/adpl
```

On the server VM, you can pull the image:
```
docker pull mlp6/adpl
```

Finally, you can run the container as follows:
```
docker run -p 80:9000 -d mlp6/adpl
```

Contributors
------------
- Aaron Forbis Stokes (a.forbis.stokes@gmail.com)
- Daniel Hull
- Suyash Kumar (suyash.kumar@duke.edu)
- Graham Miller
- Cal Nightingale
- Jawad Hoballah
- Mark Palmeri (mlp6@duke.edu)
