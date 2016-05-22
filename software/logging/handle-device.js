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
        // readData.data, published by the Electron, is close, but not valid, JSON;
        // true JSON formatting will cost more characters, so will just manually
        // parse the data
		for (d in realData.data.split(",")) {
			realData.probeid = realData.data.split(",")[d].split(":")[0].replace(/\s+/g, '');
			realData.temp = realData.data.split(",")[d].split(":")[1].replace(/\s+/g, '');
			addRecord(realData, io);
		}
    });
} 


function addRecord(data, io){
	var toAdd= {
		coreid:		data.coreid,
		time:		new Date(data.published_at),
		loc:		locMap[data.coreid],
		probeid:	data.probeid,
		temp:		data.temp
	}
	console.log(toAdd);
	var newRecord = new LogEvent(toAdd);  
	newRecord.save(function(err,event){
		if(err) console.log("error in saving to database"+err);
	})
	io.emit(data.name,newRecord);
}
