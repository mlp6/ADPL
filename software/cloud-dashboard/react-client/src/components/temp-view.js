import React, { Component } from 'react';
import { Card, CardTitle } from 'react-toolbox/lib/card';
class TempView extends Component {
	render() {
		return (
			<Card style={{minHeight: '200px'}}>
				<CardTitle
					title="Metadata"
					subtitle="This is cool"/>
			</Card>
		);
	}
}

export default TempView;
