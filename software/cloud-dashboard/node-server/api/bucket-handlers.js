/*
 * bucket-handlers.js
 * Module to hold handlers for API
 * calls dealing with bucket events
 *
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
var BucketEvent = require('../models/bucket-event');

function sendDBEvents(err, records, res) {
	if (err) {
		res.json(err);
	} else {
		res.json(records);
	}
}

module.exports = {
	list: function(req, res) {
		BucketEvent.find({}, (err, records) => sendDBEvents(err, records, res)); 
	}, 
	listLoc: function(req, res) {
		BucketEvent.find({loc: req.params.loc})
		.sort({time: -1})
		.exec((err, records) => sendDBEvents(err, records, res));
	},
	listLastNDays: function(req, res) { 
		const currentDate = new Date();
		//TODO: add validation on req.params.days
		currentDate.setDate(currentDate.getDate() - parseInt(req.params.days));
		BucketEvent.find({loc: req.params.loc, time: {'$gte': currentDate}})
		.sort({time: -1})
		.exec((err, records) => {
			if(err) res.send(err);
			res.json(records);
		}); 
	},

};
