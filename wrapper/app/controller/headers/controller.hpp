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
#include "api.hpp"

//#include "static.h" . - Not used, nginx serves static files


class BaseController : public cppcms::application {
	public:
		BaseController(cppcms::service &srv) : cppcms::application(srv), dbconn("dbname=dmvexchange user=db password=f9a548add9f0007ca4071e06f04e3f81 hostaddr=173.66.17.87 port=5432") {
			std::cout << "[+] Threaded..." << std::endl;
			// Map api controller
			attach(new api(srv, &dbconn), "api", "/api/{1}", "/api(/(.*))?", 1);
			/*
				@METHOD - GET

				@FUNCTION - 
					Displays home page
			*/
			dispatcher().assign("/", &BaseController::index_page, this);
			mapper().assign("");

			/*
				@METHOD - GET
				@FUNCTION -
					Create new post
			*/
			dispatcher().assign("/p/new(/)?", &BaseController::create_post, this);

			/*
				@METHOD - GET

				@FUNCTION - 
					Displays register page where user can register
					for DMV Exchange
			*/
			dispatcher().assign("/register(/)?", &BaseController::register_page, this);
			mapper().assign("register", "/register");

			/*
				@METHOD - GET

				@FUNCTION - 
					Path that is requested when user presses login button
					
					Authentication layer for login
			*/
			dispatcher().assign("/login/process(/)?", &BaseController::process_login, this);

			/*
				@METHOD - POST

				@FUNCTION -
					Path that is requested when user submits registration form
					validates form & creates database entry for account
			*/
			dispatcher().assign("/register/process(/)?", &BaseController::process_register, this);

			/*
				@METHOD - GET

				@FUNCTION -
					Generate password reset token & send to specified email
			*/
			dispatcher().assign("/forgot(/)?", &BaseController::forgot, this);

			/*
				@METHOD - POST

				@FUNCTION -
					Process forgot page request determine what data to send to
					the user based on selected option
			*/
			dispatcher().assign("/forgot/process(/)?", &BaseController::process_forgot, this);

			/*
				@METHOD - POST

				@FUNCTION -
					Success page, just notifies user that message has been sent
			*/
			dispatcher().assign("/forgot/success(/)?", &BaseController::forgot_landing, this);

			/*
				@METHOD - GET

				@FUNCTION -
					When valid token is specified, presents form that user completes
					to set a new password for themselves
			*/
			dispatcher().assign("/reset(/)?", &BaseController::reset, this);

			/*
				@METHOD - POST

				@FUNCTION -
					Process reset form
			*/
			dispatcher().assign("/reset/process(/)?", &BaseController::process_reset, this);

			/*
				@METHOD - GET

				@FUNCTION - Display successful password reset message
			*/
			dispatcher().assign("/reset/success(/)?", &BaseController::reset_landing, this);

			/*
				@METHOD - GET

				@FUNCTION - Render account page
			*/
			dispatcher().assign("/account(/)?", &BaseController::account_page, this);

			/*
				@METHOD - ANY

				@FUNCTION -
					Clears session values and redirects user to home page
			*/
			dispatcher().assign("/logout(/)?", &BaseController::logout, this);

			mapper().root("/");



			/*
				@FUNCTION - 
					For debugging and testing functions
			*/
			dispatcher().assign("/debug_session", &BaseController::debug_session, this);
			dispatcher().assign("/debug", &BaseController::debug_page, this);
		}

		~BaseController();

		pqxx::connection dbconn; // connection to primary database

		void index_page();
		void register_page();
		void process_login();
		void process_register();
		void forgot();
		void process_forgot();
		void forgot_landing();
		void reset();
		void reset_landing();
		void process_reset();
		void create_post();
		void account_page();
		void logout();

		void json_session();

		// debugging
		void debug_session();
		void debug_page();

		/** Session related **/
		void clear_session();
		void update_activity();
};

#endif