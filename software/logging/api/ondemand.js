/*
 * ondemand.js
 * Holds all API function definitions relating to polling ADPL site variables 
 * on demand
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
var request = require("request"); 
var deviceMap = require("../config/device-map.js");
module.exports = {
	getValveStatus: function(req,res){
		var revMap = deviceMap.locMapRev();
		var coreid = revMap[req.params.loc]; 
		request("https://api.particle.io/v1/devices/"+coreid+"/gasOn?access_token=7883544edea996822936af401fad4209c2ba5627", function(error,response,body){
			if (error) console.log(error); 
			try {
				var parsed = JSON.parse(body);
			} catch (e) {
				res.send("error");
			}
			if (!parsed || parsed.hasOwnProperty("error")){
				res.send("error");
			} else {
				res.json(parsed.result);
			}
		});


	}

}
