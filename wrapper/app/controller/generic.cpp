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
	dxtemplate::session c;
	c.set_page("HOME");
	c.LOGGED_IN = false;
	c.USERNAME = "";
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
	dxtemplate::session c;

	c.set_page("REGISTER");
	c.LOGGED_IN = false;
	c.USERNAME = "";
	render("master", c);
	return;
}

void DXServer::process_login() {
	// Make sure the user is not already logged in
	if(session().is_set("LOGGED_IN")) {
		if(session()["LOGGED_IN"].compare("true") == 0) {
			response().out() << "Already logged in";
			return;
		}
	}
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

	cppcms::json::value state;

	if(session().is_set("LOGGED_IN")) {
		if(!session()["LOGGED_IN"].empty()) {
			state["LOGGED_IN"] = session()["LOGGED_IN"];
			state["USERNAME"] = session()["USERNAME"];
			// Update last activity
			update_activity();
		} else {
			clear_session();
			state["LOGGED_IN"] = false;
			state["USERNAME"] = "";
		}
	} else {
		clear_session();
		state["LOGGED_IN"] = false;
		state["USERNAME"] = "";
	}

	response().out() << state;
	return;
}

void DXServer::clear_session() {
	session().set("LOGGED_IN", false);
	session().set("USERNAME", "");
	session().set("USER_ID", "");
	session().set("LAST_ACTIVITY", 0);
	return;
}

void DXServer::update_activity() {
	return;
}















/** Helper **/
bool DXServer::validUsername(std::string word) {
	bool status = false;
	return status;
}

bool DXServer::validEmail(std::string word) {
	bool status = false;
	return status;
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
	} else {
		// read to client
		response().content_type("text/html");
		response().out() << file.rdbuf();
		// close file
		file.close();
	}
	return;
}