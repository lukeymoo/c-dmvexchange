#include "api.hpp"

api::~api() {
	return;
}

// Add new email
void api::new_email() {
	session().load();
	// allow only POST
	if(request().request_method() != "POST") {
		response().status(404);
		response().out() << "http POST is only method allowed on this endpoint";
		return;
	}
	// ensure logged in
	if(!Context::logged_in(session())) {
		json::send("DX-REJECTED", "Must be logged in", response().out());
		return;
	}
	// ensure we've been given a valid email
	if(!form::validEmail(request().post("email"))) {
		json::send("DX-REJECTED", "Not a valid email address", response().out());
		return;
	}
	// check if email is in use
	if(db::check_exist::email(dbconn, to_lowercase(request().post("email")))) {
		json::send("DX-REJECTED", "Email in use", response().out());
		return;
	}
	// add email to user's account
	std::pair<std::string, std::string> data1("secondary_email", to_lowercase(request().post("email")));
	try {
		db::update::user::by_username(dbconn, session().get<std::string>("USERNAME"), data1);
	} catch(std::exception &e) {
		json::send("DX-FAILED", "Server error occurred, please try again", response().out());
		return;
	}
	// added email, return ok
	json::send("DX-OK", "Email added", response().out());
	return;
}

void api::session_handler() {
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

void json::send(std::string status, std::string message, std::ostream &stream) {
	cppcms::json::value jres;
	jres["status"] = status;
	jres["message"] = message;
	jres.save(stream, cppcms::json::compact);
	return;
}