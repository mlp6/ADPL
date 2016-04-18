/*
 * Set up the application's routes.
 */

var express = require('express');
var logs = require('../api/logs.js');
module.exports = function(app) { 
	app.get('/api/list', logs.list);
	app.get('/api/list/:loc', logs.listLoc); 
	app.get('/api/list/:loc/:pin', logs.listPin);
};
