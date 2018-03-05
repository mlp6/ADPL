import React, { Component, PropTypes } from 'react';
import ProgressBar from 'react-toolbox/lib/progress_bar';

const LoadingView = props => {
	return (
		<div style={{textAlign: 'center'}}>
			<ProgressBar type="circular" mode="indeterminate"/>
		</div>
	)
};

export default LoadingView;
