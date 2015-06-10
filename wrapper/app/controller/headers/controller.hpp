#ifndef __GENERIC_H__
#define __GENERIC_H__

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <sstream>
#include "view.hpp"
#include "user.hpp"

//#include "static.h" . - Not used, nginx serves static files


class DXServer : public cppcms::application {
	public:
		DXServer(cppcms::service &srv) : cppcms::application(srv), dbconn("dbname=dmvexchange user=db password=f9a548add9f0007ca4071e06f04e3f81 hostaddr=127.0.0.1 port=5432") {
			std::cout << "[+] Threaded..." << std::endl;
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
			dispatcher().assign("/register(/)?", &DXServer::register_page, this);
			mapper().assign("register", "/register");

			/*
				@METHOD - GET

				@FUNCTION - 
					Path that is requested when user presses login button
					
					Authentication layer for login
			*/
			dispatcher().assign("/login/process(/)?", &DXServer::process_login, this);

			/*
				@METHOD - POST

				@FUNCTION -
					Path that is requested when user submits registration form
					validates form & creates database entry for account
			*/
			dispatcher().assign("/register/process(/)?", &DXServer::process_register, this);

			/*
				@METHOD - GET

				@FUNCTION -
					Generate password reset token & send to specified email
			*/
			dispatcher().assign("/forgot(/)?", &DXServer::forgot, this);

			/*
				@METHOD - POST

				@FUNCTION -
					Process forgot page request determine what data to send to
					the user based on selected option
			*/
			dispatcher().assign("/forgot/process(/)?", &DXServer::process_forgot, this);

			/*
				@METHOD - POST

				@FUNCTION -
					Success page, just notifies user that message has been sent
			*/
			dispatcher().assign("/forgot/success(/)?", &DXServer::forgot_landing, this);

			/*
				@METHOD - GET

				@FUNCTION -
					When valid token is specified, presents form that user completes
					to set a new password for themselves
			*/
			dispatcher().assign("/reset(/)?", &DXServer::reset, this);

			/*
				@METHOD - POST

				@FUNCTION -
					Process reset form
			*/
			dispatcher().assign("/reset/process(/)?", &DXServer::process_reset, this);

			/*
				@METHOD - ANY

				@FUNCTION -
					Clears session values and redirects user to home page
			*/
			dispatcher().assign("/logout(/)?", &DXServer::logout, this);
			
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

		pqxx::connection dbconn; // connection to primary database

		void index_page();
		void register_page();
		void process_login();
		void process_register();
		void forgot();
		void process_forgot();
		void forgot_landing();
		void reset();
		void process_reset();
		void logout();

		// send standard status, message json response
		void json_response(std::string status, std::string message);
		void json_session();

		// debugging
		void debug_session();
		void debug_page();

		/** Session related **/
		void clear_session();
		void update_activity();
};

#endif