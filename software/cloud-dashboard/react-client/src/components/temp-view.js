import React, { Component, PropTypes } from 'react';
import { Card, CardTitle, CardText } from 'react-toolbox/lib/card';
import Notes from './temp-view/notes'
import LoadingView from './util/loading-view';
import './temp-view/temp-view.css';
import Metadata from './temp-view/metadata';
import PlotCard from './plot-card';

class TempView extends Component {

	state = {
		tempPlotSidebarOpen: false
	};

	toggleTempPlotSidebar = () => {
		return this.setState({tempPlotSidebarOpen: !this.state.tempPlotSidebarOpen});
	};

	render() {
		var tempData = [];
		if (this.props.temps.data) {
            tempData = this.props.temps.data.map(currentItem => {
                return {...currentItem.temps, time: currentItem.time};
            });
            tempData = tempData.reverse()
        }

    var yAxisMinMax = [0, 100];

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

				<PlotCard
					data={tempData}
          yAxisMinMax={yAxisMinMax}
					isLoading={this.props.temps.loading}
					fetchNewData={this.props.fetchTemps}
					currentLocation={this.props.currentLocation}
					setDaysToFetch={this.props.setDaysToFetch}
					title={"Temperature"}
					plotTitle={`Temperature (${this.props.currentLocation}, ${this.props.daysToFetch} days)`}/>

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
};

export default TempView;
