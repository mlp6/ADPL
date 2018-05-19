/*
 * temperature-event.js
 * The schema (and model) to hold incoming 
 * temperature log events
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
var mongoose = require('mongoose');
var Schema = mongoose.Schema;
var TemperatureEvent = new Schema({ 
	coreid:		{type: String, required: true, trim: true},  // coreid of sender
	loc:		{type: String, required: true, trim: true}, // Location identifier 
	time:		{type: Date, required: true}, // Time the data was published 
	temps:	{
		HXCI: 	{type: Number, required: false},
		HXCO: 	{type: Number, required: false}, 
		HTR:	{type: Number, required: false},
		HXHI:	{type: Number, required: false},
		HXHO:	{type: Number, required: false},
		E:      {type: Number, required: false},
	},
	valveStatus: {type: Boolean, required: false}, // Valve status at this event's time
});

module.exports = mongoose.model('TemperatureEvent', TemperatureEvent);
