#ifndef __VIEW_H__
#define __VIEW_H__

#include <cppcms/view.h>
#include <string>

namespace dxtemplate {
	class session : public cppcms::base_content {
		public:
			// set default session values ( defaults Home page with no auth )
			session();
			~session();

			// get title
			std::string get_title();
			// return LOGGED_IN
			bool is_logged_in();
			// validates session checking last_activity
			bool eval_logged_in();

			std::string TITLE; // Page title
			
			// Page types
			// set pages to false but PAGE_HOME to true
			void reset_page();
			// set page by string ( performs match & sets to true )
			void set_page(std::string type);

			std::string PAGE;
			bool PAGE_HOME;
			bool PAGE_REGISTER;
			bool PAGE_CREATEPOST;

			bool LOGGED_IN; // is the user logged in ?
			int LAST_ACTIVITY; // when did the user last access the server ( while logged in )
			std::string USERNAME; // username ? ( only if logged in )
	};
}

#endif