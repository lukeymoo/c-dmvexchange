#ifndef __GENERIC_H__
#define __GENERIC_H__

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/session_interface.h>
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
			// IMAGES
			mapper().assign("img", "/img/{1}");
				// img - png
				dispatcher().assign("/img/([a-zA-Z_0-9\-\.]+\.png)", &DXServer::static_png, this, 1);
				// img - bmp
				dispatcher().assign("/img/([a-zA-Z_0-9\-\.]+\.bmp)", &DXServer::static_bmp, this, 1);
				// img - jpeg
				dispatcher().assign("/img/([a-zA-Z_0-9\-\.]+\.jpeg)", &DXServer::static_jpg, this, 1);
				// img - jpg
				dispatcher().assign("/img/([a-zA-Z_0-9\-\.]+\.jpg)", &DXServer::static_jpeg, this, 1);
				// img - gif
				dispatcher().assign("/img/([a-zA-Z_0-9\-\.]+\.gif)", &DXServer::static_gif, this, 1);
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
			// html
			dispatcher().assign("/h/([a-zA-Z_0-9\.]+\.html)", &DXServer::static_html, this, 1);
			mapper().assign("h", "/h/{1}");

			
			// Index page
			dispatcher().assign("/", &DXServer::index_page, this);
			mapper().assign("");

			// Register page
			dispatcher().assign("/register", &DXServer::register_page, this);
			mapper().assign("register", "/register");

			mapper().root("/");

			// Process register function
			// Process login function
			dispatcher().assign("/login/process", &DXServer::process_login, this);
			
			// Get Session state
			dispatcher().assign("/api/session/state", &DXServer::json_session, this);
		}

		// Regex testers
		bool validUsername(std::string word);
		bool validEmail(std::string word);

		// index page
		void index_page();

		// register page
		void register_page();

		// json response
		void json_session();

		// process login
		void process_login();

		// STATIC

		// images
		void static_png(std::string filename);
		void static_bmp(std::string filename);
		void static_jpeg(std::string filename);
		void static_jpg(std::string filename);
		void static_gif(std::string filename);

		// fonts
		void static_otf(std::string filename);
		void static_ttf(std::string filename);

		// css
		void static_css(std::string filename);

		// js
		void static_js(std::string filename);

		// html
		void static_html(std::string filename);

		/** Session related **/
		void clear_session();
		void update_activity();
};

#endif