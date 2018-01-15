import React, { Component } from 'react';

class Metadata extends Component {

	calculateAverages = tempData => {
		var initialAcc = {
			HXCI: tempData[0].temps.HXCI, 
			HXCO: tempData[0].temps.HXCO, 
			HTR: tempData[0].temps.HTR, 
			HXHI: tempData[0].temps.HXHI, 
			HXHO: tempData[0].temps.HXHO
		};

		var sums = tempData.reduce( (acc, curr, idx) => {
			if (idx === 0) return acc;
			acc.HXCI += curr.temps.HXCI
			acc.HXHI += curr.temps.HXHI
			acc.HXCO += curr.temps.HXCO
			acc.HTR  += curr.temps.HTR 
			acc.HXHO += curr.temps.HXHO 
			return acc;
		}, initialAcc);

		var averages = {}
		for (const key of Object.keys(sums)) {
			averages[key] = sums[key] / tempData.length	
		}
		return averages;
	}


	render() { 
		const averages = this.calculateAverages(this.props.temps.data)
		const getAverageTemps = () => {
			var avgTemps = []
			for (const key of Object.keys(averages)) {
				avgTemps.push((
					<div style={{display: 'inline-block', marginLeft: '40px'}} key={key}>
						<p> {key} </p>
						<h3> {averages[key].toPrecision(3)} </h3>
					</div>
				))
			}
			return avgTemps;
		}

		return (
			<div style={{textAlign: 'center', padding: '10px'}}>
					<h5 style={{marginBottom: '20px'}}> Average Temperatures (past {this.props.daysToFetch} days, ˚C) </h5>
					{ getAverageTemps() }
			</div>
		)
	}

}
export default Metadata;
