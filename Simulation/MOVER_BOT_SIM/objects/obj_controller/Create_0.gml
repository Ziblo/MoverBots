/// @description Define enums

#region enums
//STATES
enum CONTROLLER_STATES{
	STANDBY,
	PAIRING,
	PAIRING_TIMEOUT,
	NAVIGATION,
	DROPPED,
	DISCONNECT,
	BOT_STUCK,
}
enum NAVIGATION_SUB_STATES{
	CONTROLLING,
	ATTACHING,
	LIFTING,
	SAFETY,
}
enum UI_FLAG{
	NO_ACTIVE,
	SINGLE_BOT_CONTROL,
	CONNECT_LIFTER_MODE,
	SOME_ACTIVE,
	LIFT_MODE,
}
//EVENTS
enum INTERNAL_EVENTS{
	ENTRY,
	EXIT,
	STEP,
	TIMEOUT,
	LOST_CONNECTION,
	NO_EVENT,
}
enum UI_EVENTS{
	POWER_BTN,
	TOGGLE_BOT_X_SELECTION,
	RETRY,
	READY,
	PAIRING,
	LIFT_MODE,
	CONNECT_LIFTER,
	ACTIVATE_BOT_X,
	DEACTIVATE_BOT_X,
	OK,
	CANCEL,
}
enum SENSOR_EVENTS{
	FOUND_DEVICE,
	CAPTOUCH,
	NO_CAPTOUCH,
	LIFTER_CONNECTED,
	LOW_BATTERY,
	BOT_STUCK,
	JOYSTICK_UPDATE,
}
#endregion