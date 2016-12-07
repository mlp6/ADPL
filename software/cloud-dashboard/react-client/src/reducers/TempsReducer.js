import { FETCH_TEMPS } from '../actions/index';
export default function(state = [], action) {
	switch(action.type) {
		case FETCH_TEMPS:
			return [action.payload.data, ...state]; 
	}
	return state;
}
