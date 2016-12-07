import { combineReducers } from 'redux';
import TempReducer from './TempsReducer';

const rootReducer = combineReducers({
	temps: TempReducer
});

export default rootReducer;
