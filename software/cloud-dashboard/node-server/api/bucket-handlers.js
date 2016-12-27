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
		BucketEvent.find({loc: req.params.loc}, (err, records) => sendDBEvents(err, records, res)); 

	} 

};
