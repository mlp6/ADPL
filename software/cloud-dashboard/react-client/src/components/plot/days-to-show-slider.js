import React from 'react';
import Slider from 'react-toolbox/lib/slider';

const DaysToShowSlider = props => {
    return (
        <div>
            <p> Days to show </p>
            <Slider
                min={1}
                max={props.daysToFetch}
                step={1}
                pinned
                snaps
                editable
                value={props.daysToShow}
                onChange={props.onDaysToShowChange} />
        </div>
    )
};

export default DaysToShowSlider;