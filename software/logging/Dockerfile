FROM    ubuntu:14.04

RUN     apt-get update

### Install proper repos for nodejs and install
RUN     apt-get install -y curl
RUN     curl -sL https://deb.nodesource.com/setup_4.x | sudo -E bash -
RUN     apt-get install -y nodejs

### Install proper repos for Mongodb and install
RUN     apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 7F0CEB10
RUN     echo 'deb http://downloads-distro.mongodb.org/repo/ubuntu-upstart dist 10gen' | tee /etc/apt/sources.list.d/mongodb.list
RUN     apt-get update
RUN     apt-get install -y mongodb-org

# Make default dirs for mongodb.
RUN     mkdir -p /data/db

#ENTRYPOINT ["/usr/bin/mongod"]

RUN     apt-get -y install supervisor
RUN     mkdir -p /var/log/supervisor

COPY    supervisord.conf /etc/supervisor/conf.d/supervisord.conf

COPY    . /src

WORKDIR /src

RUN     npm install --production

EXPOSE  9000

CMD     ["/usr/bin/supervisord"]
