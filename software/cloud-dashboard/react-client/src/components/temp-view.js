import React, { Component, PropTypes } from 'react';
import { Card, CardTitle } from 'react-toolbox/lib/card';
import {Button, IconButton} from 'react-toolbox/lib/button'; 
import ProgressBar from 'react-toolbox/lib/progress_bar';
import TempPlot from './temp-view/temp-plot';

class TempView extends Component {

	state = {
		tempPlotSidebarOpen: false
	}

	renderLoadingView = () => {
		return (
			<div style={{textAlign: 'center'}}>
				<ProgressBar type="circular" mode="indeterminate"/>
			</div>
		)
	} 

	toggleTempPlotSidebar = () => {
		return this.setState({tempPlotSidebarOpen: !this.state.tempPlotSidebarOpen});
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
					<div style={{margin: '20px'}}>
						<h3 style={{display: 'inline-block', float: 'left'}}> Temperature </h3>
						<IconButton 
							className="material-icons" 
							style={{float: 'right'}} 
							icon='menu'
							onClick={this.toggleTempPlotSidebar}/>
					</div>
					{
						this.props.temps.loading && 
						this.renderLoadingView()
					}
					<TempPlot 
						temps={this.props.temps} 
						sideBarOpen={this.state.tempPlotSidebarOpen}/>
				</Card>
			</div>
		);
	}
}

TempView.propTypes ={
	temps: PropTypes.object.isRequired
}

export default TempView;
