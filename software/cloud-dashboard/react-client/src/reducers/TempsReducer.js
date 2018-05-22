import { FETCH_TEMPS } from '../actions/index';
import { handlePromiseWithData } from './util';
export default function(state = {}, action) {
	switch(action.type) {
		case FETCH_TEMPS: 
			return handlePromiseWithData(action, state); 

    // no default
	}
	return state;
}
