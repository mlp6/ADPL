import { SET_DAYS_TO_FETCH } from '../actions/index';

export default (state={}, action) => {
	switch(action.type) {
		case SET_DAYS_TO_FETCH:
			return Object.assign({}, state, {daysToFetch: action.daysToFetch});

	}
	return state;
}
