/*
 * logs.js
 * Module to hold all API functions dealing with logs
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
	listPin: function(req, res){
		TemperatureEvent.find({probeid: req.params.pin, loc: req.params.loc}, function(err, records){
			if(err) res.send(err);
			res.json(records);
		});	
	} 

}
