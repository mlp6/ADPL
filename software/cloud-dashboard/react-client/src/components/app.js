import React, { Component } from 'react'; 
import AppBar from 'react-toolbox/lib/app_bar'; 
import 'react-toolbox/lib/commons.scss'; 
import Test from '../containers/Test';
import Dashboard from '../containers/dashboard'; 

class App extends Component {
  render() {
    return (
      <div className="App">
	  	<AppBar title="ADPL Dashboard"/> 
		<Dashboard />
      </div>
    );
  }
}

export default App;
