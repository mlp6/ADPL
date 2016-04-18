var LogEvent = require('../models/LogEvent.js');

module.exports = {

	list: function(req,res){
		LogEvent.find({}, function(err, records){
			if(err) res.send(err);
			res.json(records); 
		});

	}


}
