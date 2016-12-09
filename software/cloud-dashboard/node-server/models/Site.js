/*
 * The schema to hold incoming events
 */
var mongoose = require('mongoose');
var LogEvent = require('./LogEvent.js').schema; // Pull in LogEvent schema
var Schema = mongoose.Schema;
var Site  = new Schema({ 
	coreid:		{type: String, required: true, trim: true, unique: true},  // coreid of sender
	locStr:		{type: String, required: true, trim: true, unique: true}, // Location identifier 
	events: [LogEvent] 
});
module.exports = mongoose.model('Site', Site);
