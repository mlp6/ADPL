import React, { Component } from 'react';
import DaysToShowSlider from 'days-to-show-slider';
import Slider from 'react-toolbox/lib/slider';
import { Button } from 'react-toolbox/lib/button';

const PlotSidebar = props => {
    return (
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
                    value={this.props.downsampleFactor}
                    onChange={this.props.onDownsampleChange}/>
                <DaysToShowSlider />
                <p> Days to fetch </p>
                <div>
                    <Input
                        type='text'
                        className="sidebar-input"
                        value={this.props.daysToFetch}
                        onChange={this.props.onDaysToFetchChange} />
                    <Button
                        style={{display: 'inline-block'}}
                        icon="cached"
                        primary
                        onClick={this.props.handleDaysToFetchChange}>
                        Fetch
                    </Button>
                </div>
            </div>
        </Sidebar>
    )
};

export default PlotSidebar;