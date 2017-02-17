import React, { Component } from 'react';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import { fetchTemps, fetchLocations, selectLocation, setDaysToFetch} from '../actions/index';
import { Button } from 'react-toolbox/lib/button';
import BucketView from './bucket-view';
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
	// TODO: Refactor LocationSelector and TempView into containers 
	render() {
		return (
			<div className="dashboard"> 
				<LocationSelector 
					locations={this.props.locations} 
					selectLocation={this.props.selectLocation}
					fetchTemps={this.props.fetchTemps}
					meta={this.props.meta}/>
				{
					this.props.currentLocation &&
					<Tabs index={this.state.tabIndex} onChange={this.handleTabChange}>
						<Tab label="Temps">
							<TempView
								temps={this.props.temps} 
								fetchTemps={this.props.fetchTemps}
								setDaysToFetch={this.props.setDaysToFetch}
								currentLocation={this.props.currentLocation}/>
						</Tab>
						<Tab label="Bucket Tips">
							<BucketView />
								
						</Tab>
					</Tabs>
				}

				<div className="links"> 
					<a href="http://sanitation.pratt.duke.edu/onsite-treatment/about-onsite-treatment-project" target="_blank"> ADPL Project </a> | 
					<a href="https://github.com/mlp6/ADPL" target="_blank"> Github </a> |
					Web by <a href="http://suyashkumar.com" target="_blank"> Suyash Kumar </a>
				</div>
			</div>
		)
	} 
}

const mapDispatchToProps = dispatch => {
	return bindActionCreators({ fetchTemps, fetchLocations, selectLocation, setDaysToFetch}, dispatch);
}

const mapStateToProps = state => {
	return { 
		temps: state.temps,
		locations: state.locationData.locations,
		currentLocation: state.locationData.currentLocation,
		meta: state.meta,
	}
}

export default connect(mapStateToProps, mapDispatchToProps)(Dashboard);
