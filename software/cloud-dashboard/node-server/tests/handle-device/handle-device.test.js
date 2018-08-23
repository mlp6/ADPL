let testData = require('./test-data');
let sinon = require('sinon');


describe('Temperature Event Publish', () => {
  let MessageUtil = require('../../handle-device/message-util');
  let handleDevice = require('../../handle-device');

	const parseMessageStub = sinon.stub(MessageUtil, 'parseMessage');
	const addRecordsStub = sinon.stub(MessageUtil, 'addRecords'); 
	handleDevice.handleDataMessage(testData.sampleTempEvent, {}); 

	it('called parseMessage', () => {
		expect(parseMessageStub.calledWith(testData.sampleTempEvent)).toBe(true);
	}); 
	it('called addRecords', () => {
		expect(addRecordsStub.called).toBe(true);
	});
	sinon.restore();
});


