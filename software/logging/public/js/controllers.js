angular.module('adplApp', ['ngMaterial', 'ngMessages'])

.controller('ADPLCtrl',  ['$scope','$http', function($scope, $http) {
	console.log('start');
	$scope.locations = ["kenya-1", "india-1"];
	$scope.channels = ["10","11","12","13","14"];
	$scope.currLoc= "";
	$scope.plotHide=true;	
	$scope.currChannel=""
	
	/*
	 * Called when location selection is changed
	 */
	$scope.changeLoc = function(){
		if ($scope.currChannel != ""){
		$scope.plotHide=false;
		$scope.updateView();

		}
		console.log($scope.currLoc);
	};
	$scope.updateView = function(){
		$http.get('/api/list/'+$scope.currLoc+"/"+$scope.currChannel).success(
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

}]);
function getInnerWidth(elem) {
    return parseFloat(window.getComputedStyle(document.getElementById(elem)).width);
}

var updateGraph = function(data){
  var offset = 100;
	nv.addGraph(function() {
  var chart = nv.models.lineWithFocusChart()
  .width(getInnerWidth('graph')-offset)
  .height(500);

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
  nv.utils.windowResize( chart.update() );
  return chart;
});
}
