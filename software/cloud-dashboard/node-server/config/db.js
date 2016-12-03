/*
 * db.js
 * Database configuration happens here
 * @author: Suyash Kumar <suyashkumar2003@gmail.com>
 */
var mongoose = require('mongoose');

var config = {
  url: process.env.MONGO_URI || 'mongodb://localhost/adpl'
}; // The default port of MongoDB is 27017

module.exports = function() {
  mongoose.connect(config.url);
};
