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
	if(UserModel::validUsername(request().post("u"))) { // is valid username ?
		ID_TYPE = ID_USERNAME;
	} else {
		if(UserModel::validEmail(request().post("u"))) { // is valid email ?
			ID_TYPE = ID_EMAIL;
		} else { // invalid `u` field
			json_response("DX-REJECTED", "u_invalid");
			return;
		}
	}

	// query database for id + password combination
	if(ID_TYPE == ID_USERNAME) {
		// valid username login - set session & redirect
		if(Database::username_login(&db.conn, request().post("u"), request().post("p"))) {
			// grab user info from database for session
			return;
		} else { // invalid username login
			json_response("DX-REJECTED", "U_invalid_login");
			return;
		}
	} else if(ID_TYPE == ID_EMAIL) {
		// valid email login
		if(Database::email_login(&db.conn, request().post("u"), request().post("p"))) {
			return;
		} else { // invalid email login
			json_response("DX-REJECTED", "E_invalid_login");
			return;
		}
	}
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
	response().out() << "<span style='font-family:\"Consolas\";'>";

	// username exists
	if(Database::username_exist(&db.conn, "lukeymoo")) {
		response().out() << "Normal call - Username\t=>\tlukeymoo exists<br>";
	} else {
		response().out() << "Normal call - Username\t=>\tlukeymoo does not exists<br>";
	}

	// username does not exist
	if(Database::username_exist(&db.conn, "lukeymoo2")) {
		response().out() << "Normal call - Username\t=>\tlukeymoo2 exists<br>";
	} else {
		response().out() << "Normal call - Username\t=>\tlukeymoo2 does not exists<br>";
	}

	// email exists
	if(Database::email_exist(&db.conn, "lukeymoo@hotmail.com")) {
		response().out() << "Email\t=>\tlukeymoo@hotmail.com exists<br>";
	} else {
		response().out() << "Email\t=>\tlukeymoo@hotmail.com does not exists<br>";
	}

	// email does not exist
	if(Database::email_exist(&db.conn, "lukeymoo2@hotmail.com")) {
		response().out() << "Email\t=>\tlukeymoo2@hotmail.com exists<br>";
	} else {
		response().out() << "Email\t=>\tlukeymoo2@hotmail.com does not exists<br>";
	}

	// valid name
	if(UserModel::validName("luke")) {
		response().out() << "valid name\t=>\tluke<br>";
	} else {
		response().out() << "invalid name\t=>\tluke<br>";
	}

	// invalid name
	if(UserModel::validName("luke morrison")) {
		response().out() << "valid name\t=>\tluke morrison<br>";
	} else {
		response().out() << "invalid name\t=>\tluke morrison<br>";
	}

	// valid username
	if(UserModel::validUsername("lukeymoo__")) {
		response().out() << "valid username\t=>\tlukeymoo__<br>";
	} else {
		response().out() << "invalid username\t=>\tlukeymoo__<br>";
	}

	// invalid username
	if(UserModel::validUsername("lukeymoo!")) {
		response().out() << "valid username\t=>\tlukeymoo!<br>";
	} else {
		response().out() << "invalid username\t=>\tlukeymoo!<br>";
	}

	// valid email
	if(UserModel::validEmail("lukeymoo@hotmail.com")) {
		response().out() << "valid email\t=>\tlukeymoo@hotmail.com<br>";
	} else {
		response().out() << "invalid email\t=>\tlukeymoo@hotmail.com<br>";
	}

	// invalid email
	if(UserModel::validEmail("lukeymoo@@hotmail.com")) {
		response().out() << "valid email\t=>\tlukeymoo@@hotmail.com<br>";
	} else {
		response().out() << "invalid email\t=>\tlukeymoo@@hotmail.com<br>";
	}

	// valid password
	if(UserModel::validPassword("password")) {
		response().out() << "valid password\t=>\tpassword<br>";
	} else {
		response().out() << "invalid password\t=>\tpassword<br>";
	}

	// invalid password
	if(UserModel::validPassword("a")) {
		response().out() << "valid password\t=>\ta<br>";
	} else {
		response().out() << "invalid password\t=>\ta<br>";
	}

	// grab user info
	try {
		std::map<std::string, std::string> test = Database::getUser(&db.conn, 1);
	} catch(std::exception &e) {
		response().out() << "exception => " << e.what();
	}
	

	response().out() << "</span>";
	return;
}