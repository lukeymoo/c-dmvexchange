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




#include "generic.h"
#include "view.h"
#include "user.h"

void DXServer::index_page() {
	// only allow get method
	if(request().request_method().compare("GET") != 0) {
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
	if(request().request_method().compare("GET") != 0) {
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
	if(request().request_method().compare("POST") != 0) {
		response().status(404);
		response().out() << "http POST is only method allowed on this page";
		return;
	}
	session().load();
	// Make sure the user is not already logged in
	if(session().is_set("LOGGED_IN")) {
		if(session()["LOGGED_IN"].compare("true") == 0) {
			response().out() << "Already logged in";
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
			cppcms::json::value jres;
			jres["status"] = "DX-REJECTED";
			jres["message"] = "u_invalid";
			jres.save(response().out(), cppcms::json::compact);
			return;
		}
	}
	// is valid password
	if(!validPassword(request().post("p"))) {
		// bad password
		cppcms::json::value jres;
		jres["status"] = "DX-REJECTED";
		jres["message"] = "P";
		jres.save(response().out(), cppcms::json::compact);
		return;
	}
	// query server to determine if user exists
	cppcms::json::value jres;
	jres["status"] = "DX-FAILED";
	jres["message"] = "server backend is under construction";
	jres.save(response().out(), cppcms::json::compact);
	return;
}


/** JSON RESPONSE ROUTES **/
void DXServer::json_session() {
	// only allow get
	if(request().request_method().compare("GET") != 0) {
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
	jres.save(response().out(), cppcms::json::compact);
	return;
}

void DXServer::logout() {
	clear_session();
	response().status(302);
	response().set_header("Location", "/");
	return;
}

void DXServer::clear_session() {
	session().load();
	session().set("LOGGED_IN", "false");
	session().set("USERNAME", "");
	session().set("USER_ID", "");
	session().set("LAST_ACTIVITY", 0);
	return;
}

void DXServer::update_activity() {
	return;
}


void DXServer::debug_session() {
	session().load();
	dxtemplate::context c;
	c.resolve_session(session());
	cppcms::json::value jres;
	jres["LOGGED_IN"] = c.LOGGED_IN;
	jres["USERNAME"] = c.USERNAME;
	jres.save(std::cout, cppcms::json::readable);
	response().out() << "check console";
	return;
}

void DXServer::debug_page() {
	response().out() << "Nothing to debug!";
	return;
}



/*

function validateEmail(string) {
	return (/^([a-zA-Z0-9_.+-])+\@(([a-zA-Z0-9-])+\.)+([a-zA-Z0-9]{2,4})+$/.test(string)
		&& string.length <= 64) ? true : false;
}

function validateUsername(string) {
	return (/^[A-Za-z0-9_]+$/.test(string)
		&& string.length >= 2
		&& string.length <= 16) ? true : false;
}


*/










/** Helpers **/
/*

Username Allowed Characters

A-Z => 65 -> 90
a-z => 97 -> 122
0-9 => 48 -> 57
_ 	=> 95 		( underscore character )

*/
bool DXServer::validUsername(std::string word) {
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
bool DXServer::validEmail(std::string word) {
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
bool DXServer::validPassword(std::string word) {
	if(word.length() < 2 || word.length() > 32) {
		return false;
	}
	return true;
}

/**
	Lowercase characters in string
*/
std::string DXServer::to_lowercase(std::string word) {
	std::locale loc;
	std::stringstream ss;
	for(std::string::size_type i = 0; i < word.length(); i++) {
		ss << std::tolower(word[i], loc);
	}
	return ss.str();
}





/** STATIC ROUTES **/

void DXServer::static_otf(std::string filename) {
	std::stringstream ss;
	ss << "public/font/" << filename;
	std::string final_name = ss.str();

	std::ifstream file(final_name.c_str(), std::ifstream::in);
	if(!file) {
		std::cout << "Failed to server font .otf => " << final_name << std::endl;
		response().status(404);
		response().out() << "Server error 404 - file not found";
	} else {
		// prepare headers and write to client
		response().content_type("application/font-sfnt");
		response().out() << file.rdbuf();

		// close handle
		file.close();
	}

	return;
}

void DXServer::static_ttf(std::string filename) {
	std::stringstream ss;
	ss << "public/font/" << filename;
	std::string final_name = ss.str();

	std::ifstream file(final_name.c_str(), std::ifstream::in);
	if(!file) {
		std::cout << "Failed to server font .ttf => " << final_name << std::endl;
		response().status(404);
		response().out() << "Server error 404 - file not found";
	} else {
		// prepare headers and write to client
		response().content_type("application/x-font-tff");
		response().out() << file.rdbuf();

		// close handle
		file.close();
	}

	return;
}

void DXServer::static_js(std::string filename) {
	std::stringstream ss;
	ss << "public/js/" << filename;
	std::string final_name = ss.str();

	// open file
	std::ifstream file(final_name.c_str(), std::ifstream::in);
	if(!file) {
		std::cout << "failed to server script .js => " << final_name << std::endl;
		response().status(404);
		response().out() << "Server error 404 - file not found";
	} else {
		// write to client
		response().content_type("application/javascript");
		response().out() << file.rdbuf();
		// close client
		file.close();
	}
	return;
}

void DXServer::static_css(std::string filename) {
	// concat filename to public/css
	std::stringstream ss;
	ss << "public/css/" << filename;
	std::string final_name = ss.str();

	// open filename
	std::ifstream file;
	file.open(final_name.c_str(), std::fstream::in);
	// if failed to open file
	if(!file) {
		std::cout << "failed to serve stylesheet .css => " << final_name << std::endl;
		response().status(404);
		response().out() << "Server error 404 - file not found";
	} else {
		// read to client
		response().content_type("text/css");
		response().out() << file.rdbuf();
		
		// close
		file.close();
	}
	return;
}

// FONTS
void DXServer::static_png(std::string filename) {
	// concat filename to public/img/
	std::stringstream ss;
	ss << "public/img/" << filename;
	std::string final_name = ss.str();
	std::ifstream file(final_name.c_str(), std::ifstream::in);
	if(!file) {
		std::cout << "failed to serve image .png => " << final_name << std::endl;
		response().status(404);
		response().out() << "Server error 404 - file not found";
	} else {
		// read to client
		response().content_type("image/png");
		response().out() << file.rdbuf();
		// close file
		file.close();
	}
	return;
}

void DXServer::static_bmp(std::string filename) {
	// concat filename to public/img/
	std::stringstream ss;
	ss << "public/img/" << filename;
	std::string final_name = ss.str();
	std::ifstream file(final_name.c_str(), std::ifstream::in);
	if(!file) {
		std::cout << "failed to serve image .bmp => " << final_name << std::endl;
		response().status(404);
		response().out() << "Server error 404 - file not found";
	} else {
		// read to client
		response().content_type("image/bmp");
		response().out() << file.rdbuf();
		// close file
		file.close();
	}
	return;
}

void DXServer::static_jpeg(std::string filename) {
	// concat filename to public/img/
	std::stringstream ss;
	ss << "public/img/" << filename;
	std::string final_name = ss.str();
	std::ifstream file(final_name.c_str(), std::ifstream::in);
	if(!file) {
		std::cout << "failed to serve image .jpeg => " << final_name << std::endl;
		response().status(404);
		response().out() << "Server error 404 - file not found";
	} else {
		// read to client
		response().content_type("image/jpeg");
		response().out() << file.rdbuf();
		// close file
		file.close();
	}
	return;
}

void DXServer::static_jpg(std::string filename) {
	// concat filename to public/img/
	std::stringstream ss;
	ss << "public/img/" << filename;
	std::string final_name = ss.str();
	std::ifstream file(final_name.c_str(), std::ifstream::in);
	if(!file) {
		std::cout << "failed to serve image .jpg => " << final_name << std::endl;
		response().status(404);
		response().out() << "Server error 404 - file not found";
	} else {
		// read to client
		response().content_type("image/jpeg");
		response().out() << file.rdbuf();
		// close file
		file.close();
	}
	return;
}

void DXServer::static_gif(std::string filename) {
	// concat filename to public/img/
	std::stringstream ss;
	ss << "public/img/" << filename;
	std::string final_name = ss.str();
	std::ifstream file(final_name.c_str(), std::ifstream::in);
	if(!file) {
		std::cout << "failed to serve image .gif => " << final_name << std::endl;
		response().status(404);
		response().out() << "Server error 404 - file not found";
	} else {
		// read to client
		response().content_type("image/gif");
		response().out() << file.rdbuf();
		// close file
		file.close();
	}
	return;
}

void DXServer::static_html(std::string filename) {
	// concat filename with public/h/
	std::stringstream ss;
	ss << "public/h/" << filename;
	std::string final_name = ss.str();
	// open file
	std::ifstream file(final_name.c_str(), std::ifstream::in);
	if(!file) {
		std::cout << "failed to serve document .html => " << final_name << std::endl;
		response().status(404);
		response().out() << "Server error 404 - file not found";
	} else {
		// read to client
		response().content_type("text/html");
		response().out() << file.rdbuf();
		// close file
		file.close();
	}
	return;
}