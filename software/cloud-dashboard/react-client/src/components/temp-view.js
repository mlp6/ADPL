import React, { Component, PropTypes } from 'react';
import { Card, CardTitle, CardText } from 'react-toolbox/lib/card';
import {Button, IconButton} from 'react-toolbox/lib/button'; 
import TempPlot from './temp-view/temp-plot';
import Notes from './temp-view/notes';
import LoadingView from './util/loading-view';
import './temp-view/temp-view.css';
import Metadata from './temp-view/metadata';

class TempView extends Component {

	state = {
		tempPlotSidebarOpen: false
	}

	toggleTempPlotSidebar = () => {
		return this.setState({tempPlotSidebarOpen: !this.state.tempPlotSidebarOpen});
	}

	render() {
		return ( 
			<div>
				<Card className="card" style={{minHeight: '100px'}}>
					{
						this.props.temps.loading && 
						<div style={{margin: '10px'}}>
							<LoadingView />
						</div>
					}
					{
						!this.props.temps.loading &&
						(
							<div style={{margin: '10px'}}>
								<Metadata 
									temps={this.props.temps}
									daysToFetch={this.props.daysToFetch}/>	
							</div>
						)
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
						<LoadingView />
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
