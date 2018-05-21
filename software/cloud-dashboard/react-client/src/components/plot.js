import React, { Component } from 'react';
import {Button, IconButton} from 'react-toolbox/lib/button';
import {LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer} from 'recharts'
import { Layout, NavDrawer, Panel, Sidebar } from 'react-toolbox';
import constants from '../constants';
import './plot/plot-sidebar';
import DaysToShowSlider from './plot/days-to-show-slider';
import PlotSidebar from './plot/plot-sidebar';
const LINE_COLORS = ['#e41a1c', '#377eb8', '#4daf4a', '#984ea3', '#ff7f00', '#ffff33'];

const defaultNumberOfPoints = 100;

const styles = {
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
        downsampleFactor: 1,
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
        this.props.fetchNewData(this.props.currentLocation, this.state.daysToFetch);
        this.props.setDaysToFetch(this.state.daysToFetch);
        this.setState({daysToShow: this.state.daysToFetch});
        this.props.toggleSidebar();
    };

    componentWillReceiveProps(nextProps) {
        if (nextProps.data.length > defaultNumberOfPoints &&
            (this.props.data && (nextProps.data.length !== this.props.data.length))) {
            this.setState({downsampleFactor: Math.floor(nextProps.data.length/defaultNumberOfPoints)});
        }
    }

    componentWillMount() {
        if (this.props.data.length > defaultNumberOfPoints) {
            // Reset downsampling factor to show the default number of points on the graph
            this.setState({downsampleFactor: Math.floor(this.props.data.length/defaultNumberOfPoints)})
        }
    }


    renderPlot = () => {

        //TODO(suyashkumar): explore more efficient ways to do this (caching) instead of calculating on every render
        const dataToShow = this.filterDatesToShow(this.downsampleArray(this.props.data, this.state.downsampleFactor));
        const tickInterval = Math.floor(dataToShow.length / 5);
        return (
            <ResponsiveContainer width="94%" height={300}>
                <LineChart data={dataToShow}>
                    <XAxis dataKey="time" label="Date" interval={tickInterval} tickFormatter={this.formatDate} />
                    <YAxis type="number" domain={[this.props.yAxisMinMax[0], this.props.yAxisMinMax[1]]} allowDataOverflow={true} />
                    {
                        // Draw a line for each key
                        Object.keys(this.props.data[0]).map((currentItem, index) => {
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
                            {this.props.title}
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
                        daysToShow={this.state.daysToShow}
                        onDownsampleChange={val => this.handleSliderChange('downsampleFactor', val)}
                        onDaysToFetchChange={val => this.handleSliderChange('daysToFetch', val)}
                        onDaysToShowChange={val => this.handleSliderChange('daysToShow', val)}
                        handleDaysToFetchChange={this.handleDaysToFetchChange} />


                </Layout>
            </div>
        );
    };

    render() {
        const dataExists = !this.props.isLoading && this.props.data && this.props.data.length > 0;
        return (
            <div>
                {
                    dataExists &&
                    !this.props.isLoading &&
                    this.renderView()
                }
                {
                    !dataExists &&
                    !this.props.isLoading &&
                    <div style={{textAlign: 'center'}}>
                        No Data for the past {this.state.daysToFetch} days. Increase days to fetch in the plot menu (to the upper right).
                    </div>
                }
            </div>
        );
    }
}

export default Plot;
