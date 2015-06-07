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

/*
	@METHOD - Only GET method is allowed
	
	@FUNCTION -
		Gets session and sets context then
		Renders home page
*/
void DXServer::index_page() {
	// only allow get method
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	session().load();
	dxtemplate::context c;
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
	// only allow get method
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	session().load();
	dxtemplate::context c;
	c.resolve_session(session());
	if(c.LOGGED_IN == "true") {
		response().status(302);
		response().set_header("Location", "/");
		return;
	}
	c.set_page("REGISTER");
	render("master", c);
	return;
}

/*
	@METHOD - Only POST method is allowed

	@FUNCTION - 
		Processes login form which contains
		`u` and `p` field ~ Username/Email, Password field
		
		Ensure the user sent a valid username or email in the `u` field
		Ensures the user sent a valid password

		Queries server to determine if username/email + password combo exists in database

		Returns a JSON response structured => { status: '', message: '' }
		where status is 1 of types DX-OK, DX-REJECTED or DX-FAILED and
		where message is 1 of types...

		Meaning 						Error code
		----------------------- 	   ----------------
		username/email invalid 		=> u_invalid
		invalid username/password 	=> U_invalid_login
		invalid email/password 		=> E_invalid_login
		missing form fields 		=> invalid_form
		invalid username 			=> U
		invalid email 				=> E
		invalid password 			=> P
*/
void DXServer::process_login() {
	// only allow POST method
	if(request().request_method() != "POST") {
		response().status(404);
		response().out() << "http POST is only method allowed on this page";
		return;
	}

	// Make sure the user is not already logged in
	session().load();
	if(dxtemplate::context::is_logged_in(session())) {
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
	// only allow post
	if(request().request_method() != "POST") {
		response().status(404);
		response().out() << "http POST is only method allowed on this page";
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
		response().status(302);
		response().set_header("Location", loc.c_str());
		return;
	}


	// if any empty fields return with error
	for(auto &kv : form) {
		if(kv.first == "g") {
			if(kv.second != "m" && kv.second != "f") { // male or female (m || f)
				// generate query and return
				std::string loc = "/register?err=invalid_form" + form::generate_query(form);
				response().status(302);
				response().set_header("Location", loc.c_str());
				return;
			}
		} else {
			if(kv.second == "") {
				// generate query and return
				std::string loc = "/register?err=invalid_form" + form::generate_query(form);
				response().status(302);
				response().set_header("Location", loc.c_str());
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
		response().status(302);
		response().set_header("Location", loc.c_str());
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
			response().status(302);
			response().set_header("Location", loc.c_str());
			return;
		}
	} catch(std::exception &e) {
		// do not fail silently, blow up so i can figure out the cause
		response().status(500);
		response().out() << "Server error occurred";
		return;
	}

	// no errors
	response().out() << "Valid registration, creating account";

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
	response().status(302);
	response().set_header("Location", "/");
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
	response().set_header("Content-Type", "text/html");
	response().out() << "<span style='font-family:\"Ubuntu Mono\";'>";

	// username exists
	if(db::check_exist::username(&dbconn, "lukeymoo")) {
		response().out() << "Normal call - Username\t=>\tlukeymoo exists<br>";
	} else {
		response().out() << "Normal call - Username\t=>\tlukeymoo does not exists<br>";
	}

	// username does not exist
	if(db::check_exist::username(&dbconn, "lukeymoo2")) {
		response().out() << "Normal call - Username\t=>\tlukeymoo2 exists<br>";
	} else {
		response().out() << "Normal call - Username\t=>\tlukeymoo2 does not exists<br>";
	}

	// email exists
	if(db::check_exist::email(&dbconn, "lukeymoo@hotmail.com")) {
		response().out() << "Email\t=>\tlukeymoo@hotmail.com exists<br>";
	} else {
		response().out() << "Email\t=>\tlukeymoo@hotmail.com does not exists<br>";
	}

	// email does not exist
	if(db::check_exist::email(&dbconn, "lukeymoo2@hotmail.com")) {
		response().out() << "Email\t=>\tlukeymoo2@hotmail.com exists<br>";
	} else {
		response().out() << "Email\t=>\tlukeymoo2@hotmail.com does not exists<br>";
	}

	// valid name
	if(form::validName("luke")) {
		response().out() << "valid name\t=>\tluke<br>";
	} else {
		response().out() << "invalid name\t=>\tluke<br>";
	}

	// invalid name
	if(form::validName("luke morrison")) {
		response().out() << "valid name\t=>\tluke morrison<br>";
	} else {
		response().out() << "invalid name\t=>\tluke morrison<br>";
	}

	// valid username
	if(form::validUsername("lukeymoo__")) {
		response().out() << "valid username\t=>\tlukeymoo__<br>";
	} else {
		response().out() << "invalid username\t=>\tlukeymoo__<br>";
	}

	// invalid username
	if(form::validUsername("lukeymoo!")) {
		response().out() << "valid username\t=>\tlukeymoo!<br>";
	} else {
		response().out() << "invalid username\t=>\tlukeymoo!<br>";
	}

	// valid email
	if(form::validEmail("lukeymoo@hotmail.com")) {
		response().out() << "valid email\t=>\tlukeymoo@hotmail.com<br>";
	} else {
		response().out() << "invalid email\t=>\tlukeymoo@hotmail.com<br>";
	}

	// invalid email
	if(form::validEmail("lukeymoo@@hotmail.com")) {
		response().out() << "valid email\t=>\tlukeymoo@@hotmail.com<br>";
	} else {
		response().out() << "invalid email\t=>\tlukeymoo@@hotmail.com<br>";
	}

	// valid password
	if(form::validPassword("password")) {
		response().out() << "valid password\t=>\tpassword<br>";
	} else {
		response().out() << "invalid password\t=>\tpassword<br>";
	}

	// invalid password
	if(form::validPassword("a")) {
		response().out() << "valid password\t=>\ta<br>";
	} else {
		response().out() << "invalid password\t=>\ta<br>";
	}

	// grab user info
	try {
		std::map<std::string, std::string> test = db::get_user::by_id(&dbconn, 1);
		if(test.empty()) {
			response().out() << "No user with id => `1`<br>";
		} else {
			std::string result = crypto::sha512_enc("hello");
			response().out() << "usr pwd => " << test["password"] << "<br>";
			response().out() << "encoded => " << result << "<br>";
			result = crypto::sha512_noenc("hello");
			response().out() << "not-enc => " << result << "<br>";
		}
	} catch(std::exception &e) {
		response().out() << "exception => " << e.what();
	}
	

	response().out() << "</span>";
	return;
}
