/*
 * handle-device.js
 * This module handles listening to the particle device stream and saving received data
 * in real time to the database (LogEvent models). It also has capability to send push 
 * notifications to clients using socketio when new packages arrive. 
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
let EventSource = require('eventsource'); // Pull in event source
let MessageUtil = require('./handle-device/message-util');

let EventSourceRestartTime = 10000; // in ms

let es; // Instance of EventSource

let receivedSinceLastHeartbeat = 0;
const heartbeatInterval = 300000; // in ms
const heartbeatMessageCutoff = 1; // minimum messages that must be received per heartbeatInterval

function handleDataMessage(message, io) { 
	try {
		const parsedData = MessageUtil.parseMessage(message); 
		MessageUtil.addRecords(parsedData, io);
		receivedSinceLastHeartbeat++;
	} catch (err) {
		console.log("ERROR: Parsing DATA Message", message, err);	
	}
}

function checkReceivedMessagesHeartbeat(deviceUrl, io) {
	if (receivedSinceLastHeartbeat < heartbeatMessageCutoff) {
		es.close();
		setTimeout(handleStream, EventSourceRestartTime, deviceUrl, io);
		console.log("ERROR: Received messages since last heartbeat below threshold, restarting EventSource.");
	}

	// Reset receivedSinceLastHearbeat and schedule another call to checkReceivedMessagesHeartbeat
	receivedSinceLastHeartbeat = 0;
	setTimeout(checkReceivedMessagesHeartbeat, heartbeatInterval, deviceUrl, io);
}

function handleStream(deviceUrl, io) {
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
  	es.close();
  	setTimeout(handleStream, EventSourceRestartTime, deviceUrl, io); // Manual reinit of eventsource in error cases
  };

  // Schedule the initial call to checkReceivedMessagesHeartbeat
  setTimeout(checkReceivedMessagesHeartbeat, heartbeatInterval, deviceUrl, io);

} 

module.exports =  {
	handleStream, // Export handleDevice function
	handleDataMessage,
};
