/*
 * routes.js
 * Application routes are set up here.
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */

var express = require('express');
var logs = require('../api/logs.js');
var message = require('../api/message.js'); 
var sites = require('../api/sites.js');
var ondemand = require('../api/ondemand.js');
module.exports = function(app) { 
	app.get('/api/list', logs.list);
	app.get('/api/list/:loc', logs.listLoc); 
	app.get('/api/list-all/:loc',logs.listLocAll);
	app.get('/api/list/:loc/:pin', logs.listPin);
	app.get('/api/sites',sites.listSites);
	app.get('/api/gasOn/:loc',ondemand.getValveStatus);
	app.post('/api/message', message.onText);
};
