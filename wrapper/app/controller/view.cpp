#include "view.hpp"


// constuctor -> Sets default values for a session
dxtemplate::context::context() {
	TITLE = "Home";
	
	PAGE = "HOME";

	LOGGED_IN = "false";
	LAST_ACTIVITY = 0;
	USERNAME = "";
	EMAIL = "";
	USER_ID = "";
	return;
}

dxtemplate::context::~context() {
	return;
}

void dxtemplate::context::set_page(std::string page_id) {
	PAGE = page_id;
	if(PAGE.compare("HOME") == 0) {
		TITLE = "Home";
	}
	if(PAGE.compare("REGISTER") == 0) {
		TITLE = "Register";
	}
	if(PAGE.compare("CREATEPOST") == 0) {
		TITLE = "Create Post";
	}
	return;
}

std::string dxtemplate::context::get_title() {
	return TITLE;
}

bool dxtemplate::context::is_logged_in(cppcms::session_interface &interface) {
	// check if session is set
	if(!interface.is_set("LOGGED_IN")) {
		interface.set("LOGGED_IN", "false");
		interface.set("USERNAME", "");
		interface.set("EMAIL", "");
		interface.set("USER_ID", "");
		interface.set("LAST_ACTIVITY", 0);
		interface.save();
		return false;
	}

	// check if true/false
	if(interface.get("LOGGED_IN") == "true") {
		return true;
	} else {
		return false;
	}
	return false;
}

// check if last_activity is > 3600
bool dxtemplate::context::eval_logged_in() {
	return false;
}

void dxtemplate::context::resolve_session(cppcms::session_interface &interface) {
	// if logged in
	if(is_logged_in(interface)) {
		// check if timeout
		LOGGED_IN = "true";
		USERNAME = interface.get("USERNAME");
		EMAIL = interface.get("EMAIL");
		USER_ID = interface.get("USER_ID");
		LAST_ACTIVITY = interface.get<int>("LAST_ACTIVITY");
	} else { // not logged set empty context variables
		LOGGED_IN = "false";
		USERNAME = "";
		EMAIL = "";
		USER_ID = "";
		LAST_ACTIVITY = 0;
	}
	return;
}