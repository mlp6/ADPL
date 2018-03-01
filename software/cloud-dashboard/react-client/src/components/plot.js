import React, { Component } from 'react';
import {Button, IconButton} from 'react-toolbox/lib/button';
import {LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer} from 'recharts'
import { Layout, NavDrawer, Panel, Sidebar } from 'react-toolbox';
import Slider from 'react-toolbox/lib/slider';
import constants from '../../constants';
import './plot/plot-sidebar';
import DaysToShowSlider from './plot/days-to-show-slider';
const LINE_COLORS = ['#e41a1c', '#377eb8', '#4daf4a', '#984ea3', '#ff7f00', '#ffff33']

const defaultNumberOfPoints = 300;

var styles = {
    sidebarInput: {
        maxWidth: '50%',
        display: 'inline-block',
    },
    sidebarContent: {
        margin: '10px',
    }
};

class Plot extends Component {

    state = {
        downsampleFactor: 15,
        daysToFetch: constants.daysToFetch,
        daysToShow: constants.daysToFetch,
    };

    downsampleArray = (array, factor) => {
        const downsampledArray = [];
        for (let i = 0; i < array.length; i=i+factor) {
            downsampledArray.push(array[i]);
        }
        return downsampledArray;
    };

    filterDatesToShow = (array) => {
        const cutoffDate = new Date();
        cutoffDate.setDate(cutoffDate.getDate() - this.state.daysToShow);

        array = array.filter(value => {
            const date = new Date(value.time);
            return date > cutoffDate;
        });

        return array;
    };

    formatDate = date => {
        const dateObj = new Date(date);
        const getMinutes = minuteString => (minuteString.length > 1) ? minuteString : "0" + minuteString;

        return `${dateObj.getMonth()+1}/${dateObj.getDate()} ${dateObj.getHours()}:${getMinutes(dateObj.getMinutes().toString())}`
    };

    handleSliderChange = (slider, value) => {
        const newSliderState = {};
        newSliderState[slider] = value;
        this.setState(newSliderState);
    };

    handleDaysToFetchChange = () => {
        this.props.fetchTemps(this.props.currentLocation, this.state.daysToFetch);
        this.props.setDaysToFetch(this.state.daysToFetch);
        this.setState({daysToShow: this.state.daysToFetch});
        this.props.toggleSidebar();
    };

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
    };

    renderView = () => {
        return (
            <div>
                <Layout>
                    <Panel>
                        <div style={{textAlign: 'center'}}>
                            Temperature (past {this.state.daysToShow} days, {this.props.currentLocation})
                        </div>
                        {this.renderPlot()}
                        <div style={styles.sidebarContent}>
                            <DaysToShowSlider
                                daysToFetch={this.state.daysToFetch}
                                daysToShow={this.state.daysToShow}
                                onDaysToShowChange={val => this.handleSliderChange('daysToShow', val)} />
                        </div>
                    </Panel>

                    <PlotSidebar
                        sideBarOpen={this.props.sideBarOpen}
                        downsampleFactor={this.state.downsampleFactor}
                        daysToFetch={this.state.daysToFetch}
                        onDownsampleChange={val => this.handleSliderChange('downsampleFactor', val)}
                        onDaysToFetchChange={val => this.handleSliderChange('daysToFetch', val)}
                        handleDaysToFetchChange={this.handleDaysToFetchChange} />

                </Layout>
            </div>
        );
    };

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

export default Plot;
