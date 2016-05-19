# ADPL Server 

This web application is responsible for listening to real-time streaming data from different ADPL stations, saving that data to a database, and making those data available for inspection and analysis via a RESTful API and dashboard-like front end. The web server performs several additional functions like integrating with Twilio to allow for text-based alerts and interaction (with many features yet to be implemented). 

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