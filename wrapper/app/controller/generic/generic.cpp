#include "generic.h"
#include "view.h"
#include "user.h"

void DXServer::index_page() {
	content::session c;
	c.TITLE = "Home";
	c.PAGE = "HOME";
	c.LOGGED_IN = false;
	c.USERNAME = "";
	render("master", c);
	return;
}

void DXServer::register_page() {
	response().out() << "This route is incomplete";
	return;
}

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