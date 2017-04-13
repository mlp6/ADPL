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
var dataHandlers = require('../api/data-handlers');

module.exports = function(app) { 
	// TODO: change temp endpoints to api/temp/list*
	app.get('/api/list', temperatureHandlers.list);
	app.get('/api/list/:loc', temperatureHandlers.listLoc); 
	app.get('/api/list/:loc/:days', temperatureHandlers.listLastNDays);
	app.get('/api/list-all/:loc',temperatureHandlers.listLocAll); 
	app.get('/api/sites',sites.listSites);
	app.get('/api/gasOn/:loc',ondemand.getValveStatus);
	app.post('/api/message', message.onText);

	// Bucket Endpoints
	app.get('/api/bucket/list', bucketHandlers.list);
	app.get('/api/bucket/list/:loc', bucketHandlers.listLoc);
	app.get('/api/bucket/list/:loc/:days', bucketHandlers.listLastNDays);

	// Download Data
	app.get('/api/data/:type/:days', dataHandlers.handleDataExport); // type is either "temp" or "bucket"
	app.get('/api/data/:type/:loc/:days', dataHandlers.handleDataExport); // type is either "temp" or "bucket"


};
