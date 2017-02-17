import { combineReducers } from 'redux';
import BucketReducer from './bucket-reducer';
import TempReducer from './TempsReducer';
import LocationsReducer from './locations-reducer';
import MetaReducer from './meta-reducer';

const rootReducer = combineReducers({
	bucket: BucketReducer,
	temps: TempReducer,
	locationData: LocationsReducer,
	meta: MetaReducer
});

export default rootReducer;
