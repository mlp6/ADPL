# Anaerobic Digestion and Pasteurization Latrine (ADPL) 

Project Overview: http://sanitation.pratt.duke.edu/onsite-treatment

## Particle Electron (MCU)
The active MCU software is in `software/ADPL_electron` using the `v6.2.0` Electron firmware.

## Web Server
Live data streaming is available to view at http://dupont.egr.duke.edu .

The web application is responsible for listening to real-time streaming data from different ADPL stations, saving that data to a database, and making those data available for inspection and analysis via a RESTful API and dashboard-like front end. The web server performs several additional functions like integrating with Twilio to allow for text-based alerts and interaction (with many features yet to be implemented). 

### Backend (NodeJS) Docker Setup
The server can be run using `docker-compose` with the following steps: 
1. `cd software/cloud-dashboard/`
2. `DB_DATA=/scratch/adpl/data docker-compose up`

### Frontend (ReactJS) Configuration
The frontend is manually built, and its assets captured by the Docker backend.  After changes are made to the frontend, you need to:
1. `cd react-client/`
2. `npm install`
3. `npm run build`
4. Repeat the Docker setup above, adding the `--build` option to `docker-compose up --build` to rebuild the image.

## Data Download
As simple as: http://dupont.egr.duke.edu/api/data/temp/1

## Contributors
- Aaron Forbis Stokes (a.forbis.stokes@gmail.com)
- Daniel Hull
- Suyash Kumar (suyash.kumar@duke.edu)
- Graham Miller
- Cal Nightingale
- Jawad Hoballah
- Mark Palmeri (mlp6@duke.edu)
