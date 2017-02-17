export function handlePromiseWithData(action, state) {
	if (action.promiseLoading) {
		return Object.assign({}, state, {loading: true});
	} else {
		return Object.assign({}, state, {data: action.payload.data, loading: false});
	}

}
