/*
 * device-map.js
 * Holds mapping to/from coreid to internal loc identifier
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 *
 * Updated (2017-02-03) by Mark Palmeri (mlp6@duke.edu)
 * Updated (2017-02-15) by Mark Palmeri (mlp6@duke.edu)
 */
module.exports={
	locMap : {
		"46005b000b51343334363138": "Kenya-Central",
        "47003c000251353337353037": "Kenya-North",
        "400057000a51343334363138": "Duke",
		"390037000451353431383736": "Philippines-Sagay",
		"350055000551353431383736": "Philippines-Subayon",
        "520051000351353337353037": "India",
	}, // Maps coreids to a location identifier
	locMapRev : function(){ 
		revMap = {};
		for (key in this.locMap){
			if (this.locMap.hasOwnProperty(key)) {
				revMap[this.locMap[key]]=key;
			}
		}
		return revMap
	} // returns the reverse of locMap
}
