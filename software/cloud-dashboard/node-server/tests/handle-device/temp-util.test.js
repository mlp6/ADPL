var testData = require('./test-data');

var MessageUtil = require('../../handle-device/message-util');

describe('MessageUtil tests', () => {
		const data = MessageUtil.parseMessage(testData.sampleTempEvent);	

	it('parseMessage returns correct data', () => { 
		expect(data).toEqual(testData.tempEventParsedData);
	});
});
