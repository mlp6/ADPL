/*
 * handle-device.js
 * This module handles listening to the particle device stream and saving received data
 * in real time to the database (LogEvent models). It also has capability to send push 
 * notifications to clients using socketio when new packages arrive. 
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
var EventSource = require('eventsource'); // Pull in event source
var LogEvent = require('./models/LogEvent.js');
var locMap = require('./config/device-map.js').locMap; 


module.exports = function(deviceUrl, io){
	var es = new EventSource(deviceUrl); // Listen to the stream
    es.addEventListener("TEMPS", function (message) {
        console.log("New Message");
        realData = JSON.parse(message.data);
        try {
            probeTempSamples = realData.data.split(",")
            probeTempSamples.map((probeTempSample) => {
                addRecord(probeTempSample, realData.coreid, realData.published_at, io);
            }); 
        } catch (err) {
            console.log("ERROR parsing and saving TEMPS message");
            console.log("Message", message);
            console.log("Error:", err);
        }
    });

    es.onerror = function (err) {
        console.log("ERROR (Likely Event Source)");
        console.log(err);
    };
} 


function addRecord(probeTempSample, coreid, publishedAt, io){
	var toAdd= {
		coreid:		coreid,
		time:		new Date(publishedAt),
		loc:		locMap[coreid],
		probeid:	probeTempSample.split(":")[0].trim(),
		temp:	    probeTempSample.split(":")[1].trim()	
	}
	console.log(toAdd);
	var newRecord = new LogEvent(toAdd);  
	newRecord.save(function(err, event){
		if(err) console.log("error in saving to database"+err);
	})
	io.emit(toAdd.probeid, newRecord);
}
