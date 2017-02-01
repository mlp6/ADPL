import React, { Component, PropTypes } from 'react';
import notesData from './notes/notes-data';
import './notes/notes.css';

const Notes = props => { 
	return (
		<div>
			<p>{notesData[props.currentLocation]}</p>
		</div>
	);
};

export default Notes;
