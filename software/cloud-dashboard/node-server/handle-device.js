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

function handleDevice(deviceUrl, io){
	var es = new EventSource(deviceUrl); // Listen to the stream

	// Add temperature event listener
    es.addEventListener("TEMPS", function (message) {
        console.log("New Message");
		try {
  			const parsedData = TempUtil.parseMessage(message); 
			TempUtil.addRecord(parsedData, io);
		} catch (err) {
			console.log("ERROR: Parsing Temps Message", message, err);	
		}
    });

	// Add bucket tip listener
	es.addEventListener("BUCKET", function(message) {
		try {
			const parsedData = BucketUtil.parseMessage(message);
			BucketUtil.addRecord(parsedData, io);
		} catch (err) {
			console.log("ERROR: Parsing BUCKET Message", message, err);
		}

	});

    es.onerror = function (err) {
        console.log("ERROR (Likely Event Source)");
        console.log(err);
    };
} 

module.exports = handleDevice; // Export handleDevice function
