import axios from 'axios';

export const FETCH_TEMPS = 'FETCH_TEMPS';
export const FETCH_LOCATIONS = 'FETCH_LOCATIONS';

export function fetchTemps(location) {
	return {
		type: FETCH_TEMPS,
		payload: axios.get(`http://adpl.suyash.io/api/list/${location}`)
	}
}

export function fetchLocations() { 
	return {
		type: FETCH_LOCATIONS,
		payload: axios.get('http://adpl.suyash.io/api/sites')
	}
}
