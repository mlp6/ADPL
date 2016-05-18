var EventSource = require('eventsource'); // Pull in event source
var LogEvent = require('./models/LogEvent.js');
var locMap = require('./config/device-map.js').locMap; 


module.exports = function(deviceUrl, io){
	var es = new EventSource(deviceUrl); // Listen to the stream 
	es.addEventListener('HTR', function(message){ 
		console.log("New Message"); 
		realData = JSON.parse(message.data);
		realData["name"] = message.type;
		addRecord(realData, io); 
	}); 
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
