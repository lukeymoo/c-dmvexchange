#ifndef __VIEW_H__
#define __VIEW_H__

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/session_interface.h>
#include <cppcms/json.h>
#include <cppcms/view.h>
#include <string>

namespace Pages {

	struct Context : public cppcms::base_content {
		public:
			std::string TITLE; // Page title
			std::string PAGE; // page ID

			std::string LOGGED_IN;
			std::string USERNAME;
			std::string EMAIL;
			std::string SECONDARY_EMAIL;
			std::string USER_ID;
			std::string SETTINGS;
			int LAST_ACTIVITY;
	};

	// set page id
	void set_page(Context *context, std::string page_id);

	// return LOGGED_IN
	bool logged_in(cppcms::session_interface &interface);

	// get session values into context for rendering
	// returns true/false if logged in / logged out
	bool resolve_session(Context *context, cppcms::session_interface &interface);

};

#endif