/*
 * Set up the application's routes.
 */

var express = require('express');
var logs = require('../api/logs.js');
var message = require('../api/message.js'); 
var sites = require('../api/sites.js'); 

module.exports = function(app) { 
	app.get('/api/list', logs.list);
	app.get('/api/list/:loc', logs.listLoc); 
	app.get('/api/list/:loc/:pin', logs.listPin);
	app.get('/api/newsite/:locStr/:coreid',sites.newSite);
	app.get('/api/sites',sites.listSites);
	app.post('/api/message', message.onText);
};