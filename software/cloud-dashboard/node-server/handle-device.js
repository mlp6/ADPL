/*
 * handle-device.js
 * This module handles listening to the particle device stream and saving received data
 * in real time to the database (LogEvent models). It also has capability to send push 
 * notifications to clients using socketio when new packages arrive. 
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
var EventSource = require('eventsource'); // Pull in event source
var MessageUtil = require('./handle-device/message-util');

var EventSourceRestartTime = 10000; // in ms

var es; // Instance of EventSource

function handleDataMessage(message, io) { 
	try {
		const parsedData = MessageUtil.parseMessage(message); 
		MessageUtil.addRecords(parsedData, io);
	} catch (err) {
		console.log("ERROR: Parsing DATA Message", message, err);	
	}
}

function handleStream(deviceUrl, io){
	es = new EventSource(deviceUrl); // Listen to the stream

	// Add temperature event listener (DATA event on devices with latest firmware)
    es.addEventListener("TEMPS", (message) => {
		handleDataMessage(message, io);
	});
	
	// Add new DATA event listener
    es.addEventListener("DATA", (message) => {
		handleDataMessage(message, io);
	});

    es.onerror = function (err) {
        console.log("ERROR (Likely Event Source)");
        console.log(err);
		setTimeout(handleStream, EventSourceRestartTime, deviceUrl, io); // Manual reinit of eventsource in error cases
    };
} 

module.exports =  {
	handleStream, // Export handleDevice function
	handleDataMessage,
}
