angular.module('adplApp', ['ngMaterial', 'ngMessages'])

.controller('ADPLCtrl',  ['$scope','$http', function($scope, $http) {
	console.log('start');
	$scope.locations = ["kenya-1", "india-1"];
	$scope.channels = ["10","11","12","13","15"];
	$scope.currLoc= "";
	$scope.plotHide=true;	
	$scope.currChannel=""
	
	/*
	 * Called when location selection is changed
	 */
	$scope.changeLoc = function(){
		if ($scope.currChannel != ""){
		$scope.plotHide=false;
		}
		$scope.updateView();
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

var updateGraph = function(data){
	nv.addGraph(function() {
  var chart = nv.models.lineChart()
                .margin({left: 100})  //Adjust chart margins to give the x-axis some breathing room.
                .useInteractiveGuideline(true)  //We want nice looking tooltips and a guideline!  
                .showLegend(true)       //Show the legend, allowing users to turn on/off line series.
                .showYAxis(true)        //Show the y-axis
                .showXAxis(true)        //Show the x-axis
  ;

  chart.xAxis     //Chart x-axis settings
      .axisLabel('Time and Date')
		.tickFormat(function(d) { return d3.time.format('%x %X')(new Date(d)); });
	

chart.xScale(d3.time.scale.utc());
  chart.yAxis     //Chart y-axis settings
      .axisLabel('Temperature (C)')
      .tickFormat(d3.format('.02f'));

  /* Done setting the chart up? Time to render it!*/
 

  d3.select('#pulse svg')    //Select the <svg> element you want to render the chart in.   
      .datum(data)         //Populate the <svg> element with chart data...
      .call(chart)          //Finally, render the chart!
	  .style({ 'width': "100%", 'height': 300 });
d3.select('#pulse svg').datum(data).transition().duration(500).call(chart).style({ 'width': "100%", 'height': 300 });
	
  //Update the chart when window resizes.
  nv.utils.windowResize(function() { chart.update() });
  return chart;
});
}
