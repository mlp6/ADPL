var Site = require('../models/Site.js');

module.exports = {
	newSite: function(req,res){
		var site = new Site({coreid:req.params.coreid, locStr: req.params.locStr});
		site.save(function(err){
			if(err)	{
				res.send(err);
			}
			else{
				res.send("OK");
			}
		}); 
	},
	listSites: function(req,res){
		Site.find({},function(err, sites){
			if (err) console.log(err);
			var toReturn = sites.map(function(a) { return {"locStr":a.locStr, "coreid":a.coreid}});
			res.json(toReturn); 
		});
	}


}
