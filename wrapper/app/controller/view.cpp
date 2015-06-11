#include "view.hpp"

/*
	Sets default values for a session -> Non-authenticated user on Home page
*/
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

/*
	@FUNCTION - Destructor, does nothing
*/
dxtemplate::context::~context() {
	return;
}

/*
	@FUNCTION - Sets context to match specified page
*/
void dxtemplate::context::set_page(std::string page_id) {
	PAGE = page_id;
	if(PAGE == "HOME") {
		TITLE = "Home";
	}
	if(PAGE == "REGISTER") {
		TITLE = "Register";
	}
	if(PAGE == "CREATEPOST") {
		TITLE = "Create Post";
	}
	if(PAGE == "FORGOT_INITIAL") {
		TITLE = "Forgot Something";
	}
	if(PAGE == "FORGOT_LANDING") {
		TITLE = "Request Sent";
	}
	if(PAGE == "PASSWORD_RESET") {
		TITLE = "Password Reset";
	}
	if(PAGE == "RESET_LANDING") {
		TITLE = "Password reset";
	}
	return;
}

/*
	@FUNCTION - Retreives and returns title of current page
*/
std::string dxtemplate::context::get_title() {
	return TITLE;
}

/*
	@FUNCTION - Checks if the user is currently logged in
*/
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

/*
	@FUNCTION -
		Determine if session LAST_ACTIVITY has exceeded max timeout limit ( 3600 seconds | 1 hour )
*/
bool dxtemplate::context::eval_logged_in() {
	return false;
}

/*
	@FUNCTION - Retrieves session values and places them in context for page rendering
*/
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