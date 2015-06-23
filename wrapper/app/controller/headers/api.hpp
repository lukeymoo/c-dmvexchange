#ifndef __API_H__
#define __API_H__

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/session_interface.h>
#include "view.hpp"
#include "user.hpp"
#include "methods.hpp"

namespace json {
	void send(std::string status, std::string message, std::ostream &stream);
};

class api : public cppcms::application {
	public:
		api(cppcms::service &srv, std::shared_ptr<DatabaseClass> db_ptr, Pages::Context *context_ptr) : cppcms::application(srv) {
			db = db_ptr;
			context = context_ptr;
			// Account page -- Add new email
			dispatcher().assign("/settings/add_email(/)?", &api::settings_add_email, this);

			// Account page -- Unlocks settings page
			dispatcher().assign("/settings/unlock(/)?", &api::settings_unlock, this);

			// Account page -- Checks if settings already unlocked
			dispatcher().assign("/settings/check_auth(/)?", &api::settings_check_auth, this);

			// Account page -- Process new password request
			dispatcher().assign("/settings/change_password(/)?", &api::settings_change_password, this);

			// Account page -- block user
			dispatcher().assign("/settings/block(/)?", &api::settings_block, this);

			// Account page -- unblock user
			dispatcher().assign("/settings/unblock(/)?", &api::settings_unblock, this);

			// Account page -- get blocked list for user
			dispatcher().assign("/settings/blocked_list(/)?", &api::settings_blocked_list, this);

			// Returns session values
			dispatcher().assign("/session/state(/)?", &api::session_state, this);
		}
		~api();

		Pages::Context *context;

		std::shared_ptr<DatabaseClass> db;

		// returns session values
		void session_state();

		// add new email
		void settings_add_email();

		// unlocks account settings page
		void settings_unlock();

		// block user
		void settings_block();

		// unblock user
		void settings_unblock();

		// return blocked list of user
		void settings_blocked_list();

		// checks if user has unlocked the settings page
		void settings_check_auth();

		// processes new password request
		void settings_change_password();
};

#endif