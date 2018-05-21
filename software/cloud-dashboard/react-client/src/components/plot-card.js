import React, { Component, PropTypes } from 'react';
import { Card, CardTitle, CardText } from 'react-toolbox/lib/card';
import {IconButton} from 'react-toolbox/lib/button';
import LoadingView from './util/loading-view';
import Plot from './plot';


class PlotCard extends Component {

    state = {
        plotSidebarOpen: false,
    };

    togglePlotSidebar = () => {
        this.setState({plotSidebarOpen: !this.state.plotSidebarOpen});
    };

    render() {
       return (
           <Card className="card" style={{minHeight: '400px'}}>
               <div style={{margin: '20px'}}>
                   <h4 style={{display: 'inline-block', float: 'left'}}> {this.props.title} </h4>
                   <IconButton
                       className="material-icons"
                       style={{float: 'right'}}
                       icon='menu'
                       onClick={this.togglePlotSidebar}/>
               </div>
               {
                   this.props.isLoading &&
                   <LoadingView />
               }

               <Plot
                   data={this.props.data}
                   yAxisMinMax={this.props.yAxisMinMax}
                   isLoading={this.props.isLoading}
                   fetchNewData={this.props.fetchNewData}
                   currentLocation={this.props.currentLocation}
                   toggleSidebar={this.togglePlotSidebar}
                   setDaysToFetch={this.props.setDaysToFetch}
                   sideBarOpen={this.state.plotSidebarOpen}
                   title={this.props.plotTitle} />
           </Card>
       )
    }
}

PlotCard.propTypes = {
    data: PropTypes.array.isRequired,
    yAxisMinMax: PropTypes.array.isRequired,
    isLoading: PropTypes.bool.isRequired,
    fetchNewData: PropTypes.func.isRequired,
    currentLocation: PropTypes.string.isRequired,
    setDaysToFetch: PropTypes.func.isRequired,
    title: PropTypes.string.isRequired,
    plotTitle: PropTypes.string.isRequired,
};

export default PlotCard;
