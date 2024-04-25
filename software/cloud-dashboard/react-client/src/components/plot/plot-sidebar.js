import React from 'react';
import DaysToShowSlider from './days-to-show-slider';
import {Slider, Sidebar, Input} from 'react-toolbox';
import { Button } from 'react-toolbox/lib/button';

const styles = {
    sidebarInput: {
        maxWidth: '50%',
        display: 'inline-block',
    },
    sidebarContent: {
        margin: '10px',
    }
};

const PlotSidebar = props => {
    return (
        <div>
        <Sidebar pinned={props.sideBarOpen} width={6}>
            <div style={styles.sidebarContent}>
                <h4> Plot Options </h4>
                <p> Downsample Sampling Scale Factor </p>
                <Slider
                    min={1}
                    max={50}
                    step={5}
                    pinned
                    snaps
                    editable
                    value={props.downsampleFactor}
                    onChange={props.onDownsampleChange}/>
                <DaysToShowSlider
                    daysToFetch={props.daysToFetch}
                    daysToShow={props.daysToShow}
                    onDaysToShowChange={props.onDaysToShowChange} />
                <p> Days to fetch </p>
                <div>
                    <Input
                        type='text'
                        style={styles.sidebarInput}
                        value={props.daysToFetch}
                        onChange={props.onDaysToFetchChange} />
                    <Button
                        style={{display: 'inline-block'}}
                        icon="cached"
                        primary
                        onClick={props.handleDaysToFetchChange}>
                        Fetch
                    </Button>
                </div>
            </div>
        </Sidebar>
        </div>
    )
};

export default PlotSidebar;
