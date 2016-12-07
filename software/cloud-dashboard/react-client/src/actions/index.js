import axios from 'axios';

export const FETCH_TEMPS = 'FETCH_TEMPS';

export function fetchTemps() {
	return {
		type: FETCH_TEMPS,
		payload: axios.get('http://adpl.suyash.io/api/list/ADPLKenyaC6304')
	}
}
