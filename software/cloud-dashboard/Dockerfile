FROM node:latest

# Initialize Workspace
WORKDIR /app
RUN mkdir /app/node-server
RUN mkdir /app/react-client
COPY ./node-server /app/node-server
COPY ./react-client /app/react-client

# Install and build frontend dependencies. This will build frontend static assets
# and place them in the node-server/public directory for serving.
RUN cd /app/react-client && npm install
RUN cd /app/react-client && npm run build

# Install server dependencies
RUN cd /app/node-server && npm install

CMD node server.js
