var BucketUtil = require('../../handle-device/bucket-util');
var TempUtil = require('../../handle-device/temp-util');

var handleDevice = require('../../handle-device');
var testData = require('./test-data');
var sinon = require('sinon'); 

describe('Temperature Event Publish', () => { 
	const parseMessageStub = sinon.stub(TempUtil, 'parseMessage');
	const addRecordStub = sinon.stub(TempUtil, 'addRecord'); 
	handleDevice.handleTemperatureMessage(testData.sampleTempEvent, {}); 

	it('called parseMessage', () => {
		expect(parseMessageStub.calledWith(testData.sampleTempEvent)).toBe(true);
	}); 
	it('called addRecord', () => {
		expect(addRecordStub.called).toBe(true);
	});
});

describe('Bucket Event Publish', () => { 
	const parseMessageStub = sinon.stub(BucketUtil, 'parseMessage');
	const addRecordStub = sinon.stub(BucketUtil, 'addRecord'); 
	handleDevice.handleBucketMessage(testData.sampleBucketEvent, {});

	it('called parseMessage', () => {
		expect(parseMessageStub.calledWith(testData.sampleBucketEvent)).toBe(true);
	});
	it('called addRecord', () => {
		expect(addRecordStub.called).toBe(true);
	});
});
