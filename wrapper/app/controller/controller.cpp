#include "controller.hpp"
#include "view.hpp"
#include "user.hpp"

BaseController::~BaseController() {
	return;
}

// @METHOD - Only GET method is allowed
// @FUNCTION -	Gets session and sets context then renders home page
void BaseController::index_main() {
	session().load();
	// only allow get method
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	Pages::set_page(&context, "HOME");
	Pages::resolve_session(&context, session());
	render("master", context);
	return;
}

// @METHOD - GET
// @FUNCTION - Display tip form
void BaseController::tips_main() {
	session().load();
	// only allow get
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	Pages::set_page(&context, "TIPS_MAIN");
	// ensure logged in
	if(!Pages::logged_in(session())) {
		response().set_redirect_header("/?err=need_login&next=/tips", 302);
		return;
	}
	Pages::resolve_session(&context, session());
	render("master", context);
	return;
}

// @METHOD - POST
// @FUNCTION - Process tip form
void BaseController::tips_process() {
	response().out() << "processing...";
	return;
}

// @METHOD - GET
// @FUNCTION - Process activation token
void BaseController::activate_main() {
	// only allow GET
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	
	// capture token
	std::string token = request().get("token");

	

	// if no token, render empty token template
	if(token == "") {
		Pages::set_page(&context, "ACTIVATION_EMPTY");
		Pages::resolve_session(&context, session());
		render("master", context);
		return;
	}

	// if token exists, render success
	if(db::check_exist::activation_token(db, token)) {
		// set activation token to empty -> token used(activated)
		std::pair<std::string, std::string> data("token", "");
		db::update::user::by_activation_token(db, token, data);

		Pages::set_page(&context, "ACTIVATION_SUCCESS");
		Pages::resolve_session(&context, session());
		render("master", context);
	} else { // if not, render failed
		Pages::set_page(&context, "ACTIVATION_FAILED");
		Pages::resolve_session(&context, session());
		render("master", context);
	}
	return;
}

// @METHOD - Only GET method is allowed
// @FUNCTION - Render register page gets session and sets context
void BaseController::register_main() {
	session().load();
	// only allow get method
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	// disallow logged in
	if(Pages::logged_in(session())) {
		response().set_redirect_header("/", 302);
		return;
	}
	Pages::set_page(&context, "REGISTER");
	Pages::resolve_session(&context, session());
	render("master", context);
	return;
}

// @METHOD - Only POST method is allowed
// @FUNCTION -	Authenticates user based on submitted login form
void BaseController::login_process() {
	session().load();
	// only allow POST method
	if(request().request_method() != "POST") {
		response().status(404);
		response().out() << "http POST is only method allowed on this page";
		return;
	}

	// disallow logged in
	if(Pages::logged_in(session())) {
		json::send("DX-REJECTED", "logged_in", response().out());
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
			json::send("DX-REJECTED", "u_invalid", response().out());
			return;
		}
	}

	// query database for id + password combination
	if(ID_TYPE == ID_USERNAME) {
		try {
			// valid username login - set session & redirect
			if(db::try_login::with_username(db, request().post("u"), request().post("p"))) {
				// grab user info from database for session
				std::map<std::string, std::string> info = db::get::user::by_username(db, request().post("u"));
				// set session then redirect
				session().set("LOGGED_IN", "true");
				session().set("USERNAME", info["username"]);
				session().set("EMAIL", info["email"]);
				session().set("SECONDARY_EMAIL", info["secondary_email"]);
				session().set("USER_ID", info["id"]);
				session().set("SETTINGS", "false");
				json::send("DX-OK", "Logged in", response().out());
				return;
			} else { // invalid username login
				json::send("DX-REJECTED", "U_invalid_login", response().out());
				return;
			}
		} catch(std::exception &e) {
			json::send("DX-FAILED", "Server error", response().out());
			return;
		}
	} else if(ID_TYPE == ID_EMAIL) {
		// valid email login
		try {
			if(db::try_login::with_email(db, request().post("u"), request().post("p"))) {
				// grab user info from database for session
				std::map<std::string, std::string> info = db::get::user::by_email(db, request().post("u"));
				// set session then redirect
				session().set("LOGGED_IN", "true");
				session().set("USERNAME", info["username"]);
				session().set("EMAIL", info["email"]);
				session().set("SECONDARY_EMAIL", info["secondary_email"]);
				session().set("USER_ID", info["id"]);
				session().set("SETTINGS", "false");
				json::send("DX-OK", "Logged in", response().out());
				return;
			} else { // invalid email login
				json::send("DX-REJECTED", "E_invalid_login", response().out());
				return;
			}
		} catch(std::exception &e) {
			std::cerr << "[-] Exception occurred => " << e.what() << std::endl;
			json::send("DX-FAILED", "Server error", response().out());
			return;
		}
	}
	return;
}

// @METHOD - Only POST method is allowed
// @FUNCTION - 
//		Ensure all fields are not empty
//		Ensure all fields have validated values
//		Ensure username & email are not already in use
//		Create database entry for account
//		Send activation token to user's email address
//		Redirect to home page
void BaseController::register_process() {
	session().load();
	// only allow post
	if(request().request_method() != "POST") {
		response().status(404);
		response().out() << "http POST is only method allowed on this page";
		return;
	}

	// disallow logged in users
	if(Pages::logged_in(session())) {
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
		if(db::check_exist::username(db, form["u"])) {
			err += "UIN|";
		}
		if(db::check_exist::email(db, form["e"])) {
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
		user.save(db);
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
		std::map<std::string, std::string> info = db::get::user::by_email(db, form["e"]);
		if(info.empty()) {
			info = db::get::user::by_username(db, form["u"]);
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
		session().set("SECONDARY_EMAIL", "");
		session().set("USER_ID", info["id"]);
		session().set("SETTINGS", "false");

		// create block list for user
		db::create::block_list(db, std::stoi(info["id"], nullptr, 10));
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
void BaseController::forgot_main() {
	session().load();
	// only allow GET
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	// disallow logged in user
	if(Pages::logged_in(session())) {
		response().set_redirect_header("/", 302);
		return;
	}
	Pages::set_page(&context, "FORGOT_INITIAL");
	Pages::resolve_session(&context, session());
	render("master", context);
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
void BaseController::forgot_process() {
	session().load();
	// Only allow POST method
	if(request().request_method() != "POST") {
		response().status(404);
		response().out() << "http POST is only method allowed on this page";
		return;
	}

	// disallow logged in user
	if(Pages::logged_in(session())) {
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
		std::map<std::string, std::string> info = db::get::user::by_email(db, request().post("id"));
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
				std::map<std::string, std::string> info = db::get::user::by_email(db, request().post("id"));
				// if account with specified email exists
				if(!info.empty()) {
					// generate password reset token
					std::string token = mail::generate_token(info["email"]);
					// save token to database
					std::pair<std::string, std::string> data1("forgot_token", token);
					std::pair<std::string, std::string> data2("forgot_timestamp", get_time());
					db::update::user::by_email(db, info["email"], data1, data2);
					// send username
					mail::external::send_pwd_reset(info["email"], token);
				} else {
					std::cout << "bad email was given => " << request().post("id") << std::endl;
				}
			}
		} else { // valid username
			// try to grab account info by username
			std::map<std::string, std::string> info = db::get::user::by_username(db, request().post("id"));
			if(!info.empty()) {
				// generate password reset token
				std::string token = mail::generate_token(info["email"]);
				// save token to database
				std::pair<std::string, std::string> data1("forgot_token", token);
				std::pair<std::string, std::string> data2("forgot_timestamp", get_time());
				db::update::user::by_email(db, info["email"], data1, data2);
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
void BaseController::forgot_success() {
	session().load();
	// only allow GET request
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}

	// disallow logged in users
	if(Pages::logged_in(session())) {
		response().set_redirect_header("/", 302);
		return;
	}
	
	Pages::set_page(&context, "FORGOT_LANDING");
	Pages::resolve_session(&context, session());
	render("master", context);
	return;
}

// provides form for resetting password if the user
// provided a valid token
void BaseController::reset_main() {
	session().load();
	// only allow GET method
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	// disallow logged in
	if(Pages::logged_in(session())) {
		response().set_redirect_header("/", 302);
		return;
	}
	// redirect if empty token
	if(request().get("token") == "") {
		response().set_redirect_header("/forgot?err=tk_unset", 302);
		return;
	}
	// if valid token, render page
	if(db::check_exist::forgot_token(db, request().get("token"))) {
		Pages::set_page(&context, "PASSWORD_RESET");
		Pages::resolve_session(&context, session());
		render("master", context);
	} else { // invalid token, redirect to error page
		response().set_redirect_header("/forgot?err=bad_token", 302);
	}
	return;
}

// validates and sets new password specified on previous page
void BaseController::reset_process() {
	session().load();
	// only allow POST
	if(request().request_method() != "POST") {
		response().status(404);
		response().out() << "http POST is only method allowed on this page";
		return;
	}
	// disallow logged in users
	if(Pages::logged_in(session())) {
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
	std::map<std::string, std::string> info = db::get::user::by_forgot_token(db, token_f);
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
			long int forgot_timestamp = 0;
			long int current_time = 0;
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
				db::update::user::by_username(db, info["username"], data1, data2);
				// redirect with error
				response().set_redirect_header("/forgot?err=token_expired", 302);
				return;
			}


			// set new password
			std::pair<std::string, std::string> data1("password", password_f);
			db::update::user::by_username(db, info["username"], data1);

			// clear forgot token & timestamp
			std::pair<std::string, std::string> data2("forgot_token", "");
			std::pair<std::string, std::string> data3("forgot_timestamp", "0");
			db::update::user::by_username(db, info["username"], data2, data3);

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

// Render create post form
void BaseController::p_new() {
	// allow only GET
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}

	// only allow logged in users
	if(!Pages::logged_in(session())) {
		response().set_redirect_header("/?err=need_login&next=/p/new", 302);
		return;
	}

	// render page
	Pages::set_page(&context, "CREATEPOST");
	Pages::resolve_session(&context, session());
	render("master", context);
	return;
}

void BaseController::p_process() {
	// allow only POST
	if(request().request_method() != "POST") {
		response().status(404);
		response().out() << "http POST is only method allowed on this page";
		return;
	}
	
	// Ensure logged in
	if(!Pages::logged_in(session())) {
		response().set_redirect_header("/?err=need_login&next=/p/new", 302);
		return;
	}

	// capture all fields
	std::map<std::string, std::string> form;
	form["type"] = request().post("posttype");
	form["description"] = request().post("postdescription");

	// Ensure type is either sale or general
	if(form["type"] != "sale" && form["type"] != "general") {
		response().set_redirect_header("/p/new?err=invalid_type&desc=" + cppcms::util::urlencode(form["description"]));
		return;
	}

	// Ensure description is at least 10 characters
	if(form["description"].length() <= 9) {
		response().set_redirect_header("/p/new?err=invalid_desc&type=" + form["type"] + "&desc=" + cppcms::util::urlencode(form["description"]));
		return;
	}


	// will contain path to saved photo's
	std::vector<std::string> photos;
	// get uploaded files
	std::vector<booster::shared_ptr<cppcms::http::file>> files = request().files();
	for(auto file : files) {
		// if mime is valid, save file to verify further..
		if(file::valid_image(file->mime())) {
			// generate random name
			std::string full_name = "public/cdn/product/" + crypto::random();
			// save file
			file->save_to(full_name);
			// if valid save file path
			if(file::valid_image(file::get_mime(full_name))) {
				photos.push_back(full_name);
			} else { // if not valid redirect and notify user
				response().set_redirect_header("/p/new?err=invalid_image&type=" + form["type"] + "&desc=" + cppcms::util::urlencode(form["description"]));
				return;
			}
		}
	}

	// ensure at least one image is selected for sale type
	if(form["type"] == "sale") {
		if(photos.size() < 1) { // if sale post doesn't have at least 1 image, redirect
			response().set_redirect_header("/p/new?err=need_image&type=" + form["type"] + "&desc=" + cppcms::util::urlencode(form["description"]));
			return;
		}
	}

	// If passed so far, save the product
	ProductModel product(session().get<int>("USER_ID"), form["type"], session().get("ZIPCODE"), form["description"]);
	product.save(db);

	// redirect to main page
	response().set_redirect_header("/", 302);
	return;
}

// display success message
void BaseController::reset_success() {
	session().load();
	// only allow GET method
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	// disallow logged in users
	if(Pages::logged_in(session())) {
		response().set_redirect_header("/", 302);
		return;
	}
	// display message
	Pages::set_page(&context, "RESET_LANDING");
	Pages::resolve_session(&context, session());
	render("master", context);
	return;
}

void BaseController::account_main() {
	session().load();
	// only allow GET
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	// must be logged in
	Pages::set_page(&context, "ACCOUNT_MAIN");
	if(!Pages::resolve_session(&context, session())) {
		response().set_redirect_header("/?err=need_login&next=/account", 302);
		return;
	}
	// render page
	render("master", context);
	return;
}

void BaseController::account_filters() {
	session().load();
	// only allow GET
	if(request().request_method() != "GET") {
		response().status(404);
		response().out() << "http GET is only method allowed on this page";
		return;
	}
	// must be logged in
	Pages::set_page(&context, "ACCOUNT_FILTERS");
	if(!Pages::resolve_session(&context, session())) {
		response().set_redirect_header("/?err=need_login&next=/account/filters", 302);
		return;
	}
	// render page
	render("master", context);
	return;
}

// @METHOD - Any
// @FUNCTION -	Clears the session values used to destroy authenticated sessions
void BaseController::clear_session() {
	session().load();
	session().set("LOGGED_IN", "false");
	session().set("USERNAME", "");
	session().set("USER_ID", "");
	session().set("EMAIL", "");
	session().set("SECONDARY_EMAIL", "");
	session().set("SETTINGS", "false");
	session().set("LAST_ACTIVITY", 0);
	return;
}

// @METHOD - Not used by web client
// @FUNCTION - Updates session LAST_ACTIVITY to current time since UNIX EPOCH std::time(0)
void BaseController::update_activity() {
	return;
}

// @METHOD - Any
// @FUNCTION - Clears session values and redirects the user to home page
void BaseController::logout() {
	clear_session();
	response().set_redirect_header("/", 302);
	return;
}


// Debugging
void BaseController::debug_session() {
	session().load();
	Pages::resolve_session(&context, session());
	cppcms::json::value jres;
	jres["LOGGED_IN"] = context.LOGGED_IN;
	jres["USERNAME"] = context.USERNAME;
	jres["EMAIL"] = context.EMAIL;
	jres["SECONDARY_EMAIL"] = context.SECONDARY_EMAIL;
	jres["USER_ID"] = context.USER_ID;
	jres["LAST_ACTIVITY"] = context.LAST_ACTIVITY;
	jres.save(response().out(), cppcms::json::readable);
	return;
}

void BaseController::debug_page() {
	if(Pages::logged_in(session())) {
		response().out() << "Logged in";
	} else {
		response().out() << "Not logged in";
	}
	return;
}
