#line 1 "app/tmpl_src/layout/layout.tmpl"
#include "view.h" 
#line 2 "app/tmpl_src/layout/layout.tmpl"
namespace master {
	#line 3 "app/tmpl_src/layout/layout.tmpl"
	struct master :public cppcms::base_view
	#line 3 "app/tmpl_src/layout/layout.tmpl"
	{
	#line 3 "app/tmpl_src/layout/layout.tmpl"
		content::session &content;
	#line 3 "app/tmpl_src/layout/layout.tmpl"
		master(std::ostream &_s,content::session &_content): cppcms::base_view(_s),content(_content)
	#line 3 "app/tmpl_src/layout/layout.tmpl"
		{
	#line 3 "app/tmpl_src/layout/layout.tmpl"
		}
		#line 5 "app/tmpl_src/layout/layout.tmpl"
		virtual void render() {
			#line 9 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"<!doctype html>\n"
				"<html>\n"
				"<head>\n"
				"\t<title>DX &#8212; ";
			#line 9 "app/tmpl_src/layout/layout.tmpl"
			out()<<cppcms::filters::escape(content.TITLE);
			#line 14 "app/tmpl_src/layout/layout.tmpl"
			out()<<"</title>\n"
				"\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
				"\t<link rel='stylesheet' type='text/css' href='/css/main.css?v=1'>\n"
				"\t<link rel='stylesheet' type='text/css' href='/css/normalize.css?v=1'>\n"
				"\t<link rel='stylesheet' type='text/css' href='/css/common.css?v=1'>\n"
				"\t";
			#line 14 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "REGISTER") {
				#line 16 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<link rel='stylesheet' type='text/css' href='/css/register.css?v=1'>\n"
					"\t";
			#line 16 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 20 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"</head>\n"
				"<body>\n"
				"\t<div id='wrapper'>\n"
				"\t\t";
			#line 20 "app/tmpl_src/layout/layout.tmpl"
			header();
			#line 28 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t<div id='body-wrapper'>\n"
				"\t\t\tHello world\n"
				"\t\t</div>\n"
				"\t</div>\n"
				"\t<!-- JScripts -->\n"
				"\t<script src='https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script>\n"
				"\t<script src='/js/common.js'></script>\n"
				"\t";
			#line 28 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "HOME") {
				#line 30 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/market.js'></script>\n"
					"\t";
			#line 30 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 31 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 31 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "CREATEPOST") {
				#line 33 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/market.js'></script>\n"
					"\t";
			#line 33 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 34 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 34 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "REGISTER") {
				#line 36 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/register.js'></script>\n"
					"\t";
			#line 36 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 39 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"</body>\n"
				"</html>\n"
				"";
		#line 39 "app/tmpl_src/layout/layout.tmpl"
		} // end of template render
		#line 42 "app/tmpl_src/layout/layout.tmpl"
		virtual void header() {
			#line 45 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"<div id='header-bar'>\n"
				"\t<span id='header-menu-button'>&#9776;</span>\n"
				"\t";
			#line 45 "app/tmpl_src/layout/layout.tmpl"
			if(content.LOGGED_IN == true) {
				#line 48 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<a href='/'><span id='header-logo' class='logo-with-login'> DMV EXCHANGE </span></a>\n"
					"\t\t<span id='beta-logo' class='beta-with-login'>*BETA</span>\n"
					"\t";
			#line 48 "app/tmpl_src/layout/layout.tmpl"
			}else{
				#line 51 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<a href='/'><span id='header-logo' class='logo-no-login'> DMV EXCHANGE </span></a>\n"
					"\t\t<span id='beta-logo' class='beta-no-login'>*BETA</span>\n"
					"\t";
			#line 51 "app/tmpl_src/layout/layout.tmpl"
			}
			#line 57 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<div id='header-menu'>\n"
				"\t\t<a href='/'>\n"
				"\t\t\t<span class='header-menu-option'>Home</span>\n"
				"\t\t</a>\n"
				"\t\t<span class='header-menu-option'>Submit advice</span>\n"
				"\t\t";
			#line 57 "app/tmpl_src/layout/layout.tmpl"
			if(content.LOGGED_IN == true) {
				#line 60 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t<a href=\"/account\"><span class='header-menu-option'>Account</span></a>\n"
					"\t\t\t<a href=\"/login/logout\"><span class=\"header-menu-option\">Logout</span></a>\n"
					"\t\t";
			#line 60 "app/tmpl_src/layout/layout.tmpl"
			}else{
				#line 64 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t<a href='/register'>\n"
					"\t\t\t\t<span class='header-menu-option'>Sign up</span>\n"
					"\t\t\t</a>\n"
					"\t\t";
			#line 64 "app/tmpl_src/layout/layout.tmpl"
			}
			#line 67 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t</div>\n"
				"\t<!-- Header controls -->\n"
				"\t";
			#line 67 "app/tmpl_src/layout/layout.tmpl"
			if(content.LOGGED_IN == true) {
				#line 69 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<div id='header-controls'>\n"
					"\t\t\t";
				#line 69 "app/tmpl_src/layout/layout.tmpl"
				if(content.PAGE == "CREATEPOST") {
					#line 71 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t<span data-active='true' id='header-create-post-button' class='header-control-button'>Create Post</span>\n"
						"\t\t\t";
				#line 71 "app/tmpl_src/layout/layout.tmpl"
				}else{
					#line 73 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t<a href='/p/new'><span data-active='false' id='header-create-post-button' class='header-control-button'>Create Post</span></a>\n"
						"\t\t\t";
				#line 73 "app/tmpl_src/layout/layout.tmpl"
				}
				#line 76 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\n"
					"\t\t\t<span data-active='false' id='header-notifications-button' class='header-control-button'>Notifications</span>\n"
					"\t\t\t<span data-active='false' id='header-profile-button' class='header-control-button'>";
				#line 76 "app/tmpl_src/layout/layout.tmpl"
				out()<<cppcms::filters::escape(content.USERNAME);
				#line 78 "app/tmpl_src/layout/layout.tmpl"
				out()<<"</span>\n"
					"\t\t</div>\n"
					"\t";
			#line 78 "app/tmpl_src/layout/layout.tmpl"
			}else{
				#line 82 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t<div id='header-controls'>\n"
					"\t\t<span id='header-login-button' data-active='false' class='header-control-button' data-state='closed'>Log in</span>\n"
					"\t\t<a href='/register'>\n"
					"\t\t\t";
				#line 82 "app/tmpl_src/layout/layout.tmpl"
				if(content.PAGE == "REGISTER") {
					#line 84 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t<span data-active='true' class='header-control-button'>Register</span>\n"
						"\t\t\t";
				#line 84 "app/tmpl_src/layout/layout.tmpl"
				}else{
					#line 86 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t<span data-active='false' class='header-control-button'>Register</span>\n"
						"\t\t\t";
				#line 86 "app/tmpl_src/layout/layout.tmpl"
				}
				#line 89 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t</a>\n"
					"\t</div>\n"
					"\t";
			#line 89 "app/tmpl_src/layout/layout.tmpl"
			}
			#line 92 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<!-- End header controls -->\n"
				"\t<!-- Login form -->\n"
				"\t";
			#line 92 "app/tmpl_src/layout/layout.tmpl"
			if(content.LOGGED_IN == false) {
				#line 102 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<div id='header-login-container'>\n"
					"\t\t\t<form id='header-login-form'>\n"
					"\t\t\t\t<label>Username or email</label>\n"
					"\t\t\t\t<input id='username-or-email' type='text' placeholder='Username or email'>\n"
					"\t\t\t\t<input id='password' type='password' placeholder='Password'>\n"
					"\t\t\t\t<a href=\"/forgot\"><span id='header-login-forgot'>forgot password?</span></a>\n"
					"\t\t\t</form>\n"
					"\t\t\t<button id='header-login-form-button'>Log in</button>\n"
					"\t\t</div>\n"
					"\t";
			#line 102 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 106 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<!-- End login form -->\n"
				"</div>\n"
				"<div id='notification-container'></div>\n"
				"";
		#line 106 "app/tmpl_src/layout/layout.tmpl"
		} // end of template header
	#line 108 "app/tmpl_src/layout/layout.tmpl"
	}; // end of class master
#line 109 "app/tmpl_src/layout/layout.tmpl"
} // end of namespace master
#line 109 "app/tmpl_src/layout/layout.tmpl"
namespace {
#line 109 "app/tmpl_src/layout/layout.tmpl"
 cppcms::views::generator my_generator; 
#line 109 "app/tmpl_src/layout/layout.tmpl"
 struct loader { 
#line 109 "app/tmpl_src/layout/layout.tmpl"
  loader() { 
#line 109 "app/tmpl_src/layout/layout.tmpl"
   my_generator.name("master");
#line 109 "app/tmpl_src/layout/layout.tmpl"
   my_generator.add_view<master::master,content::session>("master",true);
#line 109 "app/tmpl_src/layout/layout.tmpl"
    cppcms::views::pool::instance().add(my_generator);
#line 109 "app/tmpl_src/layout/layout.tmpl"
 }
#line 109 "app/tmpl_src/layout/layout.tmpl"
 ~loader() {  cppcms::views::pool::instance().remove(my_generator); }
#line 109 "app/tmpl_src/layout/layout.tmpl"
} a_loader;
#line 109 "app/tmpl_src/layout/layout.tmpl"
} // anon 
