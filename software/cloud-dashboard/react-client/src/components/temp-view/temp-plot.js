import React, { Component } from 'react';
import {LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend} from 'recharts' 

const LINE_COLORS = ['#e41a1c', '#377eb8', '#4daf4a', '#984ea3', '#ff7f00', '#ffff33']

class TempPlot extends Component {

	downsampleArray = (array, factor) => {
		const downsampledArray = [];
		for (let i = 0; i < array.length; i=i+factor) {
			downsampledArray.push(array[i]);	
		}
		return downsampledArray;
	}

	formatDate = date => { 
		const dateObj = new Date(date);
		return `${dateObj.getMonth().toString()}/${dateObj.getDate().toString()} ${dateObj.getHours()}:${dateObj.getMinutes()}`
	}

	renderPlot = () => {
		const data = this.props.temps.data.map( currentItem => {
			return { ...currentItem.temps, time: currentItem.time };
		});

		return ( 
			<div style={{}}>
			<LineChart width={700} height={300} data={this.downsampleArray(data, 30).reverse()}>
				<XAxis dataKey="time" interval={10} tickFormatter={this.formatDate} />
				<YAxis />
				{
					Object.keys(data[0]).map((currentItem, index) => {
						if (currentItem === 'time') return null;
						return (
							<Line 
								type="monotone"
								dataKey={currentItem} 
								key={currentItem} 
								stroke={LINE_COLORS[index]}/>
						);
					})
				}
				<Tooltip
					labelFormatter={this.formatDate}/>
				<Legend />
				<CartesianGrid strokeDasharray="3 3" />
			</LineChart> 
			</div>
		);
	}

	render() {
		return (
			<div>
			{
				!this.props.temps.loading &&
				this.renderPlot()	
			} 

			</div>
		);
	} 
}

export default TempPlot;
