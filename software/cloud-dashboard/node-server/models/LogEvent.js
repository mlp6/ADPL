/*
 * LogEvent.js
 * The schema (and model) to hold incoming events
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
var mongoose = require('mongoose');
var Schema = mongoose.Schema;
var LogEvent  = new Schema({ 
	coreid:		{type: String, required: true, trim: true},  // coreid of sender
	loc:		{type: String, required: true, trim: true}, // Location identifier 
	time:		{type: Date, required: true}, // Time the data was published
	probeid:	{type: String, required: true}, // the pin the probe is on
	temp:		{type: Number, required: true} // in degrees C
});
module.exports = mongoose.model('LogEvent', LogEvent);
