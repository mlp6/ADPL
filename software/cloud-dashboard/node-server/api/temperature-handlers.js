/*
 * temperature-handlers.js
 * Module to hold all API functions dealing with temperature
 * events
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */ 
var TemperatureEvent = require('../models/temperature-event');

module.exports = { 
	list: function(req, res){
		TemperatureEvent.find({}, function(err, records){
			if(err) res.send(err);
			res.json(records); 
		});
	},
	listLocAll: function(req, res){
		TemperatureEvent.find({loc: req.params.loc}, function(err, records){
			if(err) res.send(err);
			res.json(records);
		});	
	},
	listLoc: function(req, res){
		TemperatureEvent.find({loc: req.params.loc})
		.sort({time:-1})
		.limit(9000)
		.exec(function(err, records){
			if(err) res.send(err);
			res.json(records);
		});	
	},
	listLastNDays: function(req, res) { 
		const daysToFetch = parseInt(req.params.days);

		// Basic validation:
		if (daysToFetch < 0) {
			res.status(400).send("Days to fetch must be positive");
		}

		fetchLastNDays(req, res, daysToFetch, req.params.loc, (err, records) => {
			if(err) res.status(500).send(err);
			res.json(records);
		}); 
	},
	fetchLastNDays: fetchLastNDays, // export this utility function

}

function fetchLastNDays(req, res, daysToFetch, loc, callback) { 
		const currentDate = new Date();
		currentDate.setDate(currentDate.getDate() - daysToFetch);

		const queryObject = {
			time: {'$gte': currentDate}
		} 

		if (loc) queryObject.loc = loc; 

		TemperatureEvent.find(queryObject)
		.sort({time: -1})
		.exec(callback); // call supplied callback
}

