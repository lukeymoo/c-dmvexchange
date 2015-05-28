#ifndef __VIEW_H__
#define __VIEW_H__

#include <cppcms/view.h>
#include <string>

namespace content {
	struct session : public cppcms::base_content {
		/** General page context variables **/
		std::string TITLE;
		std::string PAGE;
		bool LOGGED_IN;
		std::string USERNAME;
	};
}

#endif