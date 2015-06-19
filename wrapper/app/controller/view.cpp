#include "view.hpp"

// @FUNCTION - Sets context to match specified page
void Pages::set_page(Context *context, std::string page_id) {
	context->PAGE = page_id;
	if(context->PAGE == "HOME") {
		context->TITLE = "Home";
	}
	if(context->PAGE == "REGISTER") {
		context->TITLE = "Register";
	}
	if(context->PAGE == "CREATEPOST") {
		context->TITLE = "Create Post";
	}
	if(context->PAGE == "FORGOT_INITIAL") {
		context->TITLE = "Forgot Something";
	}
	if(context->PAGE == "FORGOT_SUCCESS") {
		context->TITLE = "Request Sent";
	}
	if(context->PAGE == "PASSWORD_RESET") {
		context->TITLE = "Password Reset";
	}
	if(context->PAGE == "RESET_SUCCESS") {
		context->TITLE = "Password reset";
	}
	if(context->PAGE == "RESETERROR") {
		context->TITLE = "Reset Error";
	}
	if(context->PAGE == "ACCOUNT_MAIN") {
		context->TITLE = "Account Settings";
	}
	if(context->PAGE == "ACCOUNT_FILTERS") {
		context->TITLE = "Blocked Users";
	}
	return;
}

// @FUNCTION - Checks if the user is currently logged in
bool Pages::logged_in(cppcms::session_interface &interface) {
	// check if session is set
	if(!interface.is_set("LOGGED_IN")) {
		interface.set("LOGGED_IN", "false");
		interface.set("USERNAME", "");
		interface.set("EMAIL", "");
		interface.set("SECONDARY_EMAIL", "");
		interface.set("USER_ID", "");
		interface.set("SETTINGS", "false");
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

// @FUNCTION - Retrieves session values and places them in context for page rendering
bool Pages::resolve_session(Context *context, cppcms::session_interface &interface) {
	// if logged in
	if(logged_in(interface)) {
		// check if timeout
		context->LOGGED_IN = "true";
		try {
			context->USERNAME = interface.get("USERNAME");
		} catch(std::exception &e) {
			std::cerr << "[-] Error resolving session => " << std::endl << e.what() << std::endl << "\t=> Setting to default" << std::endl;
			interface.set("USERNAME", "");
			context->USERNAME = "";
		}

		try {
			context->EMAIL = interface.get("EMAIL");
		} catch(std::exception &e) {
			std::cerr << "[-] Error resolving session => " << std::endl << e.what() << std::endl << "\t=> Setting to default" << std::endl;
			interface.set("EMAIL", "");
			context->EMAIL = "";
		}

		try {
			context->SECONDARY_EMAIL = interface.get("SECONDARY_EMAIL");
		} catch(std::exception &e) {
			std::cerr << "[-] Error resolving session => " << std::endl << e.what() << std::endl << "\t=> Setting to default" << std::endl;
			interface.set("SECONDARY_EMAIL", "");
			context->SECONDARY_EMAIL = "";
		}

		try {
			context->USER_ID = interface.get("USER_ID");
		} catch(std::exception &e) {
			std::cerr << "[-] Error resolving session => " << std::endl << e.what() << std::endl << "\t=> Setting to default" << std::endl;
			interface.set("USER_ID", "");
			context->USER_ID = "";
		}

		if(context->PAGE == "ACCOUNT_MAIN") {
			try {
				context->SETTINGS = interface.get("SETTINGS");
			} catch(std::exception &e) {
				std::cerr << "[-] Error resolving session => " << std::endl << e.what() << std::endl << "\t=> Setting to default" << std::endl;
				interface.set("SETTINGS", "false");
				context->SETTINGS = "false";
			}
		} else {
			context->SETTINGS = "false";
			interface.set("SETTINGS", "false");
		}

		try {
			context->LAST_ACTIVITY = interface.get<int>("LAST_ACTIVITY");
		} catch(std::exception &e) {
			std::cerr << "[-] Error resolving session => " << std::endl << e.what() << std::endl << "\t=> Setting to default" << std::endl;
			interface.set("LAST_ACTIVITY", "0");
			context->LAST_ACTIVITY = 0;
		}
		return true;
	} else { // not logged set empty context variables
		context->LOGGED_IN = "false";
		context->USERNAME = "";
		context->EMAIL = "";
		context->SECONDARY_EMAIL = "";
		context->USER_ID = "";
		context->SETTINGS = "false";
		context->LAST_ACTIVITY = 0;
	}
	return false;
}