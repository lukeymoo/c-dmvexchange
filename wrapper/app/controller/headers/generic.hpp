#ifndef __GENERIC_H__
#define __GENERIC_H__

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/session_interface.h>
#include "view.hpp"
#include "database.hpp"

//#include "static.h" . - Not used


class DXServer : public cppcms::application {
	public:
		DXServer(cppcms::service &srv) : cppcms::application(srv), dbconn("dbname=dmvexchange user=db password=f9a548add9f0007ca4071e06f04e3f81 hostaddr=127.0.0.1 port=5432") {
			
			/*
				@METHOD - GET

				@FUNCTION - 
					Displays home page
			*/
			dispatcher().assign("/", &DXServer::index_page, this);
			mapper().assign("");

			/*
				@METHOD - GET

				@FUNCTION - 
					Displays register page where user can register
					for DMV Exchange
			*/
			dispatcher().assign("/register", &DXServer::register_page, this);
			mapper().assign("register", "/register");

			/*
				@METHOD - GET

				@FUNCTION - 
					Path that is requested when user presses login button
					
					Authentication layer for login
			*/
			dispatcher().assign("/login/process", &DXServer::process_login, this);

			/*
				@METHOD - ANY

				@FUNCTION -
					Clears session values and redirects user to home page
			*/
			dispatcher().assign("/logout", &DXServer::logout, this);
			
			/*
				@METHOD - GET

				@FUNCTION -
					For use by in browser scripts
					to retrieve session values
			*/
			dispatcher().assign("/api/session/state", &DXServer::json_session, this);

			mapper().root("/");



			/*
				@FUNCTION - 
					For debugging and testing functions
			*/
			dispatcher().assign("/debug_session", &DXServer::debug_session, this);
			dispatcher().assign("/debug", &DXServer::debug_page, this);
		}

		~DXServer();

		pqxx::connection dbconn;

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