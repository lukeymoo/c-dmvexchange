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
		BaseController(cppcms::service &srv) : cppcms::application(srv) {
			
			// Initialize connection
			db.reset(new DatabaseClass);

			std::cout << "[+] Threaded..." << std::endl;

			// Map api controller
			attach(new api(srv, db, &context), "api", "/api/{1}", "/api(/(.*)(/(.*))?(/(.*)?))?", 1);

			// @METHOD - GET
			// @FUNCTION - Displays home page
			dispatcher().assign("/", &BaseController::index_main, this);
			mapper().assign("");

			// @METHOD - GET
			// @FUNCTION - Create new post
			dispatcher().assign("/p/new(/)?", &BaseController::p_new, this);

			// @METHOD - GET
			// @FUNCTION - Displays register page where user can register
			// 				for DMV Exchange
			dispatcher().assign("/register(/)?", &BaseController::register_main, this);
			mapper().assign("register", "/register");

			// @METHOD - GET
			// @FUNCTION - Path that is requested when user presses login button
			// 				Authentication layer for login
			dispatcher().assign("/login/process(/)?", &BaseController::login_process, this);

			// @METHOD - POST
			// @FUNCTION - Path that is requested when user submits registration form
			// 				validates form & creates database entry for account
			dispatcher().assign("/register/process(/)?", &BaseController::register_process, this);

			// @METHOD - GET
			// @FUNCTION - Generate password reset token & send to specified email
			dispatcher().assign("/forgot(/)?", &BaseController::forgot_main, this);

			// @METHOD - POST
			// @FUNCTION - Process forgot page request determine what data to send to
			// 				the user based on selected option
			dispatcher().assign("/forgot/process(/)?", &BaseController::forgot_process, this);

			// @METHOD - POST
			// @FUNCTION - Success page, just notifies user that message has been sent
			dispatcher().assign("/forgot/success(/)?", &BaseController::forgot_success, this);

			// @METHOD - GET
			// @FUNCTION - When valid token is specified, presents form that user completes
			// 				to set a new password for themselves
			dispatcher().assign("/reset(/)?", &BaseController::reset_main, this);

			// @METHOD - POST
			// @FUNCTION - Process reset form
			dispatcher().assign("/reset/process(/)?", &BaseController::reset_process, this);

			// @METHOD - GET
			// @FUNCTION - Display successful password reset message
			dispatcher().assign("/reset/success(/)?", &BaseController::reset_success, this);

			// @METHOD - GET
			// @FUNCTION - Render main account page
			dispatcher().assign("/account(/)?", &BaseController::account_main, this);

			// @METHOD - GET
			// @FUNCTION - Render block/muted users account page
			dispatcher().assign("/account/filters(/)?", &BaseController::account_filters, this);

			// @METHOD - ANY
			// @FUNCTION - Clears session values and redirects user to home page
			dispatcher().assign("/logout(/)?", &BaseController::logout, this);

			mapper().root("/");



			// @FUNCTION - For debugging and testing functions
			dispatcher().assign("/debug_session", &BaseController::debug_session, this);
			dispatcher().assign("/debug", &BaseController::debug_page, this);
		}

		~BaseController();

		Pages::Context context;

		std::shared_ptr<DatabaseClass> db;

		void index_main();
		void register_main();
		void login_process();
		void register_process();
		void forgot_main();
		void forgot_process();
		void forgot_success();
		void reset_main();
		void reset_process();
		void reset_success();
		void p_new();
		void account_main();
		void account_filters();
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