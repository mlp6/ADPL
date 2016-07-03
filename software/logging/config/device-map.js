/*
 * device-map.js
 * Holds mapping to/from coreid to internal loc identifier
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
module.exports={
	locMap : {
		"1b0048000c51343334363138": "ADPLKenyaNorth0437",
        "48002d000b51343334363138": "ADPLKenyaCentral9822", 
		"440032000a51343334363138": "ADPLdukelab4601",
		"5a004e001751353338363036": "ADPLDukelab0278",
		"310057000951343334363138": "ADPLPhilippinesSagay3904",
		"170044000c51343334363138": "ADPLPhilippinesSubayon1817"
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
