import React, { Component } from 'react';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import { fetchBucketTips } from '../actions/index';

class BucketView extends Component {
	constructor(props) {
		super(props);
		this.props.fetchBucketTips('ADPLKenyaN3763', 20)
	}
	render() {
		console.log('test')
		console.log(this.props.bucket);
		return (
			<div onClick={() => this.props.fetchBucketTips('ADPLKenyaN3763', 20)}>
			hi 
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
