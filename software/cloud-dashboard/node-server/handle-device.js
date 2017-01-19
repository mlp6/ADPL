/*
 * handle-device.js
 * This module handles listening to the particle device stream and saving received data
 * in real time to the database (LogEvent models). It also has capability to send push 
 * notifications to clients using socketio when new packages arrive. 
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
var EventSource = require('eventsource'); // Pull in event source
var TempUtil = require('./handle-device/temp-util');
var BucketUtil = require('./handle-device/bucket-util');

function handleTemperatureMessage(message, io) { 
	try {
		const parsedData = TempUtil.parseMessage(message); 
		TempUtil.addRecord(parsedData, io);
	} catch (err) {
		console.log("ERROR: Parsing Temps Message", message, err);	
	}
}

function handleBucketMessage(message, io) {
	try {
		const parsedData = BucketUtil.parseMessage(message);
		BucketUtil.addRecord(parsedData, io);
	} catch (err) {
		console.log("ERROR: Parsing BUCKET Message", message, err);
	}

}

function handleStream(deviceUrl, io){
	var es = new EventSource(deviceUrl); // Listen to the stream

	// Add temperature event listener
    es.addEventListener("TEMPS", (message) => {
		handleTemperatureMessage(message, io);
	});

	// Add bucket tip listener
	es.addEventListener("BUCKET", (message) => {
		handleBucketMessage(message, io);	
	});

    es.onerror = function (err) {
        console.log("ERROR (Likely Event Source)");
        console.log(err);
    };
} 

module.exports =  {
	handleStream, // Export handleDevice function
	handleTemperatureMessage,
	handleBucketMessage
}
