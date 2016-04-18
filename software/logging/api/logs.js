var LogEvent = require('../models/LogEvent.js');

module.exports = {

	list: function(req,res){
		LogEvent.find({}, function(err, records){
			if(err) res.send(err);
			res.json(records); 
		});
	},
	listLoc: function(req,res){
		LogEvent.find({loc:req.params.loc}, function(err,records){
			if(err) res.send(err);
			res.json(records);
		});	

	},
	listPin: function(req, res){
		LogEvent.find({probeid:req.params.pin, loc:req.params.loc}, function(err,records){
			if(err) res.send(err);
			res.json(records);
		});	
	} 

}
