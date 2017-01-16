import { combineReducers } from 'redux';
import TempReducer from './TempsReducer';
import LocationsReducer from './locations-reducer';
import MetaReducer from './meta-reducer';

const rootReducer = combineReducers({
	temps: TempReducer,
	locationData: LocationsReducer,
	meta: MetaReducer
});

export default rootReducer;
