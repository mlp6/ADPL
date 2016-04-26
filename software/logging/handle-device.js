var EventSource = require('eventsource'); // Pull in event source 
var Site = require('./models/Site.js');
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
	Site.findOne({coreid: data.coreid}, function(err, currSite){
		currSite.events.push(toAdd);
		currSite.save(function (err){
			if(err) console.log(err);
		});
	}); 
	io.emit('new',toAdd);
}
