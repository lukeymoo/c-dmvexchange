#ifndef __GENERIC_H__
#define __GENERIC_H__

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "view.h"

class DXServer : public cppcms::application {
	public:
		DXServer(cppcms::service(&srv)) : cppcms::application(srv) {

			/** STATIC **/
			// FONTS
			mapper().assign("font", "/font/{1}");
				// font - otf
				dispatcher().assign("/font/([a-zA-Z_0-9\-\.]+\.otf)", &DXServer::static_otf, this, 1);
				// font - ttf
				dispatcher().assign("/font/([a-zA-Z_0-9\-\.]+\.ttf)", &DXServer::static_ttf, this, 1);
			// js
			dispatcher().assign("/js/([a-zA-Z_0-9\.]+\.js)", &DXServer::static_js, this, 1);
			mapper().assign("js", "/js/{1}");
			// css
			dispatcher().assign("/css/([a-zA-Z_0-9\.]+\.css)", &DXServer::static_css, this, 1);
			mapper().assign("css", "/css/{1}");

			
			// Index page
			dispatcher().assign("/", &DXServer::index_page, this);
			mapper().assign("");

			mapper().root("/");

			// Register page
			dispatcher().assign("/register", &DXServer::register_page, this);
			mapper().assign("register", "/register");

			// Process register function
			// Process login function
		}

		// index page
		void index_page();

		// register page
		void register_page();

		// static files
		void static_otf(std::string filename);
		void static_ttf(std::string filename);
		void static_css(std::string filename);
		void static_js(std::string filename);
};

#endif