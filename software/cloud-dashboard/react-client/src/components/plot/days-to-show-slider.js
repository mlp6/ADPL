import React from 'react';

const DaysToShowSlider = props => {
    return (
        <div>
            <p> Days to show </p>
            <Slider
                min={1}
                max={this.props.daysToFetch}
                step={1}
                pinned
                snaps
                editable
                value={this.props.daysToShow}
                onChange={this.props.onDaysToShowChange} />
        </div>
    )
};

export default DaysToShowSlider;