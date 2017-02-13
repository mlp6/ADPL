import React, { Component } from 'react';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import { fetchBucketTips } from '../actions/index';

class BucketView extends Component {
	constructor() {
		this.props.fetchBucketTips( '', 1)
	}
	render() {
		return (
			<div>
			hi
			{this.props.bucket}
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
