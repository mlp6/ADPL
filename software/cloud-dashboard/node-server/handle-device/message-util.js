/*
 * handle-device/temp-util.js
 * A set of utility functions used to parse and save 
 * DATA events published from ADPL systems.
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
var TemperatureEvent = require('../models/temperature-event');
var BucketEvent = require('../models/bucket-event');
var locMap = require('../config/device-map.js').locMap; 

/*
 * addRecords
 * Responsible for adding information from DATA publish 
 * events to the database. This includes temperature data,
 * valve status data, and optional bucket tip data.
 *
 * @param {object} parsedData: See output from parseMessage function below
 * @param {oject} io: socketio object
 */
function addRecords(parsedData, io){
	addTemperatureRecord(parsedData, io);
	parsedData.B && addBucketRecord(parsedData, io); // Add only if bucket data is present
}

function addBucketRecord(parsedData, io) {
	var newBucketEvent = {
		coreid:	parsedData.coreid,
		loc:	locMap[parsedData.coreid],
		time:	new Date(parsedData.published_at),
		data:	parsedData.B
	};

	console.log(newBucketEvent);
	var newBucketRecord = new BucketEvent(newBucketEvent);
	newBucketRecord.save((err, event) => {
		if(err) console.log("error in saving to database" + err);
	});
	io.emit(newBucketEvent.coreid, newBucketRecord);

}

function addTemperatureRecord(parsedData, io) {
	var newTemperatureEvent = {
		coreid:		parsedData.coreid,
		time:		new Date(parsedData.published_at),
		loc:		locMap[parsedData.coreid], 
		temps:		{
			HXCI:	parsedData.HXCI,
			HXCO:	parsedData.HXCO,
			HTR:	parsedData.HTR,
			HXHI:	parsedData.HXHI,
			HXHO:	parsedData.HXHO,
			E:      parsedData.E, 
		},
		valveStatus: parsedData.V // undefined if doesn't exist
	}
	
	console.log(newTemperatureEvent);
	var newRecord = new TemperatureEvent(newTemperatureEvent);  
	newRecord.save((err, event) => {
		if(err) console.log("error in saving to database" + err);
	}); 
	io.emit(newTemperatureEvent.loc, newRecord);
}

/**
 * parseMessage
 * Responsible for parsing a Particle Temperature Publish 
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
 * This function returns a map of all the transducer fields to 
 * corresponding values along with published_at and coreid 
 * (e.g. {HXHO: 100, ... , coreid: "", published_at: ""})
 * @param {object} message
 * @return {object} a map of data fields to their values (including transducers) 
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

module.exports = {
	parseMessage,
	addRecords
};
