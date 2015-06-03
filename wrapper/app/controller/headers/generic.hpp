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
#include <locale>
#include <stdlib.h>
#include <ctime>
#include "view.hpp"
#include "user.hpp"
#include "database.hpp"

//#include "static.h" . - Not used

/**
	Used to make it easier to label `u` field types for logins
*/
#define ID_USERNAME 	500
#define ID_EMAIL		501


// Protos
bool validUsername(std::string word);
bool validEmail(std::string word);
bool validPassword(std::string word);
std::string to_lowercase(std::string word);



class DXServer : public cppcms::application {
	public:
		DXServer(cppcms::service &srv) : cppcms::application(srv), db("dbname=dmvexchange user=db password=f9a548add9f0007ca4071e06f04e3f81 hostaddr=127.0.0.1 port=5432") {
			
			// Index page
			dispatcher().assign("/", &DXServer::index_page, this);
			mapper().assign("");

			// Register page
			dispatcher().assign("/register", &DXServer::register_page, this);
			mapper().assign("register", "/register");

			// Process register function
			// Process login function
			dispatcher().assign("/login/process", &DXServer::process_login, this);

			// Logout
			dispatcher().assign("/logout", &DXServer::logout, this);
			
			// Get Session state
			dispatcher().assign("/api/session/state", &DXServer::json_session, this);

			mapper().root("/");



			// DEBUGGING
			// session debug
			dispatcher().assign("/debug_session", &DXServer::debug_session, this);
			// regular debug
			dispatcher().assign("/debug", &DXServer::debug_page, this);
		}

		~DXServer();

		Database db;

		// index page
		void index_page();

		// register page
		void register_page();

		// json response
		void json_session();

		// process login
		void process_login();
		void logout();

		// send standard status, message json response
		void json_response(std::string status, std::string message);

		// debugging
		void debug_session();
		void debug_page();

		/** Session related **/
		void clear_session();
		void update_activity();
};

#endif