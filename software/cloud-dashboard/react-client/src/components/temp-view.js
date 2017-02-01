import React, { Component, PropTypes } from 'react';
import { Card, CardTitle, CardText } from 'react-toolbox/lib/card';
import {Button, IconButton} from 'react-toolbox/lib/button'; 
import ProgressBar from 'react-toolbox/lib/progress_bar';
import TempPlot from './temp-view/temp-plot';
import Notes from './temp-view/notes';
import './temp-view/temp-view.css';

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
				<Card className="card" style={{minHeight: '100px'}}>
					<CardTitle title="Metadata"/> 
					{
						this.props.temps.loading && 
						this.renderLoadingView()
					}
				</Card>

				<Card className="card" style={{minHeight: '400px'}}> 
					<div style={{margin: '20px'}}>
						<h4 style={{display: 'inline-block', float: 'left'}}> Temperature </h4>
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
						fetchTemps={this.props.fetchTemps}
						currentLocation={this.props.currentLocation}
						toggleSidebar={this.toggleTempPlotSidebar}
						setDaysToFetch={this.props.setDaysToFetch}
						sideBarOpen={this.state.tempPlotSidebarOpen}/>
				</Card>

				<Card className="card"> 
					<CardTitle title="Notes"/> 
						<CardText>
							<Notes currentLocation={this.props.currentLocation}/>					
						</CardText>
					</Card>
			</div>
		);
	}
}

TempView.propTypes ={
	temps: PropTypes.object.isRequired
}

export default TempView;
