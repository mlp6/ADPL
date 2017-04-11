/*
 * data-handlers.js
 * Module to hold all handler functions related to
 * data export and download.
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */ 
var TemperatureEvent = require('../models/temperature-event');
var TemperatureHandlers = require('./temperature-handlers');

const temperatureHeaders = [
	'Date', 'Location', 'coreid', 'Valve Status', 'HXCI', 'HXCO', 'HTR', 'HXHI', 'HXHO',
]

function fetchAndSendTemperatureData(req, res) { 
	// Get daysToFetch
	//TODO: Consolidate daysToFetch parsing/validation in util package
	const daysToFetch = parseInt(req.params.days); 
	// Basic validation:
	if (daysToFetch < 0) {
		res.status(400).send("Days to fetch must be positive");
	}
	
	TemperatureHandlers.fetchLastNDays(req, res, daysToFetch, req.params.loc, (err, records) => {
		if (err) res.status(500).send(err); // send err if needed
		data = records.map( item => {
			return [
				item.time,
				item.loc,
				item.coreid,
				item.valveStatus,
				item.temps.HXCI,
				item.temps.HXCO,
				item.temps.HTR,
				item.temps.HXHI,
				item.temps.HXHO,
			]
		});
		data.unshift(temperatureHeaders); // Insert headers at front of data array 
		
		// Prepare response
		res.set({
			"Content-Disposition": 'attachment; filename="data.csv"'
		});
		res.csv(data);
	});
}

module.exports = {
	handleDataExport: fetchAndSendTemperatureData 
}

