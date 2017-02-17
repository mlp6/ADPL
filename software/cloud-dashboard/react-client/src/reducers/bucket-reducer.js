import { FETCH_BUCKET_TIPS } from '../actions/index';
import { handlePromiseWithData } from './util';
export default function(state = {data: []}, action) {
	switch(action.type) {
		case FETCH_BUCKET_TIPS:
			return handlePromiseWithData(action, state);
	}
	return state;
}
