#include "view.h"


// constuctor -> Sets default values for a session
dxtemplate::session::session() {
	TITLE = "Home";
	
	PAGE = "HOME";
	PAGE_HOME = true;
	PAGE_REGISTER = false;
	PAGE_CREATEPOST = false;

	LOGGED_IN = false;
	LAST_ACTIVITY = 0;
	USERNAME = "";
	return;
}

dxtemplate::session::~session() {
	return;
}

void dxtemplate::session::set_page(std::string type) {
	reset_page();
	if(type.compare("HOME") == 0) {
		TITLE = "Home";
		PAGE = "HOME";
		PAGE_HOME = true;
	}
	if(type.compare("REGISTER") == 0) {
		TITLE = "Register";
		PAGE = "REGISTER";
		PAGE_REGISTER = true;
	}
	if(type.compare("CREATEPOST") == 0) {
		TITLE = "Create Post";
		PAGE = "CREATEPOST";
		PAGE_CREATEPOST = true;
	}
	return;
}

void dxtemplate::session::reset_page() {
	PAGE = "HOME";
	PAGE_HOME = true;
	PAGE_REGISTER = false;
	PAGE_CREATEPOST = false;
	return;
}

std::string dxtemplate::session::get_title() {
	return TITLE;
}

bool dxtemplate::session::is_logged_in() {
	return LOGGED_IN;
}

bool dxtemplate::session::eval_logged_in() {
	return false;
}