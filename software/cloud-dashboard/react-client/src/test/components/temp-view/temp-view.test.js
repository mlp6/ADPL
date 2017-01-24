import React, { Component } from 'react';
import ReactDOM from 'react-dom';
import ReactTestUtils from 'react-addons-test-utils'
import sinon from 'sinon';
import TempPlot from '../../../components/temp-view/temp-plot.js';

const props = {
	temps: {
		loading: false,
		data: [1, 2, 3]
	}
}

it('TempPlot renders', () => {
	const div = document.createElement('div');
	ReactDOM.render(React.createElement(TempPlot, props), div);
});

describe('Date formatting', () => {
	const component = ReactTestUtils.renderIntoDocument(React.createElement(TempPlot, props)); 

	it('should format typical dates correctly', () => {
		const testDate = new Date('2017-01-24T07:58:32.289Z'); // 1/24/2017 23:58
		const expectedDateString = '1/24 23:58';
		const formattedDate = component.formatDate(testDate);
		expect(formattedDate).toEqual(expectedDateString);
	})

	it('should render dates with zero padding correctly', () => {
		const testDate = new Date('2017-01-24T07:08:32.289Z'); // 1/24/2017 23:08
		const expectedDateString = '1/24 23:08';
		const formattedDate = component.formatDate(testDate);
		expect(formattedDate).toEqual(expectedDateString);
	});
})
