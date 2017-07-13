FROM    debian:jessie
RUN     apt-get update
RUN     apt-get install -y curl git

# install nodejs
RUN     curl -sL https://deb.nodesource.com/setup_6.x | bash -
RUN     apt-get install -y nodejs 
RUN     apt-get install -y build-essential

# Install proper repos for Mongodb, install and setup
RUN     apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 0C49F3730359A14518585931BC711F9BA15703C6
RUN     echo "deb http://repo.mongodb.org/apt/debian jessie/mongodb-org/3.4 main" | tee /etc/apt/sources.list.d/mongodb-org-3.4.list
RUN     apt-get update
RUN     apt-get install -y mongodb-org
RUN     mkdir -p /data/db

# install supervisor to run mongodb and nodejs
RUN     apt-get -y install supervisor
RUN     mkdir -p /var/log/supervisor

# clone the ADPL repository
WORKDIR /src
RUN	git clone https://github.com/mlp6/ADPL.git
### THESE LINES ARE TEMPORARY 
WORKDIR /src/ADPL
RUN	git checkout docker-react-build
###
COPY    /src/ADPL/software/cloud-dashboard/supervisord.conf /etc/supervisor/conf.d/supervisord.conf

# setup & build the reactjs client
WORKDIR /src/ADPL/software/cloud-dashboard/react-client
RUN     npm install
RUN	npm run build

# setup node server
WORKDIR /src/ADPL/software/cloud-dashboard/node-server
RUN     npm install --production
EXPOSE  9000

# kick everything off!
CMD     ["/usr/bin/supervisord"]
