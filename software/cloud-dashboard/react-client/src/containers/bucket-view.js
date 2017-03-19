import React, { Component } from 'react';
import TotalBucketTips from './bucket-view/total-bucket-tips';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import { fetchBucketTips } from '../actions/index';

class BucketView extends Component {
	constructor(props) {
		super(props);
		this.props.fetchBucketTips(this.props.currentLocation, this.props.meta.daysToFetch);
	}

	componentWillReceiveProps(nextProps) {
		if (this.props.currentLocation != nextProps.currentLocation) {
			// Refresh bucket tips for new location
			nextProps.fetchBucketTips(nextProps.currentLocation, nextProps.meta.daysToFetch);		
		}
	}

	render() { 
		console.log(this.props.bucket);
		return ( 
			<div> 
				<TotalBucketTips 
					bucketData={this.props.bucket.data} 
					daysToFetch={this.props.meta.daysToFetch}
					loading={this.props.bucket.loading} />	

			</div>
		);
	}
}
const mapDispatchToProps = dispatch => {
	return bindActionCreators({ fetchBucketTips }, dispatch);
}

const mapStateToProps = state => {
	return {
		bucket: state.bucket,
		locations: state.locationData.locations,
		currentLocation: state.locationData.currentLocation,
		meta: state.meta,
	}
}

export default connect(mapStateToProps, mapDispatchToProps)(BucketView);
