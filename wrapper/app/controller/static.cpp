/*

This file is unused, Nginx provides static file
serving functions as it is far more efficient

*/

#include "static.hpp"



/** STATIC ROUTES **/

void StaticMapper::otf(std::string filename) {
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

void StaticMapper::ttf(std::string filename) {
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

void StaticMapper::js(std::string filename) {
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

void StaticMapper::css(std::string filename) {
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
void StaticMapper::png(std::string filename) {
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

void StaticMapper::bmp(std::string filename) {
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

void StaticMapper::jpeg(std::string filename) {
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

void StaticMapper::jpg(std::string filename) {
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

void StaticMapper::gif(std::string filename) {
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

void StaticMapper::html(std::string filename) {
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