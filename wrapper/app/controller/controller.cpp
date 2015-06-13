#include "controller.hpp"
#include "view.hpp"
#include "user.hpp"

DXServer::~DXServer() {
	return;
}

/*
	@METHOD - Only GET method is allowed
	
	@FUNCTION -
		Gets session and sets context then
		Renders home page
*/
void DXServer::index_page() {
	session().load();
	// only allow get method
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	Context c;
	c.resolve_session(session());
	c.set_page("HOME");
	render("master", c);
	return;
}

/*
	@METHOD - Only GET method is allowed
	
	@FUNCTION -
		Render register page
		Gets session and sets context
*/
void DXServer::register_page() {
	session().load();
	// only allow get method
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	// disallow logged in
	if(Context::logged_in(session())) {
		response().set_redirect_header("/", 302);
		return;
	}
	Context c;
	c.resolve_session(session());
	c.set_page("REGISTER");
	render("master", c);
	return;
}

/*
	@METHOD - Only POST method is allowed

	@FUNCTION -
		Authenticates user based on submitted login form
*/
void DXServer::process_login() {
	session().load();
	// only allow POST method
	if(request().request_method() != "POST") {
		response().status(404);
		response().out() << "http POST is only method allowed on this page";
		return;
	}

	// disallow logged in
	if(Context::logged_in(session())) {
		json_response("DX-REJECTED", "logged_in");
		return;
	}

	// Determine if the user supplied a valid username or email
	int ID_TYPE;
	if(form::validUsername(request().post("u"))) { // is valid username ?
		ID_TYPE = ID_USERNAME;
	} else {
		if(form::validEmail(request().post("u"))) { // is valid email ?
			ID_TYPE = ID_EMAIL;
		} else { // invalid `u` field
			json_response("DX-REJECTED", "u_invalid");
			return;
		}
	}

	// query database for id + password combination
	if(ID_TYPE == ID_USERNAME) {
		try {
			// valid username login - set session & redirect
			if(db::try_login::with_username(&dbconn, request().post("u"), request().post("p"))) {
				// grab user info from database for session
				std::map<std::string, std::string> info = db::get_user::by_username(&dbconn, request().post("u"));
				// set session then redirect
				session().set("LOGGED_IN", "true");
				session().set("USERNAME", info["username"]);
				session().set("EMAIL", info["email"]);
				session().set("USER_ID", info["id"]);
				json_response("DX-OK", "Logged in");
				return;
			} else { // invalid username login
				json_response("DX-REJECTED", "U_invalid_login");
				return;
			}
		} catch(std::exception &e) {
			json_response("DX-FAILED", "Server error");
			return;
		}
	} else if(ID_TYPE == ID_EMAIL) {
		// valid email login
		try {
			if(db::try_login::with_email(&dbconn, request().post("u"), request().post("p"))) {
				// grab user info from database for session
				std::map<std::string, std::string> info = db::get_user::by_email(&dbconn, request().post("u"));
				// set session then redirect
				session().set("LOGGED_IN", "true");
				session().set("USERNAME", info["username"]);
				session().set("EMAIL", info["email"]);
				session().set("USER_ID", info["id"]);
				json_response("DX-OK", "Logged in");
				return;
			} else { // invalid email login
				json_response("DX-REJECTED", "E_invalid_login");
				return;
			}
		} catch(std::exception &e) {
			json_response("DX-FAILED", "Server error");
			return;
		}
	}
	return;
}

/*
	@METHOD - Only POST method is allowed

	@FUNCTION - 
		Ensure all fields are not empty
		Ensure all fields have validated values
		Ensure username & email are not already in use
		Create database entry for account
		Send activation token to user's email address
		Redirect to home page
*/
void DXServer::process_register() {
	session().load();
	// only allow post
	if(request().request_method() != "POST") {
		response().status(404);
		response().out() << "http POST is only method allowed on this page";
		return;
	}

	// disallow logged in users
	if(Context::logged_in(session())) {
		response().set_redirect_header("/", 302);
		return;
	}

	std::map<std::string, std::string> form;
	// get form data into std::map `form`
	form["f"] 	= to_lowercase(request().post("f"));
	form["l"] 	= to_lowercase(request().post("l"));
	form["u"] 	= to_lowercase(request().post("u"));
	form["p"] 	= request().post("p");
	form["pa"] 	= request().post("pa");
	form["e"] 	= to_lowercase(request().post("e"));
	form["ea"] 	= to_lowercase(request().post("ea"));
	form["z"] 	= request().post("z");
	form["g"] 	= to_lowercase(request().post("g"));
	form["tos"] = request().post("tos");
	if(form["tos"] == "") {
		std::string loc = "/register?err=tos" + form::generate_query(form);
		response().set_redirect_header(loc.c_str(), 302);
		return;
	}


	// if any empty fields return with error
	for(auto &kv : form) {
		if(kv.first == "g") {
			if(kv.second != "m" && kv.second != "f") { // male or female (m || f)
				// generate query and return
				std::string loc = "/register?err=invalid_form" + form::generate_query(form);
				response().set_redirect_header(loc.c_str(), 302);
				return;
			}
		} else {
			if(kv.second == "") {
				// generate query and return
				std::string loc = "/register?err=invalid_form" + form::generate_query(form);
				response().set_redirect_header(loc.c_str(), 302);
				return;
			}
		}
	}

	// save validation results
	std::map<std::string, int> errors;
	errors["F"] 	= form::validName(form["f"]);
	errors["L"] 	= form::validName(form["l"]);
	errors["U"] 	= form::validUsername(form["u"]);
	errors["P"] 	= form::validPassword(form["p"]);
	errors["PM"] 	= (form["p"] == form["pa"]) ? 1 : 0;
	errors["E"] 	= form::validEmail(form["e"]);
	errors["EM"] 	= (form["e"] == form["ea"]) ? 1 : 0;
	errors["Z"] 	= form::validZipcode(form["z"]);

	// check validation results
	std::string err = "";
	for(auto &kv : errors) {
		if(kv.second == 0) { // false
			err += kv.first + "|";
		}
	}

	if(err != "") {
		err.pop_back();
		std::string loc = "/register?err=" + err + form::generate_query(form);
		response().set_redirect_header(loc.c_str(), 302);
		return;
	}

	// check if username or email address is in use
	try {
		if(db::check_exist::username(&dbconn, form["u"])) {
			err += "UIN|";
		}
		if(db::check_exist::email(&dbconn, form["e"])) {
			err += "EIN|";
		}
		if(err != "") {
			err.pop_back();
			std::string loc = "/register?err=" + err + form::generate_query(form);
			response().set_redirect_header(loc.c_str(), 302);
			return;
		}
	} catch(std::exception &e) {
		// do not fail silently, blow up so i can figure out the cause
		response().status(500);
		response().out() << "Server error occurred";
		return;
	}


	// Create UserModel
	// these values must be specified...
	// firstname, lastname, username, email, password, token(for activation), zipcode
	// the timestamp is generated when UserModel.save() is called
	int zipcode_i;
	std::istringstream ss(form["z"]);
	ss >> zipcode_i;

	std::string token = mail::generate_token(form["e"]);

	// save the user into database
	UserModel user(form["f"], form["l"], form["u"], form["e"], form["p"], token, zipcode_i, form["g"]);
	try {
		user.save(&dbconn);
	} catch(std::exception &e) {
		// report error
		response().set_header("Content-Type", "text/html");
		response().out() << "Error creating account<br>";
		response().out() << "<a href='/'>Click here to return to home page</a>";
		return;
	}

	// email user the activation token
	mail::external::send_registration(form["e"], token);

	// grab the user from database to get the generated id
	try {
		std::map<std::string, std::string> info = db::get_user::by_email(&dbconn, form["e"]);
		if(info.empty()) {
			info = db::get_user::by_username(&dbconn, form["u"]);
			if(info.empty()) {
				// redirect to home page with session values
				response().set_redirect_header("/", 302);
				return;
			}
		}
		// set session
		session().set("LOGGED_IN", "true");
		session().set("USERNAME", info["username"]);
		session().set("EMAIL", info["email"]);
		session().set("USER_ID", info["id"]);
	} catch(std::exception &e) {
		// capture get user error
		response().out() << "Server error => " << e.what() << "<br>";
		return;
	}


	// redirect to home page with session values
	response().set_redirect_header("/", 302);
	return;
}

// Display forgot page offering options
// to select what data they wish to recover
void DXServer::forgot() {
	session().load();
	// only allow GET
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	// disallow logged in user
	if(Context::logged_in(session())) {
		response().set_redirect_header("/", 302);
		return;
	}
	Context c;
	c.resolve_session(session());
	c.set_page("FORGOT_INITIAL");
	render("master", c);
	return;
}

// Process the previous forgot page data
// validating input & determining what data to
// send to the user
// Errors
// -------
// invalid_type		--- neither radio buttons were selected ( username || password )
// invalid_email 	--- radio button username selected, but invalid email was specified
// invalid_id 		--- radio button password selected, but neither a valid username || email was given
void DXServer::process_forgot() {
	session().load();
	// Only allow POST method
	if(request().request_method() != "POST") {
		response().status(404);
		response().out() << "http POST is only method allowed on this page";
		return;
	}

	// disallow logged in user
	if(Context::logged_in(session())) {
		response().set_redirect_header("/", 302);
		return;
	}

	// validate forgot request
	std::string need = request().post("type");
	if(need != "username" && need != "password") {
		response().set_redirect_header("/forgot?err=invalid_type", 302);
		return;
	}

	if(need == "username") {
		// if not valid email return with error
		if(!form::validEmail(request().post("id"))) {
			response().set_redirect_header("/forgot?err=invalid_email", 302);
			return;
		}
		// try to grab account info by email
		std::map<std::string, std::string> info = db::get_user::by_email(&dbconn, request().post("id"));
		// if account information was retrieved send the username
		if(!info.empty()) {
			mail::external::send_username(request().post("id"), info["username"]);
		}
	} else if(need == "password") {
		// ensure valid username or email
		if(!form::validUsername(request().post("id"))) {
			if(!form::validEmail(request().post("id"))) {
				// bad id
				response().set_redirect_header("/forgot?err=invalid_id", 302);
				return;
			} else { // if valid email was specified
				// try to grab account info by email
				std::map<std::string, std::string> info = db::get_user::by_email(&dbconn, request().post("id"));
				// if account with specified email exists
				if(!info.empty()) {
					// generate password reset token
					std::string token = mail::generate_token(info["email"]);
					// save token to database
					std::pair<std::string, std::string> data1("forgot_token", token);
					std::pair<std::string, std::string> data2("forgot_timestamp", get_time());
					db::update::user::by_email(&dbconn, info["email"], data1, data2);
					// send username
					mail::external::send_pwd_reset(info["email"], token);
				} else {
					std::cout << "bad email was given => " << request().post("id") << std::endl;
				}
			}
		} else { // valid username
			// try to grab account info by username
			std::map<std::string, std::string> info = db::get_user::by_username(&dbconn, request().post("id"));
			if(!info.empty()) {
				// generate password reset token
				std::string token = mail::generate_token(info["email"]);
				// save token to database
				std::pair<std::string, std::string> data1("forgot_token", token);
				std::pair<std::string, std::string> data2("forgot_timestamp", get_time());
				db::update::user::by_email(&dbconn, info["email"], data1, data2);
				// send password reset token to user
				mail::external::send_pwd_reset(info["email"], token);
			} else {
				std::cout << "bad username was given => " << request().post("id") << std::endl;
			}
		}
	}

	// redirect to success page
	response().set_redirect_header("/forgot/success", 302);
	return;
}

// provides form for which user can request
// account information to be emailed
void DXServer::forgot_landing() {
	session().load();
	// only allow GET request
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}

	// disallow logged in users
	if(Context::logged_in(session())) {
		response().set_redirect_header("/", 302);
		return;
	}
	
	Context c;
	c.resolve_session(session());
	c.set_page("FORGOT_LANDING");
	render("master", c);
	return;
}

// provides form for resetting password if the user
// provided a valid token
void DXServer::reset() {
	session().load();
	// only allow GET method
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	// disallow logged in
	if(Context::logged_in(session())) {
		response().set_redirect_header("/", 302);
		return;
	}
	// redirect if empty token
	if(request().get("token") == "") {
		response().set_redirect_header("/forgot?err=tk_unset", 302);
		return;
	}
	// if valid token, render page
	if(db::check_exist::forgot_token(&dbconn, request().get("token"))) {
		Context c;
		c.resolve_session(session());
		c.set_page("PASSWORD_RESET");
		render("master", c);
	} else { // invalid token, redirect to error page
		response().set_redirect_header("/forgot?err=bad_token", 302);
	}
	return;
}

// validates and sets new password specified on previous page
void DXServer::process_reset() {
	session().load();
	// only allow POST
	if(request().request_method() != "POST") {
		response().status(404);
		response().out() << "http POST is only method allowed on this page";
		return;
	}
	// disallow logged in users
	if(Context::logged_in(session())) {
		response().set_redirect_header("/", 302);
		return;
	}
	// if not valid password, redirect with error
	if(!form::validPassword(request().post("np"))) {
		response().set_redirect_header("/reset?err=invalid_pwd", 302);
		return;
	}
	// if passwords dont match, redirect with error
	if(request().post("np") != request().post("npa")) {
		response().set_redirect_header("/reset?err=non_match", 302);
		return;
	}
	// decode the percent encoded token
	std::string token_f = cppcms::util::urldecode(request().post("token"));

	// check if password is the same as the existing one
	std::map<std::string, std::string> info = db::get_user::by_forgot_token(&dbconn, token_f);
	std::string password_f = crypto::gen_password::by_username(info["username"], request().post("np"));
	if(!info.empty()) { // if found with token
		// if passwords match, return with error same_password
		if(info["password"] == password_f) {
			std::string loc = "/reset?token=" + request().post("token") + "&err=same_password";
			response().set_redirect_header(loc.c_str(), 302);
			return;
		} else {
			// if timestamp is empty, error, need to request new token
			if(info["forgot_token"] == "") {
				response().set_redirect_header("/forgot?err=bk_error", 302);
				return;
			}

			// ensure the token hasn't timed out
			long int forgot_timestamp;
			long int current_time;
			try {
				forgot_timestamp = std::stoi(info["forgot_timestamp"], nullptr, 10);
				current_time = static_cast<long int>(std::time(0));
			} catch(std::exception &e) {
				std::cout << "Exception => " << e.what() << std::endl;
			}

			// Ensure the forgot_token hasn't timed out
			// 21600 seconds -> 6 hours
			if(current_time - forgot_timestamp >= 21600) {
				// clear forgot token/timestamp
				std::pair<std::string, std::string> data1("forgot_token", "");
				std::pair<std::string, std::string> data2("forgot_timestamp", "0");
				db::update::user::by_username(&dbconn, info["username"], data1, data2);
				// redirect with error
				response().set_redirect_header("/forgot?err=token_expired", 302);
				return;
			}


			// set new password
			std::pair<std::string, std::string> data1("password", password_f);
			db::update::user::by_username(&dbconn, info["username"], data1);

			// clear forgot token & timestamp
			std::pair<std::string, std::string> data2("forgot_token", "");
			std::pair<std::string, std::string> data3("forgot_timestamp", "0");
			db::update::user::by_username(&dbconn, info["username"], data2, data3);

			// email user about change
			mail::external::notice_password(info["email"]);

			// redirect to landing page
			response().set_redirect_header("/reset/success", 302);
			return;
		}
	} else { // if failed to find with token, return with error
		std::string loc = "/reset?token=" + request().post("token") + "&err=invalid_token";
		response().set_redirect_header(loc.c_str(), 302);
	}
	return;
}

// display success message
void DXServer::reset_landing() {
	session().load();
	// only allow GET method
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	// disallow logged in users
	if(Context::logged_in(session())) {
		response().set_redirect_header("/", 302);
		return;
	}
	// display message
	Context c;
	c.resolve_session(session());
	c.set_page("RESET_LANDING");
	render("master", c);
	return;
}

/*
	@METHOD - Only GET method is allowed
	
	@FUNCTION -
		Used by scripts to query server and retreive session state
		Main function of scripts is to log the user out should session timeout
		while inactive
*/
void DXServer::json_session() {
	session().load();
	// only allow get
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	Context c;
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

/*
	@METHOD - ANY

	@FUNCTION -
		Clears the session values
		Used to destroy authenticated sessions
*/
void DXServer::clear_session() {
	session().load();
	session().set("LOGGED_IN", "false");
	session().set("USERNAME", "");
	session().set("USER_ID", "");
	session().set("EMAIL", "");
	session().set("LAST_ACTIVITY", 0);
	return;
}

/*
	@METHOD - Not used by web client

	@FUNCTION -
		Updates session LAST_ACTIVITY to
		current time since UNIX EPOCH std::time(0)
*/
void DXServer::update_activity() {
	return;
}

/*
	@METHOD - Any

	@FUNCTION -
		Clears session values and redirects
		the user to home page
*/
void DXServer::logout() {
	clear_session();
	response().set_redirect_header("/", 302);
	return;
}



/*
	@METHOD - Not used by web client

	@FUNCTION -
		Returns a JSON Parsed response to client
		Structured as { status: '', message: '' }

		. Valid statuses -> `DX-OK`, `DX-REJECTED`, `DX-FAILED`
		. Message can vary to pre-specified error codes or custom responses
			depending on status
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
	Context c;
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
	response().out() << "Nothing to debug<br>";
	return;
}
