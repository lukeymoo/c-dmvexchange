#ifndef __VIEW_H__
#define __VIEW_H__

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/session_interface.h>
#include <cppcms/view.h>
#include <string>

namespace dxtemplate {
	class context : public cppcms::base_content {
		public:
			// set default session values ( defaults Home page with no auth )
			context();
			~context();

			// get title
			std::string get_title();
			// return LOGGED_IN
			bool is_logged_in(cppcms::session_interface &interface);
			// validates session checking last_activity
			bool eval_logged_in();

			void resolve_session(cppcms::session_interface &interface);


			void set_page(std::string page_id);

			std::string TITLE; // Page title
			std::string PAGE; // page ID
			std::string LOGGED_IN;
			int LAST_ACTIVITY;
			std::string USERNAME;
	};
}

#endif