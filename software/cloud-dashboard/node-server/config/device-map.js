/*
 * device-map.js
 * Holds mapping to/from coreid to internal loc identifier
 */
module.exports={
	locMap : {
		"46005b000b51343334363138": "Kenya-Central",
    "4a003b000351353337353037": "Kenya-North",
    "230042001951353338363036": "Duke",
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
