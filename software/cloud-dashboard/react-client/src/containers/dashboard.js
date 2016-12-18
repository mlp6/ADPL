import React, { Component } from 'react';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import { fetchTemps, fetchLocations, selectLocation} from '../actions/index';
import { Button } from 'react-toolbox/lib/button';
import {Tab, Tabs} from 'react-toolbox';
import LocationSelector from '../components/location-selector';
import TempView from '../components/temp-view';
import './dashboard.css';

class Dashboard extends Component {
	constructor(props) {
		super(props);
		this.state = {
			tabIndex: 0,
		}
		this.props.fetchLocations();
	}

	handleTabChange = index => {
		this.setState({tabIndex: index});
	}

	render() {
		console.log(this.props);
		return (
			<div className="dashboard"> 
				<LocationSelector 
					locations={this.props.locations} 
					selectLocation={this.props.selectLocation}
					fetchTemps={this.props.fetchTemps}/>
				{
					this.props.currentLocation &&
					<Tabs index={this.state.tabIndex} onChange={this.handleTabChange}>
						<Tab label="Temps">
							<TempView
								temps={this.props.temps} />
						</Tab>
						<Tab label="Bucket Tips">

						</Tab>
					</Tabs>
				}
			</div>
		)
	} 
}

const mapDispatchToProps = dispatch => {
	return bindActionCreators({ fetchTemps, fetchLocations, selectLocation }, dispatch);
}

const mapStateToProps = state => {
	return { 
		temps: state.temps,
		locations: state.locationData.locations,
		currentLocation: state.locationData.currentLocation
	}
}

export default connect(mapStateToProps, mapDispatchToProps)(Dashboard);