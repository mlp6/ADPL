/*
 * message.js
 * Module to hold all API function definitions dealing wih 
 * message handling (using twilio)
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
var LogEvent = require('../models/LogEvent.js');
var twilio_auth = require('./twilio_auth.js');

module.exports = {
	onText: function(req,res){
		if(req.body.Body == 'temp'){
			// Get latest data for probe 10 and send text back to number 
			LogEvent.find().sort('-time').find({probeid:10}, function(err, records){ 
				sendText(records[1], req);
				res.send("OK");
			});
			
		}

	}

}
function sendText(data, req){
	var client = require('twilio')(twilio_auth.sid, twilio_auth.auth);
	client.messages.create({
		body: "Latest Probe "+data.probeid+" temp in "+ data.loc+": " +data.temp+"C at "+data.time,
		to: req.body.From,
		from: req.body.To}, function(err, message){
		console.log(err);
		console.log(message)});


}
