import React from 'react';
import LoadingView from '../../components/util/loading-view';
import './total-bucket-tips/total-bucket-tips.css'; 
import Card  from 'react-toolbox/lib/card';

const getTotalBucketTips = (bucketData) => {
	return bucketData.reduce( (acc, val) => {
		return acc + val.data;
	}, 0);
}

const TotalBucketTips = props => {
	return (
		<div className="container"> 
			<Card className="container-card">
				{
					!props.loading &&
					<div>
						<p> Flow Rate in L/day (avg over {props.daysToFetch} days): </p>
						<h3>{(.325*getTotalBucketTips((props.bucketData))/props.daysToFetch).toPrecision(2)}</h3>
					</div>
				}
				{
					props.loading &&
					<LoadingView />
				}
			</Card> 
		</div>
	);
};

export default TotalBucketTips;
