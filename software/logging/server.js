/*
 * server.js
 * Entry point for the ADPL web application
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
// Set up Express.js
var express = require('express');
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);

// Load libraries
var bodyParser = require('body-parser');
var morgan = require('morgan'); 

// Set up logging
app.use(morgan('dev'));

// Set up body parsing
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));

// Set up static content
app.use(express.static(__dirname + '/node_modules')); // client-side frameworks
app.use(express.static(__dirname + '/public')); // HTML, CSS 

// Connect to Mongodb
require('./config/db')();

// Set up app routes
require('./config/routes')(app);

// Set up device handler
var deviceURL = "https://api.particle.io/v1/devices/events?access_token=7883544edea996822936af401fad4209c2ba5627"; 
require('./handle-device.js')(deviceURL, io);

io.on('connection',function(socket){
	console.log("hello");
});

exports = module.exports = app;
if (!module.parent) {
  var port = process.env.PORT || 9000; // 9000 as default 
  server.listen(port, function() {
    console.log('Listening on port ' + port);
  });
}
