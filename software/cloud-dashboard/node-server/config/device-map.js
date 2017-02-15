/*
 * device-map.js
 * Holds mapping to/from coreid to internal loc identifier
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 *
 * Updated (2017-02-03) by Mark Palmeri
 */
module.exports={
	locMap : {
		"220036000b51343136333035": "ADPLKenyaC2212",
		"4b0031000d51353432393339": "ADPLKenyaC6304",
        "3e001d000e51353432393339": "ADPLKenyaN6833",
        "400057000a51343334363138": "ADPLDuke0785",
        "20002a001951353338363036": "ADPLDuke6191",
		"310057000951343334363138": "Philippines-Sagay",
		"170044000c51343334363138": "Philippines-Subayon",
        "520051000351353337353037": "India",
        "2d0042000951343334363138": "ADPLIndia2723"
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
