// Script assets have changed for v2.3.0 see
// Game state machine
function controller_state_machine(_event, _event_parameters){
	var _next_state = undefined;
	static controller_state = CONTROLLER_STATES.STANDBY;
	switch (controller_state){
		case CONTROLLER_STATES.STANDBY:
			_next_state = controller_standby_state(_event, _event_parameters);
			break;
		case CONTROLLER_STATES.PAIRING:
			_next_state = controller_pairing_state(_event, _event_parameters);
			break;
		case CONTROLLER_STATES.NAVIGATION:
			_next_state = controller_navigation_state(_event, _event_parameters);
			break;
	}
	if (_next_state != undefined){
		controller_state_machine(INTERNAL_EVENTS.EXIT, 0);//exit current state
		controller_state = _next_state;
		controller_state_machine(INTERNAL_EVENTS.ENTRY, 0);//enter the next state
	}
}

function controller_standby_state(_event, _event_parameters){
	var _next_state = undefined;
	switch(_event){
		// INTERNAL EVENTS //
		case INTERNAL_EVENTS.ENTRY:
			//destroy selected bots list
			ds_list_destroy(selected_bots);
			break;
		case INTERNAL_EVENTS.EXIT:
			break;
		// UI EVENTS //
		case UI_EVENTS.POWER_BTN:
			_next_state = CONTROLLER_STATES.PAIRING;
			break;
		// SENSOR EVENTS //
	}
	return _next_state;
}

function controller_pairing_state(_event, _event_parameters){
	var _next_state = undefined;
	switch(_event){
		// INTERNAL EVENTS //
		case INTERNAL_EVENTS.ENTRY:
			//init selected bots list
			selected_bots = ds_list_create();
			//init found bots list
			found_bots = ds_list_create();
			///send first ping?
			///start ping timer
			break;
		case INTERNAL_EVENTS.TIMEOUT:
			///time to send another ping?
			///or instead go to timeout state?
			break;
		case INTERNAL_EVENTS.EXIT:
			//destroy found_bots
			ds_list_destroy(found_bots);
			///save selected_bots to some global thing
			break;
		// UI EVENTS //
		case UI_EVENTS.TOGGLE_BOT_X_SELECTION:
			var _index = ds_list_find_index(selected_bots, _event_parameters);
			if (_index == -1){
				//bot is not already selected
				ds_list_add(selected_bots, _event_parameters);
			}else{
				//bot already selected
				ds_list_delete(selected_bots, _index);
			}
			break;
		case UI_EVENTS.READY:
			if (ds_list_size(selected_bots) > 0){
				_next_state = CONTROLLER_STATES.NAVIGATION;
			}else{
				show_debug_message("PAIRING state: READY event without any selected bots");
			}
			break;
		// SENSOR EVENTS //
		case SENSOR_EVENTS.FOUND_DEVICE:
			ds_list_add(found_bots, _event_parameters);
			break;
	}
	return _next_state;
}

function controller_navigation_state(_event, _event_parameters){
	var _next_state = undefined;
	//navigation sub state gets events first so it can eat events!
	_event = navigation_sub_state_machine(_event, _event_parameters);
	switch(_event){ //these are the default responses to events in navigation state
		// INTERNAL EVENTS //
		case INTERNAL_EVENTS.ENTRY:
			//init active bots data structure
			init_active_bots();
			break;
		case INTERNAL_EVENTS.EXIT:
			//destroy active bots data structure
			destroy_active_bots();
			break;
		case INTERNAL_EVENTS.LOST_CONNECTION:
			_next_state = CONTROLLER_STATES.BOT_STUCK;
			break;
		// UI EVENTS //
		case UI_EVENTS.PAIRING:
			//switch back to pairing mode again
			_next_state = CONTROLLER_STATES.PAIRING;
			break;
		case UI_EVENTS.POWER_BTN:
			_next_state = CONTROLLER_STATES.STANDBY;
			break;
		// SENSOR EVENTS //
		case SENSOR_EVENTS.BOT_STUCK:
			_next_state = CONTROLLER_STATES.BOT_STUCK;
			break;
		case SENSOR_EVENTS.NO_CAPTOUCH:
			_next_state = CONTROLLER_STATES.DROPPED;
			break;
		case SENSOR_EVENTS.LOW_BATTERY:
			_next_state = CONTROLLER_STATES.DISCONNECT;
			break;
		case SENSOR_EVENTS.JOYSTICK_UPDATE:
			///do something with_event_parameters to control the motors of all active bots in sync
			break;
		case UI_EVENTS.ACTIVATE_BOT_X:
			//add this bot to the active bots data structure
			active_bots_add(_event_parameters);
			break;
		case UI_EVENTS.DEACTIVATE_BOT_X:
			//add this bot to the active bots data structure
			active_bots_remove(_event_parameters);
			break;
	}
	return _next_state;
}

function navigation_sub_state_machine(_event, _event_parameters){
	var _next_sub_state = undefined;
	//sub state machines are allowed to modify events (for example eating one)
	var _substate_param;
	static navigation_sub_state = NAVIGATION_SUB_STATES.CONTROLLING;
	switch (navigation_sub_state){
		case NAVIGATION_SUB_STATES.CONTROLLING:
			_substate_param = navigation_controlling_sub_state(_event, _event_parameters);
			_next_sub_state = _substate_param.p_next_state;
			_event = _substate_param.p_event;
			break;
		case NAVIGATION_SUB_STATES.LIFTING:
			_substate_param = navigation_lifting_sub_state(_event, _event_parameters);
			_next_sub_state = _substate_param.p_next_state;
			_event = _substate_param.p_event;
			break;
		case NAVIGATION_SUB_STATES.ATTACHING:
			_substate_param = navigation_attaching_sub_state(_event, _event_parameters);
			_next_sub_state = _substate_param.p_next_state;
			_event = _substate_param.p_event;
			break;
		case NAVIGATION_SUB_STATES.SAFETY:
			_substate_param = navigation_safety_sub_state(_event, _event_parameters);
			_next_sub_state = _substate_param.p_next_state;
			_event = _substate_param.p_event;
			break;
	}
	if (_next_sub_state != undefined){
		navigation_sub_state_machine(INTERNAL_EVENTS.EXIT, 0);//exit current state
		navigation_sub_state = _next_sub_state;
		navigation_sub_state_machine(INTERNAL_EVENTS.ENTRY, 0);//enter the next state
	}
	return _event;
}

function navigation_controlling_sub_state(_event, _event_parameters){
	var _next_sub_state = undefined;
	//This one has different UI depending on the global data structure active_bots.
	//Therefore it needs to update the UI flag for Kim to use.
	if ( active_bots_empty() ){
		ui_state = UI_FLAG.NO_ACTIVE;
	}else{
		//One or more bots are active.
		if ( _event == UI_EVENTS.LIFT_MODE and active_bots_has_lifter() ){
			_next_sub_state = NAVIGATION_SUB_STATES.LIFTING;
			_event = NO_EVENT; //eat event!
		}
		if ( active_bots_len() == 1 ){
			ui_state = UI_FLAG.SINGLE_BOT_CONTROL;
			if ( _event == UI_EVENTS.CONNECT_LIFTER ){
				//this is here because it should only happen if there is one bot selected
				///turn on electromagnets of the lifter and bot in question
				_next_sub_state = NAVIGATION_SUB_STATES.ATTACHING;
				_event = NO_EVENT; //eat event!
			}
		}else{
			//More than one bot is active.
			ui_state = UI_FLAG.SOME_ACTIVE;
		}
	}
	//These are the default responses to events in the controlling sub state
	switch (_event){
		//Most if not all of these are probably going to be the default navigation event responses or already handled by the cases above, so not much to do here.
		//Any event responses here would apply to NoActive, SingleBotControl, and SomeActive.
	}
	return {p_next_state: _next_sub_state, p_event: _event};
}

function navigation_lifting_sub_state(_event, _event_parameters){
	var _next_sub_state = undefined;
	ui_state = UI_FLAG.LIFT_MODE;
	switch (_event){
		///joystick will controll lifter
	}
	return {p_next_state: _next_sub_state, p_event: _event};
}

function navigation_attaching_sub_state(_event, _event_parameters){
	var _next_sub_state = undefined;
	ui_state = UI_FLAG.CONNECT_LIFTER_MODE;
	switch (_event){
	}
	return {p_next_state: _next_sub_state, p_event: _event};
}

function navigation_safety_sub_state(_event, _event_parameters){
	//I might not use this.
	var _next_sub_state = undefined;
	switch (_event){
	}
	return {p_next_state: _next_sub_state, p_event: _event};
}