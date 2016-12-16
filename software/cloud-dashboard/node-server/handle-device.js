/*
 * handle-device.js
 * This module handles listening to the particle device stream and saving received data
 * in real time to the database (LogEvent models). It also has capability to send push 
 * notifications to clients using socketio when new packages arrive. 
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
var EventSource = require('eventsource'); // Pull in event source
var TemperatureEvent = require('./models/temperature-event');
var locMap = require('./config/device-map.js').locMap; 

function addRecord(parsedData, io){
	var newTemperatureEvent= {
		coreid:		parsedData.coreid,
		time:		new Date(parsedData.published_at),
		loc:		locMap[parsedData.coreid], 
		temps:		{
			HXCI:	parsedData.HXCI,
			HXCO:	parsedData.HXCO,
			HTR:	parsedData.HTR,
			HXHI:	parsedData.HXHI,
			HXHO:	parsedData.HXHO
		},
		valveStatus: parsedData.V // Null if doesn't exist
	}
	
	console.log(newTemperatureEvent);
	var newRecord = new TemperatureEvent(newTemperatureEvent);  
	newRecord.save(function(err, event){
		if(err) console.log("error in saving to database"+err);
	})
	io.emit(newTemperatureEvent.probeid, newRecord);
}

/**
 * parseMessage
 * Responsible for parsing a Particle Publish 
 * message from the ADPL platform. A sample message
 * looks like:
 * {
 * 	"data": {
 * 		"data":"HXCI:18.9,HXCO:21.8,HTR:80.6,HXHI:22.7,HXHO:19.0",
 * 		""ttl":"60",
 * 		"published_at":"2016-12-02T06:18:25.026Z",
 * 		"coreid":"4b0031000d51353432393339"
 * 	}
 * }
 * @param {object} message
 */
function parseMessage(message) {
	deviceData = JSON.parse(message.data); 
	const splitData = deviceData.data.split(",");
	
	// Parse Transducer data
	const parsedData = splitData.reduce((result, current) => {
		const currentSplit = current.split(":");
		result[currentSplit[0]] = currentSplit[1];
		return result;
	}, {}); 
	
	// Add metadata
	parsedData['published_at'] = deviceData['published_at'];
	parsedData['coreid'] = deviceData['coreid'];

	return parsedData;
}

function handleDevice(deviceUrl, io){
	var es = new EventSource(deviceUrl); // Listen to the stream
    es.addEventListener("TEMPS", function (message) {
        console.log("New Message");
		try {
  			parsedData = parseMessage(message); 
			addRecord(parsedData, io);
		} catch (err) {
			console.log("ERROR: Parsing Temps Message", message, err);	
		}
    });

    es.onerror = function (err) {
        console.log("ERROR (Likely Event Source)");
        console.log(err);
    };
} 

module.exports = handleDevice; // Export handleDevice function
