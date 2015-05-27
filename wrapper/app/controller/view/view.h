#ifndef __VIEW_H__
#define __VIEW_H__

#include <cppcms/view.h>
#include <string>

namespace content {
	struct basic : public cppcms::base_content {
		/** General page context variables **/
		std::string TITLE;
		std::string PAGE;
	};

	struct session : public basic {
		/** Session related **/
		bool LOGGED_IN;
		std::string USERNAME;
	};
}

#endif