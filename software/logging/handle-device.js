/*
 * handle-device.js
 * This module handles listening to the particle device stream and saving recieved data 
 * in real time to the database (LogEvent models). It also has capability to send push 
 * notifications to clients using socketio when new packages arrive. 
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
var EventSource = require('eventsource'); // Pull in event source
var LogEvent = require('./models/LogEvent.js');
var locMap = require('./config/device-map.js').locMap; 


module.exports = function(deviceUrl, io){
	var es = new EventSource(deviceUrl); // Listen to the stream
    var temp_probes = ['HXCI', 'HXCO', 'HTR', 'HXHI', 'HXHO'];
    for (probe in temp_probes) {
        es.addEventListener(temp_probes[probe], function (message) {
            console.log("New Message");
            realData = JSON.parse(message.data);
            realData["name"] = message.type;
            addRecord(realData, io);
        });
    }
} 


function addRecord(data, io){
	var toAdd= {
		coreid:		data.coreid,
		time:		new Date(data.published_at),
		loc:		locMap[data.coreid],
		probeid:	data.name,
		temp:		data.data
	}
	console.log(toAdd);
	var newRecord = new LogEvent(toAdd);  
	newRecord.save(function(err,event){
		if(err) console.log("error in saving to database"+err);
	})
	io.emit('new',newRecord);
}
