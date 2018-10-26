/*
 * device-map.js
 * Holds mapping to/from coreid to internal loc identifier
 */
module.exports={
	locMap : {
		"3b001b000e51353432393339": "Kenya-Central",
    "47003c000251353337353037": "Kenya-North",
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
