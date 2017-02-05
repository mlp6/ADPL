var MessageUtil = require('../../handle-device/message-util');

var handleDevice = require('../../handle-device');
var testData = require('./test-data');
var sinon = require('sinon'); 

describe('Temperature Event Publish', () => { 
	const parseMessageStub = sinon.stub(MessageUtil, 'parseMessage');
	const addRecordsStub = sinon.stub(MessageUtil, 'addRecords'); 
	handleDevice.handleDataMessage(testData.sampleTempEvent, {}); 

	it('called parseMessage', () => {
		expect(parseMessageStub.calledWith(testData.sampleTempEvent)).toBe(true);
	}); 
	it('called addRecords', () => {
		expect(addRecordsStub.called).toBe(true);
	});
});

