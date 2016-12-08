import { FETCH_LOCATIONS } from '../actions/index';

export default (state=[], action) => {
	switch(action.type) {
		case FETCH_LOCATIONS:
			return [action.payload.data];
	}
	return state;
}
