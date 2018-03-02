import React, { Component } from 'react';
import TotalBucketTips from './bucket-view/total-bucket-tips';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import { fetchBucketTips } from '../actions/index';

const MILLISECONDS_PER_MINUTE = 60000;

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
	// generateBinnedFlow rate calculates the number of bucketTips per binSize (which is specified in mins)
	generateBinnedFlowRate = (bucketTips, binSize, daysToFetch) => {
		bucketTips.sort( (a, b) => {
			const aDate = new Date(a.time);
			const bDate = new Date(b.time);
			if (aDate < bDate) return -1;
			if (aDate > bDate) return 1;
			return 0;
		});
		console.log(bucketTips);
		let binnedBucketTips = new Array(daysToFetch * 24 * 60 / binSize).fill(0);
		const initialDate = new Date(bucketTips[0].time);
		let nextBinStarts = new Date(initialDate.getTime() + (binSize * MILLISECONDS_PER_MINUTE));
		let currentBinCount = 0;
		let currentBinIndex = 0;

		console.log(initialDate);
		console.log(nextBinStarts);
		let currentDate = initialDate;
		let i = 0;
		for (let bin=0; bin < binnedBucketTips.length; bin++) {
			console.log("---bin iter---", bin);
			console.log("Next Bin Starts: ", nextBinStarts);
			console.log("current date: ", currentDate)

			while(currentDate < nextBinStarts) {
                currentBinCount += bucketTips[i].data;
                console.log("Added to bin", currentDate);
                i++;
                if (i == bucketTips.length){
                    console.log("finished")
                    console.log("Current date:", currentDate);
                    console.log("Next Bin Starts:", nextBinStarts);
                    return binnedBucketTips;
                }
				currentDate = new Date(bucketTips[i].time);
			}
			binnedBucketTips[bin] = currentBinCount;
			currentBinCount = 0;
			nextBinStarts = new Date(nextBinStarts.getTime() + (binSize * MILLISECONDS_PER_MINUTE));



		}


		return binnedBucketTips;

	};

	render() {
		if (this.props.bucket.data.length > 0) {
		    console.log(this.props.meta.daysToFetch);
            var binnedFlowRate = this.generateBinnedFlowRate(this.props.bucket.data, 30, this.props.meta.daysToFetch);
            console.log(binnedFlowRate);
        }

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
};

const mapStateToProps = state => {
	return {
		bucket: state.bucket,
		locations: state.locationData.locations,
		currentLocation: state.locationData.currentLocation,
		meta: state.meta,
	}
};

export default connect(mapStateToProps, mapDispatchToProps)(BucketView);
