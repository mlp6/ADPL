/*
 * handle-device/bucket-util.js
 * A set of utility functions used to parse and save 
 * bucket events published from ADPL systems.
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */

var BucketEvent = require('../models/bucket-event'); 
var locMap = require('../config/device-map.js').locMap; 

/*
 * addRecord
 * Responsible for adding a bucket-event record
 * to the database
 * 
 * @param {object} parsedData: Output from parseMessage below
 * @param {object} io: socketio object
 */
function addRecord(parsedData, io) {
	var newBucketEvent = {
		coreid:		parsedData.coreid,
		time:		new Date(parsedData.published_at),
		loc:		locMap[parsedData.coreid],
		data:		parsedData.data,
	}

	console.log(newBucketEvent);
	var newRecord = new BucketEvent(newBucketEvent);
	newRecord.save(function(err, event) {
		if (err) console.log("error saving bucket event to database", err);
	});
	io.emit("BUCKET", newRecord);

}

/*
 * parseMessage
 * Parses bucket event published data from
 * ADPL devices. 
 *
 * @param {object} Particle publish message
 * @return {object} Parsed data
 */
function parseMessage(message) { 
	deviceData = JSON.parse(message.data);
	deviceData.data = parseInt(deviceData.data); 
	return deviceData;
}

module.exports = {
	parseMessage,
	addRecord,
}
