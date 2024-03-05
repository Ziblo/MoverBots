// Script assets have changed for v2.3.0 see
// https://help.yoyogames.com/hc/en-us/articles/360005277377 for more information
function init_active_bots(){
	active_bots = {
		list: ds_list_create(),
		has_lifter: false,
	};
}
function destroy_active_bots(){
	ds_list_destroy(active_bots.list);
	active_bots = undefined;
}

function active_bots_len(){
	return ds_list_size(active_bots.list);
}

function active_bots_has_lifter(){
	return active_bots.has_lifter;
}

function active_bots_empty(){
	return ds_list_empty(active_bots.list);
}

function active_bots_add(_event_parameters){
	//from the ACTIVATE_BOT_X event
	ds_list_add(active_bots.list, _event_parameters);
}

function active_bots_remove(_event_parameters){
	//from the ACTIVATE_BOT_X event
	ds_list_delete(active_bots.list, _event_parameters);
}