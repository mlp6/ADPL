import React, { Component } from 'react';
import ReactDOM from 'react-dom';
import ReactTestUtils from 'react-addons-test-utils'
import Plot from '../../../components/plot.js';

const props = {
	temps: {
		loading: false,
		data: [1, 2, 3]
	}
}

it('Plot renders', () => {
	const div = document.createElement('div');
	ReactDOM.render(React.createElement(Plot, props), div);
});

describe('Date formatting', () => {
	const component = ReactTestUtils.renderIntoDocument(React.createElement(Plot, props)); 
	// Date is set in this day to be timezone agnostic
 	// relative to server running this.
	// TODO(suyashkumar): revisit this test to determine if there's a better way to set up
	const testDate = new Date();
	testDate.setDate(23);
	testDate.setHours(23);
	testDate.setMonth(0);
	testDate.setMinutes(58);

	it('should format typical dates correctly', () => { 
		const expectedDateString = '1/23 23:58';
    process.stdout.write(expectedDateString);
		const formattedDate = component.formatDate(testDate);
		expect(formattedDate).toEqual(expectedDateString);
	})

	it('should render dates with zero padding correctly', () => {
		testDate.setMinutes(8);	
		const expectedDateString = '1/23 23:08';
		const formattedDate = component.formatDate(testDate);
		expect(formattedDate).toEqual(expectedDateString);
	});
})
