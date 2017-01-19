
const sampleTempEvent = {data: '{"data":"HXCI:-73.9,HXCO:-75.7,HTR:-74.7,HXHI:-75.1,HXHO:-74.9,V:0","ttl":"60","published_at":"2017-01-19T06:45:44.999Z","coreid":"400057000a51343334363138"}'}

const sampleBucketEvent = {data: '{"data":"0","ttl":"60","published_at":"2017-01-19T06:35:03.110Z","coreid":"2d0042000951343334363138"}'}


const tempEventParsedData = {"HTR": "-74.7", "HXCI": "-73.9", "HXCO": "-75.7", "HXHI": "-75.1", "HXHO": "-74.9", "V": "0", "coreid": "400057000a51343334363138", "published_at": "2017-01-19T06:45:44.999Z"}

const bucketEventParsedData = { 
	data: 0,
	ttl: '60',
	published_at: '2017-01-19T06:35:03.110Z',
	coreid: '2d0042000951343334363138' 
} 

module.exports = {
	sampleTempEvent,
	sampleBucketEvent,
	tempEventParsedData,
	bucketEventParsedData
}
