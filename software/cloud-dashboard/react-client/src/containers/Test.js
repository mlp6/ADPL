import React, { Component } from 'react';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import { fetchTemps } from '../actions/index';

class Test extends Component {
	
	render() {
		console.log(this.props.temps);
	return (
		<div>
			Test AJAX  
			<button onClick={this.props.fetchTemps}>Hi</button>
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
