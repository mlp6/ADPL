/*
 * device-map.js
 * Holds mapping to/from coreid to internal loc identifier
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
module.exports={
	locMap : {
		"1b0048000c51343334363138": "ADPLKenyaNorth0437",
        "48002d000b51343334363138": "ADPLKenyaCentral9822", 
		"400057000a51343334363138": "ADPLDuke0785",
		"20002a001951353338363036": "ADPLDuke6191",
		"310057000951343334363138": "ADPLPhilippinesSagay3904",
		"170044000c51343334363138": "ADPLPhilippinesSubayon1817",
        "430055000951343334363138": "ADPLIndia3847",
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
