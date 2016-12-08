import React, { Component } from 'react'; 
import AppBar from 'react-toolbox/lib/app_bar';
import Input from 'react-toolbox/lib/input';
import 'react-toolbox/lib/commons.scss'; 

import Test from '../containers/Test';
import {Button, IconButton} from 'react-toolbox/lib/button';

class App extends Component {
  render() {
    return (
      <div className="App">
	  	<AppBar title="ADPL Dashboard"> 
		</AppBar>

		<Test /> 
      </div>
    );
  }
}

export default App;
