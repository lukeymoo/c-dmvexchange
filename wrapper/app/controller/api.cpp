#include "api.hpp"

api::~api() {
	return;
}

// Add new email
void api::settings_add_email() {
	session().load();
	// allow only POST
	if(request().request_method() != "POST") {
		response().status(404);
		response().out() << "http POST is only method allowed on this endpoint";
		return;
	}
	// ensure logged in
	if(!Pages::logged_in(session())) {
		json::send("DX-REJECTED", "Must be logged in", response().out());
		return;
	}
	// ensure we've been given a valid email
	if(!form::validEmail(request().post("email"))) {
		json::send("DX-REJECTED", "Not a valid email address", response().out());
		return;
	}
	// check if email is in use
	if(db::check_exist::email(db, to_lowercase(request().post("email")))) {
		json::send("DX-REJECTED", "Email in use", response().out());
		return;
	}
	// add email to user's account
	std::pair<std::string, std::string> data1("secondary_email", to_lowercase(request().post("email")));
	try {
		// add the user to the database
		db::update::user::by_username(db, session().get("USERNAME"), data1);
		// send email to user telling them email has been added
		mail::external::notice_new_email(request().post("email"), session().get("USERNAME"));
	} catch(std::exception &e) {
		json::send("DX-FAILED", "Server error occurred, please try again", response().out());
		return;
	}
	// update session
	session().set("SECONDARY_EMAIL", to_lowercase(request().post("email")));
	// added email, return ok
	json::send("DX-OK", "Email added!", response().out());
	return;
}

void api::settings_unlock() {
	// only allow POST
	if(request().request_method() != "POST") {
		json::send("DX-REJECTED", "http POST is only method allowed", response().out());
		return;
	}
	// ensure logged in
	if(!Pages::logged_in(session())) {
		json::send("DX-REJECTED", "Must be logged in!", response().out());
	}
	// try login
	if(db::try_login::with_username(db, session().get("USERNAME"), request().post("password"))) {
		session().set("SETTINGS", "true");
		json::send("DX-OK", "Authenicated", response().out());
	} else {
		session().set("SETTINGS", "false");
		json::send("DX-REJECTED", "Incorrect password", response().out());
	}
	return;
}

void api::settings_block() {
	// only allow POST
	if(request().request_method() != "POST") {
		json::send("DX-REJECTED", "http POST is only method allowed on this page", response().out());
		return;
	}

	// ensure logged in
	if(!Pages::logged_in(session())) {
		json::send("DX-REJECTED", "Must be logged in!", response().out());
	}

	std::string username = to_lowercase(request().post("username"));

	// ensure valid username specified
	if(!form::validUsername(username)) {
		json::send("DX-REJECTED", "Not a valid username", response().out());
		return;
	}

	// ensure this is not the user's username
	if(session().get("USERNAME") == username) {
		json::send("DX-REJECTED", "Cannot block yourself", response().out());
		return;
	}

	// if user already blocked send DX-OK
	if(db::check_in::blocked_list::by_id(db, session().get<int>("USER_ID"), username)) {
		json::send("DX-OK", "Already blocked", response().out());
		return;
	}

	// Add to block list
	db::block::by_id(db, session().get<int>("USER_ID"), username);
	
	json::send("DX-OK", "Blocked", response().out());
	return;
}

void api::settings_unblock() {
	// only allow POST
	if(request().request_method() != "POST") {
		json::send("DX-REJECTED", "http POST is only method allowed on this page", response().out());
		return;
	}

	// ensure logged in
	if(!Pages::logged_in(session())) {
		json::send("DX-REJECTED", "Must be logged in!", response().out());
	}
	
	std::string username = to_lowercase(request().post("username"));
	
	// ensure valid username was specified
	if(!form::validUsername(username)) {
		json::send("DX-REJECTED", "Not a valid username", response().out());
		return;
	}

	// if user's username, skip to success
	if(session().get("USERNAME") == username) {
		json::send("DX-OK", "Unblocked", response().out());
		return;
	}
	
	// remove user from blocked list

	json::send("DX-OK", "Unblocked", response().out());
	return;
}

void api::settings_blocked_list() {
	std::vector<std::string> list = db::get::blocked_list::by_id(db, session().get<int>("USER_ID"));
	// create string from vector of names
	std::stringstream ss;
	copy(list.begin(), list.end(), std::ostream_iterator<std::string>(ss, ","));
	// remove last delimiter
	std::string as_string = ss.str();
	if(as_string.length() > 0) {
		as_string.pop_back();
	}
	json::send("DX-OK", as_string, response().out()); // send to client
	return;
}

void api::settings_check_auth() {
	session().load();
	// only allow GET
	if(request().request_method() != "GET") {
		json::send("DX-REJECTED", "http GET is only method allowed on this page", response().out());
		return;
	}

	// if not logged in auto-fail
	if(!Pages::logged_in(session())) {
		json::send("DX-REJECTED", "Must be logged in", response().out());
		return;
	}

	// send session SETTINGS
	if(session().get("SETTINGS") == "true") {
		json::send("DX-OK", "true", response().out());
	} else {
		json::send("DX-OK", "false", response().out());
	}
	return;
}

void api::settings_change_password() {
	session().load();
	// only allow POST
	if(request().request_method() != "POST") {
		json::send("DX-REJECTED", "http POST is only method allowed on this page", response().out());
		return;
	}
	// if not logged in auto-fail
	if(!Pages::logged_in(session())) {
		json::send("DX-REJECTED", "Must be logged in", response().out());
		return;
	}
	// ensure valid password
	if(!form::validPassword(request().post("initial"))) {
		json::send("DX-REJECTED", "Not a valid password", response().out());
		return;
	}
	// ensure both passwords match
	if(request().post("initial") != request().post("confirmed")) {
		json::send("DX-REJECTED", "Passwords must match", response().out());
		return;
	}
	// generate password
	std::string password = crypto::gen_password::by_username(session().get("USERNAME"), request().post("initial"));
	// try login, if password is the same, reject request
	if(db::try_login::with_username_enc(db, session().get("USERNAME"), password)) {
		json::send("DX-REJECTED", "That is already you're password!", response().out());
		return;
	} else {
		// set new password
		std::pair<std::string, std::string> data1("password", password);
		//db::update::user::by_username(db, session().get("USERNAME"), data1);
		json::send("DX-OK", "Password updated", response().out());
	}
	return;
}

void api::session_state() {
	session().load();
	// only allow get
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	Pages::resolve_session(context, session());

	cppcms::json::value jres;
	jres["LOGGED_IN"] = context->LOGGED_IN;
	jres["USERNAME"] = context->USERNAME;
	jres["USER_ID"] = context->USER_ID;
	jres["EMAIL"] = context->EMAIL;
	jres["LAST_ACTIVITY"] = context->LAST_ACTIVITY;
	jres.save(response().out(), cppcms::json::compact);
	return;
}

void json::send(std::string status, std::string message, std::ostream &stream) {
	cppcms::json::value jres;
	jres["status"] = status;
	jres["message"] = message;
	jres.save(stream, cppcms::json::compact);
	return;
}