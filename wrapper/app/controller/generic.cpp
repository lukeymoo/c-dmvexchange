/*


TIP FROM QUORA USER
---------------------
1. Get a list of potential customers
2. Developma message
3. Create a catalogue
4 Start calling, emailing
5. Get perdonal visits
6. Sell, sell, sell
7. Talk to as many cuetomers as you can . Make your presentation and listen to them,mto see what they need
8. Tweak your speaxh with the following customers until you get it right.


*/




#include "generic.hpp"
#include "view.hpp"
#include "user.hpp"

DXServer::~DXServer() {
	return;
}

void DXServer::index_page() {
	// only allow get method
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	session().load();
	dxtemplate::context c;

	// make sure all context variables exist
	c.resolve_session(session());

	c.set_page("HOME");
	render("master", c);
	return;
}

void DXServer::register_page() {
	// only allow get method
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	session().load();
	dxtemplate::context c;
	c.resolve_session(session());
	if(c.is_logged_in(session())) {
		response().status(302);
		response().set_header("Location", "/");
		return;
	}
	c.set_page("REGISTER");
	render("master", c);
	return;
}

void DXServer::process_login() {
	// accept only POST requests
	if(request().request_method() != "POST") {
		response().status(404);
		response().out() << "http POST is only method allowed on this page";
		return;
	}
	session().load();
	// Make sure the user is not already logged in
	if(session().is_set("LOGGED_IN")) {
		if(session()["LOGGED_IN"].compare("true") == 0) {
			json_response("DX-OK", "Already logged in");
			return;
		}
	}
	int ID_TYPE = 0;
	if(ID_TYPE == 0) {
	}
	// is valid username ?
	if(validUsername(request().post("u"))) {
		ID_TYPE = ID_USERNAME;
	} else {
		// is valid email ?
		if(validEmail(request().post("u"))) {
			ID_TYPE = ID_EMAIL;
		} else {
			// bad username/email
			json_response("DX-REJECTED", "u_invalid");
			return;
		}
	}
	// is valid password
	if(!validPassword(request().post("p"))) {
		// bad password
		json_response("DX-REJECTED", "P");
		return;
	}
	// query server to determine if user exists
	json_response("DX-FAILED", "Server backend is under construction");
	return;
}


/** JSON RESPONSE ROUTES **/
void DXServer::json_session() {
	// only allow get
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	session().load();
	dxtemplate::context c;
	c.resolve_session(session());

	cppcms::json::value jres;
	jres["LOGGED_IN"] = c.LOGGED_IN;
	jres["USERNAME"] = c.USERNAME;
	jres["USER_ID"] = c.USER_ID;
	jres["EMAIL"] = c.EMAIL;
	jres["LAST_ACTIVITY"] = c.LAST_ACTIVITY;
	jres.save(response().out(), cppcms::json::compact);
	return;
}

void DXServer::clear_session() {
	session().load();
	session().set("LOGGED_IN", "false");
	session().set("USERNAME", "");
	session().set("USER_ID", "");
	session().set("EMAIL", "");
	session().set("LAST_ACTIVITY", 0);
	return;
}

void DXServer::update_activity() {
	return;
}

void DXServer::logout() {
	clear_session();
	response().status(302);
	response().set_header("Location", "/");
	return;
}



/*

send standard json structured response
{
	status: "",
	message: ""
}

*/
void DXServer::json_response(std::string status, std::string message) {
	cppcms::json::value jres;
	jres["status"] = status;
	jres["message"] = message;
	jres.save(response().out(), cppcms::json::compact);
	return;
}







// Debugging
void DXServer::debug_session() {
	session().load();
	dxtemplate::context c;
	c.resolve_session(session());
	cppcms::json::value jres;
	jres["LOGGED_IN"] = c.LOGGED_IN;
	jres["USERNAME"] = c.USERNAME;
	jres["EMAIL"] = c.EMAIL;
	jres["USER_ID"] = c.USER_ID;
	jres["LAST_ACTIVITY"] = c.LAST_ACTIVITY;
	jres.save(response().out(), cppcms::json::readable);
	return;
}

void DXServer::debug_page() {
	response().out() << "Timestamp => " << std::time(0);
	return;
}





/** Helpers **/
/*

Username Allowed Characters

A-Z => 65 -> 90
a-z => 97 -> 122
0-9 => 48 -> 57
_ 	=> 95 		( underscore character )

*/
bool validUsername(std::string word) {
	int length = 0;
	// lower case string
	std::string lowercase = to_lowercase(word);
	// iterate and test characters ( if i makes it through check its GOOD )
	for(char &c : lowercase) {
		// a-z ?
		if(c >= 97 && c <= 122) {
			++length;
			continue;
		} else {
			// 0-9 ?
			if(c >= 48 && c <= 57) {
				++length;
				continue;
			} else {
				// is it an underscore ?
				if(c == 95) {
					++length;
					continue;
				} else {
					// No match == bad character
					return false;
				}
			}
		}
	}
	// if the length is below 2 or over 16 == bad username
	if(length < 2 || length > 16) {
		return false;
	}
	return true;
}

// fuck checking lol, just make sure no spaces and less than 65 characters
// must only contain 1 `@` symbol & there must be at least 3 characters AFTER `@`
bool validEmail(std::string word) {
	int cafter = 0;
	int numat = 0;
	bool afterat = false;
	// lower case string
	std::string lowercase = to_lowercase(word);
	// space checking
	for(char &c : lowercase) {
		if(afterat) {
			++cafter;
		}
		if(c == '@') {
			++numat;
			afterat = true;
		}
		if(c == ' ') {
			return false;
		}
	}
	// `@` checker -> needs only 1
	if(numat != 1) {
		return false;
	}
	// ensure cafter >= 3
	if(cafter < 3) {
		return false;
	}
	// length check
	if(lowercase.length() > 64) {
		return false;
	}
	return true;
}

/**
	Validate password
	Must be 2-32 characters
*/
bool validPassword(std::string word) {
	if(word.length() < 2 || word.length() > 32) {
		return false;
	}
	return true;
}

/**
	Lowercase characters in string
*/
std::string to_lowercase(std::string word) {
	std::locale loc;
	std::stringstream ss;
	for(std::string::size_type i = 0; i < word.length(); i++) {
		ss << std::tolower(word[i], loc);
	}
	return ss.str();
}


