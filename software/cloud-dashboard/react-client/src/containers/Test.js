import React, { Component } from 'react';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import { fetchTemps } from '../actions/index';
import { Button } from 'react-toolbox/lib/button';

class Test extends Component {
	
	render() {
		console.log(this.props.temps);
	return (
		<div> 
			<Button onClick={this.props.fetchTemps} raised >Test AJAX</Button>
		</div>
	);
	}
}

const mapDispatchToProps = dispatch => {
	return bindActionCreators({ fetchTemps }, dispatch);
}

const mapStateToProps = state => {
	return { 
		temps: state.temps
	}
}

export default connect(mapStateToProps, mapDispatchToProps)(Test);
