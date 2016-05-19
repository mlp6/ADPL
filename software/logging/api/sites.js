/*
 * sites.js
 * Holds API function definitions dealing with listing new sites and adding new sites. 
 * newSite is currently defunct.
 * @author: Suyash Kumar <suyashkumar2003@gmail.com> *
 */
var Site = require('../models/Site.js');

module.exports = {
	// Not used currently: 
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
