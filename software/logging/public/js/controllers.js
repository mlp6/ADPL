angular.module('adplApp', ['ngMaterial', 'ngMessages'])

.controller('ADPLCtrl',  ['$scope','$http', function($scope, $http) {
	console.log('start'); 
	updateLocations();
	$scope.currLoc= "";
	$scope.plotHide=true;	
	$scope.currChannel=""


	function updateLocations(){ 
		$http.get('/api/sites').success(
			function(data){

				$scope.locations=[];
				for (i=0;i<data.length;i++){
					$scope.locations.push(data[i]);
				}
				console.log($scope.locations);
			});
	}
	/*
	 * Called when location selection is changed
	 */
	$scope.changeLoc = function(){ 
		$scope.plotHide=false;
		$scope.updateViewAll(); 

	};
	$scope.updateViewAll = function(){
		$http.get('/api/list/'+$scope.currLoc).success(
			function(data) {

				var plotData = {}; // key is probeid, value is array of {x: , y: } objects

				for (i=0;i<data.length;i++){
					if (!(data[i].probeid in plotData)){
						plotData[data[i].probeid] = [];	
					} 
					if (data[i].temp < 0){
						continue;
					}
					plotData[data[i].probeid].push({x:new Date(data[i].time), y:data[i].temp});
				}
				console.log(plotData);
				// build plot array
				var plotArray = []; 
				for (var key in plotData){
					if (plotData.hasOwnProperty(key)){
						var currSeries = {"key":key, "values":plotData[key]};
						plotArray.push(currSeries); 
						console.log("h");
					}
				}
				console.log(plotArray);
				updateGraph(plotArray);
				$scope.plotTitle = "Temperature Data ("+$scope.currLoc+")";
				$scope.plotData = plotData;
			}); 
	}
	$scope.updateViewSingle = function(){
		$http.get('/api/list/'+$scope.currLoc+"/"+$cope.currChannel).success(
			function(data) {
				console.log(data[1])
				var plotData = [];
				for (i=0;i<data.length;i++){
					plotData.push({x:new Date(data[i].time), y:data[i].temp});
				}
				updateGraph([{"key":"TempData", "values":plotData}]);
				$scope.plotTitle = "Temp vs. Time (Channel " + $scope.currChannel+")";
			}); 
	}
	$scope.saveSVG = function(){


	}
	$scope.saveCSV = function(data){
		var csvStr = "Probe ID: ,";
		var keys = Object.keys(data); // holds the probe ids 
		csvStr = writeHeader(csvStr, data);
		csvStr = writeData(csvStr, data);
		// Save CSV String using blob
		var blob = new Blob([csvStr], {type:"text/csv;charset=utf-8"});
		saveAs(blob, "data.csv");

	}


}]);
function getLongestLength(data){
	var longLen = 0;
	for (var pID in data){
			// keep track of longest set of data
			if (data[pID].length >longLen){
				longLen =data[pID].length
			}
		} 
	return longLen;

}
function writeHeader(csvStr, data){
	for (var pID in data){
		// Write header
		csvStr = csvStr + pID +", ,";
	}
	csvStr=csvStr+"\n"; // New line for actual data start
	return csvStr;

}
function writeData(csvStr, data){
	var longLen = getLongestLength(data);
	for (i=0;i<longLen;i++){ // Each row in csv
		csvStr=csvStr + " , "; // Skip column for the probe id label
		for (var pID in data){
			if (i<data[pID].length){ // in case num data points different for each probe ID's dataset 
				csvStr=csvStr + data[pID][i]["x"].toString() + "," + data[pID][i]["y"].toString()+","; 
			}
			else{
				csvStr=csvStr + " , ";
			}
		}
		csvStr=csvStr+"\n" 
	}
	return csvStr;
		
}
function getInnerWidth(elem) {
    return parseFloat(window.getComputedStyle(document.getElementById(elem)).width);
}

var updateGraph = function(data){
  var offset = 100;
	nv.addGraph(function() {
  var chart = nv.models.lineWithFocusChart()
  .width(getInnerWidth('graph')-offset)
  .height(500);
	chart.useInteractiveGuideline(true)
  chart.xAxis     //Chart x-axis settings 
      .axisLabel('Time and Date') 
		.tickFormat(function(d) { return d3.time.format('%x %X')(new Date(d)); }) ;


  chart.x2Axis     //Chart x-axis settings
      .axisLabel('Time and Date')
		.tickFormat(function(d) { return d3.time.format('%x %X')(new Date(d)); });

	
chart.xScale(d3.time.scale.utc());

  chart.yAxis     //Chart y-axis settings
      .axisLabel('Temperature (C)')
      .tickFormat(d3.format('.02f'));

  chart.y2Axis     //Chart y-axis settings
      .axisLabel('Temp (C)')
      .tickFormat(d3.format('.02f')) 




  /* Done setting the chart up? Time to render it!*/
 

  d3.select('#graph svg')    //Select the <svg> element you want to render the chart in.   
      .datum(data)         //Populate the <svg> element with chart data...  
	  .style({ 'width': getInnerWidth('graph'), 'height': 500, 'margin-left':50 }) 
      .call(chart);          //Finally, render the chart!
//d3.select('#graph svg').datum(data).transition().duration(500).call(chart)
	
  //Update the chart when window resizes.
  nv.utils.windowResize(function(){ 
	chart.width(getInnerWidth('graph')-offset);
	d3.select('#graph svg')    //Select the <svg> element you want to render the chart in.   
      .datum(data)         //Populate the <svg> element with chart data...  
	  .style({ 'width': getInnerWidth('graph'), 'height': 500, 'margin-left':50 }) 
  	chart.update(); 
	});

  return chart;
});
}
