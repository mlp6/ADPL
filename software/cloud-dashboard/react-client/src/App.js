import React, { Component } from 'react';
import logo from './logo.svg';
import AppBar from 'react-toolbox/lib/app_bar';
import Input from 'react-toolbox/lib/input';

import Test from './containers/Test';
import {Button, IconButton} from 'react-toolbox/lib/button';

class App extends Component {
  render() {
    return (
      <div className="App">
	  	<AppBar title="react">

		</AppBar>

		<Test />
		<section>
		<Button label='add' raised primary/>
		<Input type='text' label='none' />
		</section>
      </div>
    );
  }
}

export default App;
