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
        var wordArray = req.body.Body.split(' ');
		if(wordArray[0] == 'temp'){
            var location = (wordArray.length > 1) ? wordArray[1] : 'ADPLDukelab0278';
			// Get latest data for probe 10 and send text back to number 
			LogEvent.find().sort('-time').find({loc: location}, function(err, records){ 
                console.log(records.slice(0,5));
				sendText(records.slice(0,5), req);
				res.send("OK");
			});
			
		}

	}

}
function sendText(data, req){
	var client = require('twilio')(twilio_auth.sid, twilio_auth.auth);
    var createTempString = function(item) {
        return `${item.probeid}: ${item.temp} C`;
    };
    var tempStrings = data.map(createTempString).join("\n");
    console.log(tempStrings);
	client.messages.create({
        body: `Latest Probe temps in ${data[1].loc} @ ${data[1].time}:\n${tempStrings}`,
		to: req.body.From,
		from: req.body.To}, function(err, message){
		console.log(err);
		console.log(message)});


}
