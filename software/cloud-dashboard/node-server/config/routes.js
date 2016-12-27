/*
 * routes.js
 * Application routes are set up here.
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */

var express = require('express');
var temperatureHandlers = require('../api/temperature-handlers');
var bucketHandlers = require('../api/bucket-handlers');
var message = require('../api/message'); 
var sites = require('../api/sites');
var ondemand = require('../api/ondemand');

module.exports = function(app) { 
	app.get('/api/list', temperatureHandlers.list);
	app.get('/api/list/:loc', temperatureHandlers.listLoc); 
	app.get('/api/list-all/:loc',temperatureHandlers.listLocAll);
	app.get('/api/list/:loc/:pin', temperatureHandlers.listPin);
	app.get('/api/sites',sites.listSites);
	app.get('/api/gasOn/:loc',ondemand.getValveStatus);
	app.post('/api/message', message.onText);

	// Bucket Endpoints
	app.get('/api/bucket/list', bucketHandlers.list);
	app.get('/api/bucket/list/:loc', bucketHandlers.listLoc);
};
