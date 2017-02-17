import axios from 'axios';

export const FETCH_TEMPS = 'FETCH_TEMPS';
export const FETCH_LOCATIONS = 'FETCH_LOCATIONS';
export const SELECT_LOCATION = 'SELECT_LOCATION';
export const SET_DAYS_TO_FETCH = 'SET_DAYS_TO_FETCH';
export const FETCH_BUCKET_TIPS = 'FETCH_BUCKET_TIPS';

const serverUrl = 'http://adpl.suyash.io'; 
//const serverUrl = 'http://localhost:9000';

export function fetchTemps(location, numDays) {
	return {
		type: FETCH_TEMPS,
		payload: axios.get(`${serverUrl}/api/list/${location}/${numDays}`)
	}
}

export function fetchLocations() { 
	return {
		type: FETCH_LOCATIONS,
		payload: axios.get(`${serverUrl}/api/sites`)
	}
}

export function selectLocation(currentLocation) {
	return {
		type: SELECT_LOCATION,
		currentLocation
	}
}

export function setDaysToFetch(daysToFetch) {
	return {
		type: SET_DAYS_TO_FETCH,
		daysToFetch
	}
}

export function fetchBucketTips(location, numDays) {
	return {
		type: FETCH_BUCKET_TIPS,
		payload: axios.get(`${serverUrl}/api/bucket/list/${location}/${numDays}`)
	}
}
