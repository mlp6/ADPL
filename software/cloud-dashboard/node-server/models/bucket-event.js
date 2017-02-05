/*
 * bucket-event.js
 * The schema to hold incoming bucket
 * tip events from ADPL sites.
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
var mongoose = require('mongoose');
var Schema = mongoose.Schema;
var BucketEvent = new Schema({
	coreid: {type: String, required: true, trim: true}, // Particle core id of sender
	loc:	{type: String, required: true, trim: true}, // Location identifier
	time:	{type: Date, required: true}, // Datetime of publish
	data:	{type: Number, required: false}, // number of bucket tips since last publish 
});

module.exports = mongoose.model('BucketEvent', BucketEvent);
