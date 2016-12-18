import React, { Component, PropTypes } from 'react';
import Dropdown from 'react-toolbox/lib/dropdown';

class LocationSelector extends Component {
	constructor(props) {
		super(props);
		this.state = {
			currentValue: null	
		} 

		this.locationData = []; 
	}

	componentWillReceiveProps(nextProps) {
		this.locationData = nextProps.locations.map( (loc, index) => {
			return {value: loc, label: loc}
		});
	}

	handleChange = value => {
		this.setState({currentValue: value});
		this.props.fetchTemps(value);
		this.props.selectLocation(value);
	}

	render() { 
		return (
			<div style={{maxWidth: '300px', margin:'auto'}}>
			<Dropdown
				auto
				label='Select ADPL Location'
				allowBlank={true}
				onChange={this.handleChange}
				source={this.locationData}
				value={this.state.currentValue}/>
			</div>
		)
	} 
}

LocationSelector.propTypes = {
	locations: PropTypes.array.isRequired,
	fetchTemps: PropTypes.func.isRequired, 
}

export default LocationSelector;
