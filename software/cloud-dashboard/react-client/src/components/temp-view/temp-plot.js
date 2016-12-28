import React, { Component } from 'react';
import {Button, IconButton} from 'react-toolbox/lib/button'; 
import {LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend} from 'recharts' 
import { Layout, NavDrawer, Panel, Sidebar } from 'react-toolbox'; 
import Slider from 'react-toolbox/lib/slider'; 

const LINE_COLORS = ['#e41a1c', '#377eb8', '#4daf4a', '#984ea3', '#ff7f00', '#ffff33']

class TempPlot extends Component {
	
	state = {
		downsampleFactor: 25
	}

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

	handleSliderChange = (slider, value) => {
		const newSliderState = {};
		newSliderState[slider] = value;
		this.setState(newSliderState);
	}

	renderPlot = () => {
		const data = this.props.temps.data.map( currentItem => {
			return { ...currentItem.temps, time: currentItem.time };
		});
		console.log(data.length);
		return ( 
			<div style={{}}>
			<Layout>
					<Panel>
							<LineChart width={700} height={300} data={this.downsampleArray(data, this.state.downsampleFactor).reverse()}>
								<XAxis dataKey="time" interval={30} tickFormatter={this.formatDate} />
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
					</Panel>
					<Sidebar pinned={this.props.sideBarOpen} width={6}>
						<div>
							<h4> Plot Options </h4>
							<p> Downsample Sampling Scale Factor </p>	
							<Slider 
								min={10} 
								max={100} 
								step={5}
								pinned
								snaps
								editable 
								value={this.state.downsampleFactor} 
								onChange={this.handleSliderChange.bind(this, 'downsampleFactor')}/>

						</div>
					</Sidebar>
			</Layout>
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
