import React, { Component } from 'react';
import {Button, IconButton} from 'react-toolbox/lib/button'; 
import {LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer} from 'recharts' 
import { Layout, NavDrawer, Panel, Sidebar } from 'react-toolbox'; 
import Slider from 'react-toolbox/lib/slider'; 
import constants from '../../constants';
import Input from 'react-toolbox/lib/input';
import './temp-plot/temp-plot.css';

const LINE_COLORS = ['#e41a1c', '#377eb8', '#4daf4a', '#984ea3', '#ff7f00', '#ffff33']

const defaultNumberOfPoints = 300;

class TempPlot extends Component { 

	state = {
		downsampleFactor: 15,
		daysToFetch: constants.daysToFetch,
		daysToShow: constants.daysToFetch,
	}

	downsampleArray = (array, factor) => {
		const downsampledArray = [];
		for (let i = 0; i < array.length; i=i+factor) {
			downsampledArray.push(array[i]);	
		}
		return downsampledArray;
	}

	filterDatesToShow = (array) => {
		const cutoffDate = new Date();
		cutoffDate.setDate(cutoffDate.getDate() - this.state.daysToShow);

		array = array.filter(value => { 
			const date = new Date(value.time); 
			return date > cutoffDate; 
		});

		return array;
	}
	
	formatDate = date => { 
		const dateObj = new Date(date);
		const getMinutes = minuteString => (minuteString.length > 1) ? minuteString : "0" + minuteString;
		
		return `${dateObj.getMonth()+1}/${dateObj.getDate()} ${dateObj.getHours()}:${getMinutes(dateObj.getMinutes().toString())}`
	}

	handleSliderChange = (slider, value) => {
		const newSliderState = {};
		newSliderState[slider] = value;
		this.setState(newSliderState);
	} 

	handleDaysToFetchChange = () => { 
		this.props.fetchTemps(this.props.currentLocation, this.state.daysToFetch);
		this.props.setDaysToFetch(this.state.daysToFetch);
		this.setState({daysToShow: this.state.daysToFetch}); 
		this.props.toggleSidebar(); 
	} 

	componentWillReceiveProps(nextProps) { 
		if (nextProps.temps.data.length > defaultNumberOfPoints && 
			(this.props.temps.data && (nextProps.temps.data.length != this.props.temps.data.length))) {
			this.setState({downsampleFactor: Math.floor(nextProps.temps.data.length/defaultNumberOfPoints)});
		}
	} 

	renderPlot = () => {
		const data = this.props.temps.data.map( currentItem => {
			return { ...currentItem.temps, time: currentItem.time };
		});
		//TODO(suyashkumar): explore more efficient ways to do this (caching) instead of calculating on every render
		const dataToShow = this.filterDatesToShow(this.downsampleArray(data, this.state.downsampleFactor)).reverse();
		const tickInterval = Math.floor(dataToShow.length / 5);
		return (
			<ResponsiveContainer width="94%" height={300}>
				<LineChart data={dataToShow}>
					<XAxis dataKey="time" label="Date" interval={tickInterval} tickFormatter={this.formatDate} />
					<YAxis />
					{
						// Draw a line for each temperature probe:
						Object.keys(data[0]).map((currentItem, index) => {
							if (currentItem === 'time') return null;
							return (
								<Line 
									type="monotone"
									dataKey={currentItem} 
									key={currentItem} 
									animationDuration={500}
									stroke={LINE_COLORS[index]}/>
							);
						})
					}
					<Tooltip labelFormatter={this.formatDate}/>
					<Legend />
					<CartesianGrid strokeDasharray="3 3" />
				</LineChart> 
			</ResponsiveContainer>
		); 
	}

	renderDaysToShow = () => {
		return ( 
			<div>
			<p> Days to show </p>
			<Slider 
				min={1} 
				max={this.state.daysToFetch} 
				step={1}
				pinned
				snaps
				editable 
				value={this.state.daysToShow} 
				onChange={this.handleSliderChange.bind(this, 'daysToShow')}/> 
			</div>
		); 
	}

	renderView = () => {
		return ( 
			<div>
			<Layout>
					<Panel>
							<div style={{textAlign: 'center'}}>
								Temperature (past {this.state.daysToShow} days, {this.props.currentLocation})
							</div>
							{this.renderPlot()}
							<div className="sidebar-content">	
								{this.renderDaysToShow()}
							</div>
					</Panel>

					<Sidebar pinned={this.props.sideBarOpen} width={6}>
						<div className="sidebar-content">
							<h4> Plot Options </h4>
							<p> Downsample Sampling Scale Factor </p>	
							<Slider 
								min={1} 
								max={50} 
								step={5}
								pinned
								snaps
								editable 
								value={this.state.downsampleFactor} 
								onChange={this.handleSliderChange.bind(this, 'downsampleFactor')}/>
							{this.renderDaysToShow()}
							<p> Days to fetch </p>	
							<div>
								<Input 
									type='text' 
									className="sidebar-input"
									value={this.state.daysToFetch} 
									onChange={this.handleSliderChange.bind(this, 'daysToFetch')} />
								<Button 
									style={{display: 'inline-block'}} 
									icon="cached"
									primary
									onClick={this.handleDaysToFetchChange}>
									Fetch
								</Button>
							</div> 
						</div>
					</Sidebar>
			</Layout>
			</div>
		);
	}

	render() { 
		const dataExists = !this.props.temps.loading && this.props.temps.data.length > 0;
		return (
			<div>
				{
					dataExists &&	
					!this.props.temps.loading &&
					this.renderView()	
				}
				{
					!dataExists &&
					!this.props.temps.loading &&
					<div style={{textAlign: 'center'}}>
						No Data for the past {this.state.daysToFetch} days. Increase days to fetch in the plot menu (to the upper right).
					</div>
				}
			</div>
		);
	} 
}

export default TempPlot;
