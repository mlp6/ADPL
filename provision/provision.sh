#!/bin/sh
# This script provisions blank Ubunutu 14+ servers how
# I like 'em. Installs latest versions of NodeJS, mongodb,
# git, screen, and authbind, along with some minor config. 
# @author: Suyash Kumar

[ "$(whoami)" != "root" ] && exec sudo -- "$0" "$@" # Ensure user is root

apt-get update 

### Install proper repos for nodejs and install
apt-get -y install curl 
curl -sL https://deb.nodesource.com/setup_4.x | sudo -E bash - 
apt-get install -y nodejs

### Install proper repos for Mongodb and install
apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 7F0CEB10
echo 'deb http://downloads-distro.mongodb.org/repo/ubuntu-upstart dist 10gen' | tee /etc/apt/sources.list.d/mongodb.list
apt-get update
sudo apt-get install -y mongodb-org
# Make default dirs for mongodb. Make sure to chown later if 
# you want to run mongod not as root (or use --dbpath)
mkdir /data 
mkdir /data/db

### Install and configure authbind 
apt-get -y install authbind
touch /etc/authbind/byport/80
chown bitnami /etc/authbind/byport/80 # Replace bitnami with non-root user
chmod 755 /etc/authbind/byport/80

### Install git
apt-get -y install git

### Install VIM
apt-get -y install vim

### Install screen
apt-get -y install screen

