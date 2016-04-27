var EventSource = require('eventsource'); // Pull in event source
var LogEvent = require('./models/LogEvent.js');
var locMap = {
	"1b0048000c51343334363138": "kenya-1",
	"530031000a51343334363138": "durham-1"
}; // Maps coreids to a location identifier 

module.exports = function(deviceUrl, io){
	var es = new EventSource(deviceUrl); // Listen to the stream 
	es.addEventListener('temp_probe', function(message){ 
		console.log("New Message"); 
		realData = JSON.parse(message.data); 
		console.log(realData); 
		addRecord(realData, io); 
	}); 
} 


function addRecord(data, io){
	var toAdd= {
		coreid:		data.coreid,
		time:		new Date(data.published_at),
		loc:		locMap[data.coreid],
		probeid:	data.data.split(":")[0],
		temp:		data.data.split(":")[1]
	}
	console.log(toAdd);
	var newRecord = new LogEvent(toAdd);  
	newRecord.save(function(err,event){
		if(err) console.log("error in saving to database"+err);
	})
	io.emit('new',newRecord);
}
