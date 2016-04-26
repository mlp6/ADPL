var LogEvent = require('../models/LogEvent.js').model;
var Site = require('../models/Site.js');
module.exports = {

	list: function(req,res){
		Site.find({}, function(err, records){
			if(err) res.send(err);
			res.json(records); 
		});
	},
	listLoc: function(req,res){
		Site.findOne({locStr:req.params.loc}, function(err,site){
			if(err) res.send(err);
			res.json(site.events);
		});	

	},
	listPin: function(req, res){
		LogEvent.find({probeid:req.params.pin, loc:req.params.loc}, function(err,records){
			if(err) res.send(err);
			res.json(records);
		});	
	} 

}
