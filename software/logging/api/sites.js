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
		var siteMap = require('../config/device-map.js').locMap;
		var sites = []
		for (var key in siteMap) {
		    if (Object.prototype.hasOwnProperty.call(siteMap, key)) {
			        sites.push(siteMap[key]);
					
					}
			}
			res.json(sites);

	}


}
