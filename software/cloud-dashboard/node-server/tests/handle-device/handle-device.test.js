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

describe('Do not reset connection on heartbeat if messages received', () => {
  let MessageUtil = require('../../handle-device/message-util');
  let handleDevice = require('../../handle-device');
  var clock;

  // Test that the connection is not reset if a message does come through
  let handleStreamFake = sinon.fake();
  sinon.replace(handleDevice, 'handleStream', handleStreamFake);
  clock = sinon.useFakeTimers();

  const parseMessageFake = sinon.fake();
  const addRecordsFake = sinon.fake();

  sinon.replace(MessageUtil, 'parseMessage', parseMessageFake);
  sinon.replace(MessageUtil, 'addRecords', addRecordsFake);

  // simulate receiving a message
  handleDevice.handleDataMessage(testData.sampleTempEvent, {});

  handleDevice.checkReceivedMessagesHeartbeat("", "");
  clock.tick(11001);

  it('did not reset the connection if sufficient messages were received before the heartbeat', () => {
    expect(handleStreamFake.calledOnce)
  });

  sinon.restore();
  clock.restore();
});

describe('Messages Received Heartbeat test', () => {
  let MessageUtil = require('../../handle-device/message-util');
  let handleDevice = require('../../handle-device.js');
  var clock;

  let handleStreamFake = sinon.fake();
  sinon.replace(handleDevice, 'handleStream', handleStreamFake);
  clock = sinon.useFakeTimers();

  handleDevice.checkReceivedMessagesHeartbeat("", "");
  clock.tick(11001);
  it('reset the connection and called handleStream', () => {
    expect(handleStreamFake.calledOnce);
  });
  sinon.restore();
  clock.restore();

});