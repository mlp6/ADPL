var testData = require('./test-data');
var sinon = require('sinon'); 

var BucketUtil = require('../../handle-device/bucket-util');

describe('BucketUtil Tests', () => {
	const data = BucketUtil.parseMessage(testData.sampleBucketEvent);	
	it('parseMessage returns the right data', () => {
		expect(data).toEqual(testData.bucketEventParsedData);
	});
});
