var testData = require('./test-data');
var sinon = require('sinon'); 

var TempUtil = require('../../handle-device/temp-util');

describe('TempUtil tests', () => {
		const data = TempUtil.parseMessage(testData.sampleTempEvent);	

	it('parseMessage returns correct data', () => { 
		expect(data).toEqual(testData.tempEventParsedData);
	});
});
