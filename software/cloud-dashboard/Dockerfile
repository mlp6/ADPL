FROM    debian:jessie

RUN     apt-get update

RUN     apt-get install -y curl
RUN     curl -sL https://deb.nodesource.com/setup_6.x | bash -
RUN     apt-get install -y nodejs 
RUN     apt-get install -y build-essential

### Install proper repos for Mongodb and install
RUN     apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 0C49F3730359A14518585931BC711F9BA15703C6
RUN     echo "deb http://repo.mongodb.org/apt/debian jessie/mongodb-org/3.4 main" | tee /etc/apt/sources.list.d/mongodb-org-3.4.list
RUN     apt-get update
RUN     apt-get install -y mongodb-org

# Make default dirs for mongodb.
RUN     mkdir -p /data/db

#ENTRYPOINT ["/usr/bin/mongod"]

RUN     apt-get -y install supervisor
RUN     mkdir -p /var/log/supervisor

COPY    supervisord.conf /etc/supervisor/conf.d/supervisord.conf

COPY    . /src

WORKDIR /src/react-client
RUN     npm install --production
RUN	npm run build

WORKDIR /src/node-server
RUN     npm install --production

EXPOSE  9000

CMD     ["/usr/bin/supervisord"]
