FROM    ubuntu:14.04

RUN     apt-get update

### Install proper repos for nodejs and install
RUN     apt-get -y install curl
RUN     curl -sL https://deb.nodesource.com/setup_4.x | sudo -E bash -
RUN     apt-get install -y nodejs

### Install proper repos for Mongodb and install
RUN     apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 7F0CEB10
RUN     echo 'deb http://downloads-distro.mongodb.org/repo/ubuntu-upstart dist 10gen' | tee /etc/apt/sources.list.d/mongodb.list
RUN     apt-get update

# Make default dirs for mongodb.
RUN     mkdir /data && mkdir /data/db

### Install and configure authbind 
RUN     useradd -r adpl
RUN     apt-get -y install authbind 
RUN     touch /etc/authbind/byport/80 
RUN     chown adpl /etc/authbind/byport/80 
RUN     chmod 755 /etc/authbind/byport/80

COPY    package.json /src/package.json
RUN     cd /src; npm install --production

COPY    . /src

EXPOSE  80

USER    adpl
CMD     ["authbind", "node", "/src/server.js"]
