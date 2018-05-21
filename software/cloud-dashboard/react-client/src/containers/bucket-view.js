import React, { Component } from 'react';
import TotalBucketTips from './bucket-view/total-bucket-tips';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import { fetchBucketTips, setDaysToFetch } from '../actions/index';
import PlotCard from '../components/plot-card';

const MILLISECONDS_PER_MINUTE = 60000;
const BIN_SIZE_MINS = 90;

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

		let binnedBucketTips = new Array(daysToFetch * 24 * 60 / binSize).fill(0);
		let binnedBucketTipsTimes = new Array(daysToFetch * 24 * 60 / binSize);
		const initialDate = new Date(bucketTips[0].time);
		binnedBucketTipsTimes[0] = initialDate;
		let nextBinStarts = new Date(initialDate.getTime() + (binSize * MILLISECONDS_PER_MINUTE));
		let currentBinCount = 0;

		let currentDate = initialDate;
		let i = 0;
		for (let bin=0; bin < binnedBucketTips.length; bin++) {
		    if (i === bucketTips.length) break;
			while(currentDate < nextBinStarts) {
                currentBinCount += bucketTips[i].data;
                i++;
                if (i === bucketTips.length) break;
				currentDate = new Date(bucketTips[i].time);
			}
			binnedBucketTips[bin] = currentBinCount;
            binnedBucketTipsTimes[bin + 1] = nextBinStarts;
			currentBinCount = 0;
			nextBinStarts = new Date(nextBinStarts.getTime() + (binSize * MILLISECONDS_PER_MINUTE));

		}


		return { binnedBucketTips, binnedBucketTipsTimes };

	};

	render() {
    var flowData = [];
		if (this.props.bucket.data.length > 0) {
            var {binnedBucketTips, binnedBucketTipsTimes} = this.generateBinnedFlowRate(this.props.bucket.data, BIN_SIZE_MINS, this.props.meta.daysToFetch);
            for (let i = 0; i < binnedBucketTips.length; i++) {
                if (binnedBucketTipsTimes[i] === undefined) {
                    flowData[i] = {};
                } else {
                    flowData[i] = {flow: (binnedBucketTips[i] * 0.325), time: binnedBucketTipsTimes[i].toString()}
                }
            }
        }

    var yAxisMinMax = [flowData.min(), flowData.max()];

		return ( 
			<div> 
				<TotalBucketTips 
					bucketData={this.props.bucket.data} 
					daysToFetch={this.props.meta.daysToFetch}
					loading={this.props.bucket.loading} />
                { this.props.bucket.data.length > 0 &&
                    <PlotCard
                        data={flowData}
                        yAxisMinMax={yAxisMinMax}
                        isLoading={this.props.bucket.loading}
                        fetchNewData={this.props.fetchBucketTips}
                        currentLocation={this.props.currentLocation}
                        setDaysToFetch={this.props.setDaysToFetch}
                        title={"Flow (L)"}
                        plotTitle={`Flow (L) (${this.props.currentLocation}, ${this.props.meta.daysToFetch} days, ${BIN_SIZE_MINS} min bins)`}/>
                }
			</div>
		);
	}
}
const mapDispatchToProps = dispatch => {
	return bindActionCreators({ fetchBucketTips, setDaysToFetch }, dispatch);
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
