import React, { Component, PropTypes } from 'react';
import { Card, CardTitle } from 'react-toolbox/lib/card';
import ProgressBar from 'react-toolbox/lib/progress_bar';
import TempPlot from './temp-view/temp-plot';

class TempView extends Component {

	renderLoadingView = () => {
		return (
			<div style={{textAlign: 'center'}}>
				<ProgressBar type="circular" mode="indeterminate"/>
			</div>
		)
	} 

	render() {
		return ( 
			<div>
				<Card style={{minHeight: '100px'}}>
					<CardTitle
						title="Metadata"/> 
					{
						this.props.temps.loading && 
						this.renderLoadingView()
					}
				</Card>
				<Card style={{minHeight: '400px', marginTop: '20px'}}> 
					<CardTitle
						title="Temperature Data"
						subtitle="Last 3 Days"/>
					{
						this.props.temps.loading && 
						this.renderLoadingView()
					}
					<TempPlot temps={this.props.temps}/>
				</Card>
			</div>
		);
	}
}

TempView.propTypes ={
	temps: PropTypes.object.isRequired
}

export default TempView;
