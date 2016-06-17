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
RUN     apt-get install -y mongodb-org

# Make default dirs for mongodb.
RUN     mkdir -p /data/db

COPY    package.json /src/package.json

WORKDIR /src

RUN     npm install --production

COPY    . /src

EXPOSE  9000

CMD     ["node", "server.js"]
