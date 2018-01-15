FROM node:7.8.0 
RUN mkdir /work
COPY . /work
RUN cd /work && npm install
CMD cd /work && npm run start
