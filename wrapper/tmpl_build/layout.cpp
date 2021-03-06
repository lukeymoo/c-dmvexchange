#line 1 "app/tmpl_src/layout/layout.tmpl"
#include "view.hpp" 
#line 2 "app/tmpl_src/layout/layout.tmpl"
namespace layout {
	#line 3 "app/tmpl_src/layout/layout.tmpl"
	struct master :public cppcms::base_view
	#line 3 "app/tmpl_src/layout/layout.tmpl"
	{
	#line 3 "app/tmpl_src/layout/layout.tmpl"
		Pages::Context &content;
	#line 3 "app/tmpl_src/layout/layout.tmpl"
		master(std::ostream &_s,Pages::Context &_content): cppcms::base_view(_s),content(_content)
	#line 3 "app/tmpl_src/layout/layout.tmpl"
		{
	#line 3 "app/tmpl_src/layout/layout.tmpl"
		}
		#line 6 "app/tmpl_src/layout/layout.tmpl"
		virtual void header() {
			#line 9 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<div id='header-bar'>\n"
				"\t\t<span id='header-menu-button'>&#9776;</span>\n"
				"\t\t";
			#line 9 "app/tmpl_src/layout/layout.tmpl"
			if(content.LOGGED_IN == "true") {
				#line 12 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t<a href='/'><span id='header-logo' class='logo-with-login'> DMV EXCHANGE </span></a>\n"
					"\t\t\t<span id='beta-logo' class='beta-with-login'>*BETA</span>\n"
					"\t\t";
			#line 12 "app/tmpl_src/layout/layout.tmpl"
			}else{
				#line 15 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t<a href='/'><span id='header-logo' class='logo-no-login'> DMV EXCHANGE </span></a>\n"
					"\t\t\t<span id='beta-logo' class='beta-no-login'>*BETA</span>\n"
					"\t\t";
			#line 15 "app/tmpl_src/layout/layout.tmpl"
			}
			#line 20 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t<div id='header-menu'>\n"
				"\t\t\t<a href='/'>\n"
				"\t\t\t\t<span class='header-menu-option'>Home</span>\n"
				"\t\t\t</a>\n"
				"\t\t\t";
			#line 20 "app/tmpl_src/layout/layout.tmpl"
			if(content.LOGGED_IN == "true") {
				#line 22 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t<span id='header-submit-advice' class='header-menu-option'>Submit advice</span>\n"
					"\t\t\t";
			#line 22 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 23 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 23 "app/tmpl_src/layout/layout.tmpl"
			if(content.LOGGED_IN == "true") {
				#line 26 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t<a href=\"/account\"><span class='header-menu-option'>Account</span></a>\n"
					"\t\t\t\t<span id='header-logout-button' class=\"header-menu-option\">Logout</span>\n"
					"\t\t\t";
			#line 26 "app/tmpl_src/layout/layout.tmpl"
			}else{
				#line 30 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t<a href='/register'>\n"
					"\t\t\t\t\t<span class='header-menu-option'>Sign up</span>\n"
					"\t\t\t\t</a>\n"
					"\t\t\t";
			#line 30 "app/tmpl_src/layout/layout.tmpl"
			}
			#line 33 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t</div>\n"
				"\t\t<!-- Header controls -->\n"
				"\t\t";
			#line 33 "app/tmpl_src/layout/layout.tmpl"
			if(content.LOGGED_IN == "true") {
				#line 35 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t<div id='header-controls'>\n"
					"\t\t\t\t";
				#line 35 "app/tmpl_src/layout/layout.tmpl"
				if(content.PAGE == "CREATEPOST") {
					#line 37 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t\t<span data-active='true' id='header-create-post-button' class='header-control-button'>Create Post</span>\n"
						"\t\t\t\t";
				#line 37 "app/tmpl_src/layout/layout.tmpl"
				}else{
					#line 39 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t\t<a href='/p/new'><span data-active='false' id='header-create-post-button' class='header-control-button'>Create Post</span></a>\n"
						"\t\t\t\t";
				#line 39 "app/tmpl_src/layout/layout.tmpl"
				}
				#line 42 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\n"
					"\t\t\t\t<span data-active='false' id='header-notifications-button' class='header-control-button'>Notifications</span>\n"
					"\t\t\t\t<span data-active='false' id='header-profile-button' class='header-control-button'>";
				#line 42 "app/tmpl_src/layout/layout.tmpl"
				out()<<cppcms::filters::escape(content.USERNAME);
				#line 44 "app/tmpl_src/layout/layout.tmpl"
				out()<<"</span>\n"
					"\t\t\t</div>\n"
					"\t\t";
			#line 44 "app/tmpl_src/layout/layout.tmpl"
			}else{
				#line 48 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<div id='header-controls'>\n"
					"\t\t\t<span id='header-login-button' data-active='false' class='header-control-button' data-state='closed'>Log in</span>\n"
					"\t\t\t<a href='/register'>\n"
					"\t\t\t\t";
				#line 48 "app/tmpl_src/layout/layout.tmpl"
				if(content.PAGE == "REGISTER") {
					#line 50 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t\t<span data-active='true' class='header-control-button'>Register</span>\n"
						"\t\t\t\t";
				#line 50 "app/tmpl_src/layout/layout.tmpl"
				}else{
					#line 52 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t\t<span data-active='false' class='header-control-button'>Register</span>\n"
						"\t\t\t\t";
				#line 52 "app/tmpl_src/layout/layout.tmpl"
				}
				#line 55 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t</a>\n"
					"\t\t</div>\n"
					"\t\t";
			#line 55 "app/tmpl_src/layout/layout.tmpl"
			}
			#line 58 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t<!-- End header controls -->\n"
				"\t\t<!-- Login form -->\n"
				"\t\t";
			#line 58 "app/tmpl_src/layout/layout.tmpl"
			if(content.LOGGED_IN == "false") {
				#line 68 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t<div id='header-login-container'>\n"
					"\t\t\t\t<form id='header-login-form'>\n"
					"\t\t\t\t\t<label>Username or email</label>\n"
					"\t\t\t\t\t<input id='username-or-email' type='text' spellcheck='false' placeholder='Username or email'>\n"
					"\t\t\t\t\t<input id='password' type='password' placeholder='Password'>\n"
					"\t\t\t\t\t<a href=\"/forgot\"><span id='header-login-forgot'>forgot password?</span></a>\n"
					"\t\t\t\t</form>\n"
					"\t\t\t\t<button id='header-login-form-button'>Log in</button>\n"
					"\t\t\t</div>\n"
					"\t\t";
			#line 68 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 72 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t<!-- End login form -->\n"
				"\t</div>\n"
				"\t<div id='notification-container'></div>\n"
				"";
		#line 72 "app/tmpl_src/layout/layout.tmpl"
		} // end of template header
		#line 74 "app/tmpl_src/layout/layout.tmpl"
		virtual void footer() {
			#line 81 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<div id='footer-container'>\n"
				"\t\t<div id='footer-logo-container'>\n"
				"\t\t\t<span id='footer-logo' class='logo-no-login'> DMV EXCHANGE </span>\n"
				"\t\t\t<span id='footer-beta-logo' class='beta-no-login'>*BETA</span>\n"
				"\t\t</div>\n"
				"\t</div>\n"
				"";
		#line 81 "app/tmpl_src/layout/layout.tmpl"
		} // end of template footer
		#line 83 "app/tmpl_src/layout/layout.tmpl"
		virtual void create_post_form() {
			#line 127 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<div data-visible='true' id='post-form-container'>\n"
				"\t\t<form action='/p/process' method='POST' enctype='multipart/form-data' name='postform' id='post-form' data-type='sale' data-valid='false'>\n"
				"\t\t\t<img id='post-form-validator' src='/img/cross.png'>\n"
				"\t\t\t<input type='text' name='producttype' class='hidden-input' value='shoe' id='producttype'>\n"
				"\t\t\t<input type='text' name='posttype' class='hidden-input' value='sale' id='posttype'>\n"
				"\t\t\t<label>What kind of post is this?</label>\n"
				"\t\t\t<div id='post-type-container'>\n"
				"\t\t\t\t<span data-selected='true' data-value='sale' id='post-type-left' class='post-type'>Sale</span>\n"
				"\t\t\t\t<span data-selected='false' data-value='general' id='post-type-right' class='post-type'>General Post</span>\n"
				"\t\t\t</div>\n"
				"\t\t\t<label>Post description</label>\n"
				"\t\t\t<textarea name='postdescription' id='post-form-description'></textarea>\n"
				"\t\t\t<input type='file' data-for='#handler-container1' id='photo1' name='photo1' class='post-image hidden-input'>\n"
				"\t\t\t<input type='file' data-for='#handler-container2' id='photo2' name='photo2' class='post-image hidden-input'>\n"
				"\t\t\t<input type='file' data-for='#handler-container3' id='photo3' name='photo3' class='post-image hidden-input'>\n"
				"\t\t\t<input type='file' data-for='#handler-container4' id='photo4' name='photo4' class='post-image hidden-input'>\n"
				"\t\t\t<label id='post-image-label'>Sales <span style=\"color:rgb(190, 0, 0);\">must</span> have at least 1 image</label>\n"
				"\t\t\t<!-- Photo Handlers -->\n"
				"\t\t\t<div id='preview-container'>\n"
				"\t\t\t\t<!-- Handler 1 -->\n"
				"\t\t\t\t<div id='handler-container1' class='handler-container' data-visible='true' data-active='false'>\n"
				"\t\t\t\t\t<span data-for='#photo1' class='photo-remover'>&times;</span>\n"
				"\t\t\t\t\t<img data-for='#photo1' id='photo-handler1' class='photo-handler' src='/img/cross.png'>\n"
				"\t\t\t\t</div>\n"
				"\t\t\t\t<!-- Handler 2 -->\n"
				"\t\t\t\t<div id='handler-container2' class='handler-container' data-visible='false' data-active='false'>\n"
				"\t\t\t\t\t<span data-for='#photo2' class='photo-remover'>&times;</span>\n"
				"\t\t\t\t\t<img data-for='#photo2' id='photo-handler2' class='photo-handler' src='/img/cross.png'>\n"
				"\t\t\t\t</div>\n"
				"\t\t\t\t<!-- Handler 3 -->\n"
				"\t\t\t\t<div id='handler-container3' class='handler-container' data-visible='false' data-active='false'>\n"
				"\t\t\t\t\t<span data-for='#photo3' class='photo-remover'>&times;</span>\n"
				"\t\t\t\t\t<img data-for='#photo3' id='photo-handler3' class='photo-handler' src='/img/cross.png'>\n"
				"\t\t\t\t</div>\n"
				"\t\t\t\t<!-- Handler 4 -->\n"
				"\t\t\t\t<div id='handler-container4' class='handler-container' data-visible='false' data-active='false'>\n"
				"\t\t\t\t\t<span data-for='#photo4' class='photo-remover'>&times;</span>\n"
				"\t\t\t\t\t<img data-for='#photo4' id='photo-handler4' class='photo-handler' src='/img/cross.png'>\n"
				"\t\t\t\t</div>\n"
				"\t\t\t</div>\n"
				"\t\t</form>\n"
				"\t\t<button id='post-form-button'>Create Post</button>\n"
				"\t</div>\n"
				"";
		#line 127 "app/tmpl_src/layout/layout.tmpl"
		} // end of template create_post_form
		#line 130 "app/tmpl_src/layout/layout.tmpl"
		virtual void register_form() {
			#line 181 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='banner'>Create an account to get started</span>\n"
				"\t<div id='signup-why'>\n"
				"\t\t<span id='main'>Build trust with your buyers and sellers</span>\n"
				"\t\t<span id='main-support'>\n"
				"\t\t\tWith built-in review and reporting system we filter and remove scammers and people who have a history of backing-out/wasting time.\n"
				"\t\t</span>\n"
				"\t\t<span id='secondary'>Find buyers and sellers quickly and efficiently</span>\n"
				"\t\t<span id='secondary-support'>\n"
				"\t\t\tEfficient search algorithms ensure you can find relevant products closests to you and at a price you want.\n"
				"\t\t</span>\n"
				"\t\t<span id='last'>Build a brand with no overhead</span>\n"
				"\t\t<span id='last-support'>\n"
				"\t\t\tSell your products through DMV Exchange and avoid the cost and associated overhead of a business.\n"
				"\t\t</span>\n"
				"\t</div>\n"
				"\t<div id='signup-card'>\n"
				"\t\t<div id='signup-container'>\n"
				"\t\t\t<form id='signup-form' action='/register/process' method='post'>\n"
				"\t\t\t\t<label>Name</label>\n"
				"\t\t\t\t<input id='firstname' name='f' type='text' placeholder='First' class='same-line-input field' spellcheck='false'>\n"
				"\t\t\t\t<input id='lastname' name='l' type='text' placeholder='Last' class='same-line-input field' spellcheck='false'>\n"
				"\t\t\t\t<label>Username</label>\n"
				"\t\t\t\t<input id='username' name='u' type='text' placeholder='Username' class='single-line-input field' spellcheck='false'>\n"
				"\t\t\t\t<label>Password</label>\n"
				"\t\t\t\t<input id='password' name='p' type='password' placeholder='Password' class='single-line-input field'>\n"
				"\t\t\t\t<label>Confirm password</label>\n"
				"\t\t\t\t<input id='password-again' name='pa' type='password' placeholder='Confirm password' class='single-line-input field'>\n"
				"\t\t\t\t<label>Email</label>\n"
				"\t\t\t\t<input id='email' name='e' type='text' placeholder='Email' class='single-line-input field' spellcheck='false'>\n"
				"\t\t\t\t<label>Confirm email</label>\n"
				"\t\t\t\t<input id='email-again' name='ea' type='text' placeholder='Confirm email' class='single-line-input field' spellcheck='false'>\n"
				"\t\t\t\t<label>Zipcode</label>\n"
				"\t\t\t\t<input id='zipcode' name='z' maxlength='5' type='text' placeholder='Zipcode' class='same-line-input field' spellcheck='false'>\n"
				"\t\t\t\t<label>Gender</label>\n"
				"\t\t\t\t<select id='gender-select' name='g' class='field'>\n"
				"\t\t\t\t\t<option selected='true'>I am...</option>\n"
				"\t\t\t\t\t<option value='m'>Male</option>\n"
				"\t\t\t\t\t<option value='f'>Female</option>\n"
				"\t\t\t\t</select>\n"
				"\t\t\t\t<label>Terms and Conditions</label>\n"
				"\t\t\t\t<div id='tos-container' class='field'>\n"
				"\t\t\t\t\t<input id='tos-checkbox' type='checkbox' value='I_AGREE' name='tos'>\n"
				"\t\t\t\t\t<span id='tos-disclosure'>\n"
				"\t\t\t\t\t\tI agree to the <a target=\"_blank\" href='/h/policy.html'>policies</a> and the <a target=\"_blank\" href='/h/policy.html'>Terms of Service</a>\n"
				"\t\t\t\t\t</span>\n"
				"\t\t\t\t</div>\n"
				"\t\t\t\t<button id='signup-button'>Sign up</button>\n"
				"\t\t\t</form>\n"
				"\t\t</div>\n"
				"\t</div>\n"
				"";
		#line 181 "app/tmpl_src/layout/layout.tmpl"
		} // end of template register_form
		#line 185 "app/tmpl_src/layout/layout.tmpl"
		virtual void forgot_initial() {
			#line 198 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='forgot-header'>Select what you've forgotten...</span><br>\n"
				"\t<div id='forgot-container'>\n"
				"\t\t<form id='forgot-form' method='post' action='/forgot/process'>\n"
				"\t\t\t<div id='forgot-type-container'>\n"
				"\t\t\t\t<label>Forgot username</label><input class='radio-button' name='type' type='radio' value='username'>\n"
				"\t\t\t\t<br>\n"
				"\t\t\t\t<label>Forgot password</label><input class='radio-button' name='type' type='radio' value='password'>\n"
				"\t\t\t</div>\n"
				"\t\t\t<input id='id' type='text' name='id' placeholder='Username or email...' spellcheck='false'>\n"
				"\t\t</form>\n"
				"\t\t<button id='forgot-button'>Submit Request</button>\n"
				"\t</div>\n"
				"";
		#line 198 "app/tmpl_src/layout/layout.tmpl"
		} // end of template forgot_initial
		#line 200 "app/tmpl_src/layout/layout.tmpl"
		virtual void forgot_success() {
			#line 203 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='forgot-landing-header'>An email has been sent with further information</span>\n"
				"\t<a id='forgot-return' href='/'>Click here to return home</a>\n"
				"";
		#line 203 "app/tmpl_src/layout/layout.tmpl"
		} // end of template forgot_success
		#line 205 "app/tmpl_src/layout/layout.tmpl"
		virtual void reset_password() {
			#line 215 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='password-reset-header'>Password reset form</span>\n"
				"\t<form id='password-reset-form' method='post' action='/reset/process'>\n"
				"\t\t<input class='hidden' id='token' name='token' type='text' value=''>\n"
				"\t\t<label>Enter a new password</label>\n"
				"\t\t<input type='password' id='np' name='np' placeholder='Enter a new password...'><br>\n"
				"\t\t<label>Confirm new password</label>\n"
				"\t\t<input type='password' id='npa' name='npa' placeholder='Confirm new password...'><br>\n"
				"\t</form>\n"
				"\t<button id='reset-button'>Set new password</button>\n"
				"";
		#line 215 "app/tmpl_src/layout/layout.tmpl"
		} // end of template reset_password
		#line 217 "app/tmpl_src/layout/layout.tmpl"
		virtual void reset_success() {
			#line 220 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='reset-landing-header'>Your password has been reset!</span>\n"
				"\t<a id='forgot-return' href='/'>Click here to return home</a>\n"
				"";
		#line 220 "app/tmpl_src/layout/layout.tmpl"
		} // end of template reset_success
		#line 222 "app/tmpl_src/layout/layout.tmpl"
		virtual void account_main() {
			#line 235 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<div id='account-tabs-container'>\n"
				"\t\t<label id='account-tabs-label'>Settings</label>\n"
				"\t\t<span class='account-tab' data-selected='true'>Account</span>\n"
				"\t\t<a href='/account/filters'>\n"
				"\t\t\t<span class='account-tab' data-selected='false'>Blocked</span>\n"
				"\t\t</a>\n"
				"\t</div>\n"
				"\t<div id='account-view-container'>\n"
				"\t\t<label id='account-view-label'>Account</label>\n"
				"\t\t<div id='email-view-container'>\n"
				"\t\t\t<div id='primary-email-row'>\n"
				"\t\t\t\t<span id='primary-email-label'>Primary Email</span>\n"
				"\t\t\t\t<span class='email'>";
			#line 235 "app/tmpl_src/layout/layout.tmpl"
			out()<<cppcms::filters::escape(content.EMAIL);
			#line 237 "app/tmpl_src/layout/layout.tmpl"
			out()<<"</span>\n"
				"\t\t\t</div>\n"
				"\t\t\t";
			#line 237 "app/tmpl_src/layout/layout.tmpl"
			if(content.SECONDARY_EMAIL == "") {
				#line 242 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t<div id='secondary-email-row'>\n"
					"\t\t\t\t\t<span id='secondary-email-label'></span>\n"
					"\t\t\t\t\t<span class='email add-new-button'>Add new email</span>\n"
					"\t\t\t\t</div>\n"
					"\t\t\t";
			#line 242 "app/tmpl_src/layout/layout.tmpl"
			}else{
				#line 245 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t<div id='secondary-email-row'>\n"
					"\t\t\t\t\t<span id='secondary-email-label'></span>\n"
					"\t\t\t\t\t<span class='email filled'>";
				#line 245 "app/tmpl_src/layout/layout.tmpl"
				out()<<cppcms::filters::escape(content.SECONDARY_EMAIL);
				#line 247 "app/tmpl_src/layout/layout.tmpl"
				out()<<"</span><span id='remove-secondary-email'>Remove</span>\n"
					"\t\t\t\t</div>\n"
					"\t\t\t";
			#line 247 "app/tmpl_src/layout/layout.tmpl"
			}
			#line 255 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t</div>\n"
				"\t\t<div id='password-view-container'>\n"
				"\t\t\t<span id='password-label'>Password</span>\n"
				"\t\t\t<span id='change-password-button'>Change Password</span>\n"
				"\t\t</div>\n"
				"\t\t<!-- Connect Social Accounts TBAH(To be added here) -->\n"
				"\t</div>\n"
				"";
		#line 255 "app/tmpl_src/layout/layout.tmpl"
		} // end of template account_main
		#line 257 "app/tmpl_src/layout/layout.tmpl"
		virtual void account_filters() {
			#line 282 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<div id='account-tabs-container'>\n"
				"\t\t<label id='account-tabs-label'>Settings</label>\n"
				"\t\t<a href='/account'>\n"
				"\t\t\t<span class='account-tab' data-selected='false'>Account</span>\n"
				"\t\t</a>\n"
				"\t\t<span class='account-tab' data-selected='true'>Blocked</span>\n"
				"\t</div>\n"
				"\t<div id='account-view-container'>\n"
				"\t\t<label id='account-view-label'>Blocked Users</label>\n"
				"\t\t<div id='blocked-users-container'>\n"
				"\t\t\t<div id='blocked-users-controls'>\n"
				"\t\t\t\t<div class='control-row'>\n"
				"\t\t\t\t\t<button id='add-user'>+</button><label>Add user</label>\n"
				"\t\t\t\t</div>\n"
				"\t\t\t\t<div class='control-row'>\n"
				"\t\t\t\t\t<button id='remove-user'>-</button><label>Remove user</label>\n"
				"\t\t\t\t</div>\n"
				"\t\t\t</div>\n"
				"\t\t\t<div id='blocked-users-list'>\n"
				"\t\t\t\t<!-- sample blocked user listing -->\n"
				"\t\t\t\t<!-- <span class='blocked-user' data-selected='true'>sample_usr</span> -->\t\t\t\t\n"
				"\t\t\t</div>\n"
				"\t\t</div>\n"
				"\t</div>\n"
				"";
		#line 282 "app/tmpl_src/layout/layout.tmpl"
		} // end of template account_filters
		#line 284 "app/tmpl_src/layout/layout.tmpl"
		virtual void activation_success() {
			#line 287 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='activation-success'>Your account has been activated!</span>\n"
				"\t<a id='forgot-return' href='/'>Click here to return home</a>\n"
				"";
		#line 287 "app/tmpl_src/layout/layout.tmpl"
		} // end of template activation_success
		#line 289 "app/tmpl_src/layout/layout.tmpl"
		virtual void activation_empty() {
			#line 292 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='activation-empty'>No activation token specified!</span>\n"
				"\t<a id='forgot-return' href='/'>Click here to return home</a>\n"
				"";
		#line 292 "app/tmpl_src/layout/layout.tmpl"
		} // end of template activation_empty
		#line 294 "app/tmpl_src/layout/layout.tmpl"
		virtual void activation_failed() {
			#line 297 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='activation-failed'>Invalid or already used token specified!</span>\n"
				"\t<a id='forgot-return' href='/'>Click here to return home</a>\n"
				"";
		#line 297 "app/tmpl_src/layout/layout.tmpl"
		} // end of template activation_failed
		#line 299 "app/tmpl_src/layout/layout.tmpl"
		virtual void tips_main() {
			#line 305 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='tips-header'>Enter your suggestion below</span>\n"
				"\t<div id='tips-container'>\n"
				"\t\t<textarea id='tips-content'></textarea>\n"
				"\t\t<button>Submit tip</button>\n"
				"\t</div>\n"
				"";
		#line 305 "app/tmpl_src/layout/layout.tmpl"
		} // end of template tips_main
		#line 307 "app/tmpl_src/layout/layout.tmpl"
		virtual void market_block() {
			#line 308 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 308 "app/tmpl_src/layout/layout.tmpl"
			if(content.LOGGED_IN == "true") {
				#line 326 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<div id='left-panel'>\n"
					"\t\t\t<div id='search-container'>\n"
					"\t\t\t\t<input id='text' type='text' placeholder='Search posts...'><button>Search</button>\n"
					"\t\t\t</div>\n"
					"\t\t\t<label id='views-label'>Filters</label>\n"
					"\t\t\t<div id='view-container'>\n"
					"\t\t\t\t<span id='all' class='view' data-selected='true'>All</span>\n"
					"\t\t\t\t<span id='sale' class='view' data-selected='false'>For Sale</span>\n"
					"\t\t\t\t<span id='general' class='view' data-selected='false'>General</span>\n"
					"\t\t\t</div>\n"
					"\t\t\t<label id='controls-label'>Controls</label>\n"
					"\t\t\t<div id='controls-container'>\n"
					"\t\t\t\t<span class='control'>My Store</span>\n"
					"\t\t\t\t<span class='control'>My Posts</span>\n"
					"\t\t\t\t<span class='control'>Settings</span>\n"
					"\t\t\t</div>\n"
					"\t\t</div>\n"
					"\t";
			#line 326 "app/tmpl_src/layout/layout.tmpl"
			}else{
				#line 338 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<div id='left-panel' class='unauth'>\n"
					"\t\t\t<div id='search-container'>\n"
					"\t\t\t\t<input id='text' type='text' placeholder='Search posts...'><button>Search</button>\n"
					"\t\t\t</div>\n"
					"\t\t\t<label id='views-label'>Filters</label>\n"
					"\t\t\t<div id='view-container'>\n"
					"\t\t\t\t<span id='all' class='view' data-selected='true'>All</span>\n"
					"\t\t\t\t<span id='sale' class='view' data-selected='false'>For Sale</span>\n"
					"\t\t\t\t<span id='general' class='view' data-selected='false'>General</span>\n"
					"\t\t\t</div>\n"
					"\t\t</div>\n"
					"\t";
			#line 338 "app/tmpl_src/layout/layout.tmpl"
			}
			#line 340 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<div class='auth' id='center-feed'><span style='display:block;text-align:center;font-family:GOOGLE;font-size:1.025em;'>Loading products...</span></div>\n"
				"";
		#line 340 "app/tmpl_src/layout/layout.tmpl"
		} // end of template market_block
		#line 342 "app/tmpl_src/layout/layout.tmpl"
		virtual void render() {
			#line 346 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"<!doctype html>\n"
				"<html>\n"
				"<head>\n"
				"\t<title>DX &#8212; ";
			#line 346 "app/tmpl_src/layout/layout.tmpl"
			out()<<cppcms::filters::escape(content.TITLE);
			#line 351 "app/tmpl_src/layout/layout.tmpl"
			out()<<"</title>\n"
				"\t<meta name=\"viewport\" session=\"width=device-width, initial-scale=1\">\n"
				"\t<link rel='stylesheet' type='text/css' href='/css/main.css?v=1'>\n"
				"\t<link rel='stylesheet' type='text/css' href='/css/normalize.css?v=1'>\n"
				"\t<link rel='stylesheet' type='text/css' href='/css/common.css?v=1'>\n"
				"\t";
			#line 351 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "REGISTER") {
				#line 353 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<link rel='stylesheet' type='text/css' href='/css/register.css?v=1'>\n"
					"\t";
			#line 353 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 357 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"</head>\n"
				"<body>\n"
				"\t<div id='wrapper'>\n"
				"\t\t";
			#line 357 "app/tmpl_src/layout/layout.tmpl"
			header();
			#line 359 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t<div id='body-wrapper'>\n"
				"\t\t\t";
			#line 359 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "HOME") {
				#line 361 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t<!-- Products Start -->\n"
					"\t\t\t\t";
				#line 361 "app/tmpl_src/layout/layout.tmpl"
				market_block();
				#line 363 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t<!-- End Products -->\n"
					"\t\t\t";
			#line 363 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 365 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\n"
				"\t\t\t";
			#line 365 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "ACTIVATION_SUCCESS") {
				#line 366 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 366 "app/tmpl_src/layout/layout.tmpl"
				activation_success();
				#line 367 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 367 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 369 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t\n"
				"\t\t\t";
			#line 369 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "ACTIVATION_EMPTY") {
				#line 370 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 370 "app/tmpl_src/layout/layout.tmpl"
				activation_empty();
				#line 371 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 371 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 373 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\n"
				"\t\t\t";
			#line 373 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "ACTIVATION_FAILED") {
				#line 374 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 374 "app/tmpl_src/layout/layout.tmpl"
				activation_failed();
				#line 375 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 375 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 377 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t\n"
				"\t\t\t";
			#line 377 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "TIPS_MAIN") {
				#line 378 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 378 "app/tmpl_src/layout/layout.tmpl"
				tips_main();
				#line 379 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 379 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 381 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\n"
				"\t\t\t";
			#line 381 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "CREATEPOST") {
				#line 382 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 382 "app/tmpl_src/layout/layout.tmpl"
				if(content.LOGGED_IN == "true") {
					#line 383 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t\t";
					#line 383 "app/tmpl_src/layout/layout.tmpl"
					create_post_form();
					#line 384 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t";
				#line 384 "app/tmpl_src/layout/layout.tmpl"
				} // endif
				#line 385 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 385 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 386 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 386 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "REGISTER") {
				#line 387 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 387 "app/tmpl_src/layout/layout.tmpl"
				register_form();
				#line 388 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 388 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 389 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 389 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "FORGOT_INITIAL") {
				#line 390 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 390 "app/tmpl_src/layout/layout.tmpl"
				forgot_initial();
				#line 391 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 391 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 392 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 392 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "FORGOT_SUCCESS") {
				#line 393 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 393 "app/tmpl_src/layout/layout.tmpl"
				forgot_success();
				#line 394 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 394 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 395 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 395 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "PASSWORD_RESET") {
				#line 396 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 396 "app/tmpl_src/layout/layout.tmpl"
				reset_password();
				#line 397 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 397 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 398 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 398 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "RESET_SUCCESS") {
				#line 399 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 399 "app/tmpl_src/layout/layout.tmpl"
				reset_success();
				#line 400 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 400 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 401 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 401 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "ACCOUNT_MAIN") {
				#line 402 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 402 "app/tmpl_src/layout/layout.tmpl"
				account_main();
				#line 403 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 403 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 404 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 404 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "ACCOUNT_FILTERS") {
				#line 405 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 405 "app/tmpl_src/layout/layout.tmpl"
				account_filters();
				#line 406 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 406 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 412 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t</div>\n"
				"\t</div>\n"
				"\t<!-- JScripts -->\n"
				"\t<script src='/js/jquery.min.js'></script>\n"
				"\t<script src='/js/common.js'></script>\n"
				"\t";
			#line 412 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "HOME") {
				#line 414 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/market.js'></script>\n"
					"\t";
			#line 414 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 415 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 415 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "CREATEPOST") {
				#line 417 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/market.js'></script>\n"
					"\t";
			#line 417 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 418 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 418 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "REGISTER") {
				#line 420 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/register.js'></script>\n"
					"\t";
			#line 420 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 421 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 421 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "FORGOT_INITIAL") {
				#line 423 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/forgot.js'></script>\n"
					"\t";
			#line 423 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 424 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 424 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "PASSWORD_RESET") {
				#line 426 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/reset.js'></script>\n"
					"\t";
			#line 426 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 427 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 427 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "ACCOUNT_MAIN" || content.PAGE == "ACCOUNT_FILTERS") {
				#line 429 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/account.js'></script>\n"
					"\t";
			#line 429 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 430 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 430 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "TIPS_MAIN") {
				#line 432 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/tips.js'></script>\n"
					"\t";
			#line 432 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 435 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"</body>\n"
				"</html>\n"
				"";
		#line 435 "app/tmpl_src/layout/layout.tmpl"
		} // end of template render
	#line 437 "app/tmpl_src/layout/layout.tmpl"
	}; // end of class master
#line 438 "app/tmpl_src/layout/layout.tmpl"
} // end of namespace layout
#line 438 "app/tmpl_src/layout/layout.tmpl"
namespace {
#line 438 "app/tmpl_src/layout/layout.tmpl"
 cppcms::views::generator my_generator; 
#line 438 "app/tmpl_src/layout/layout.tmpl"
 struct loader { 
#line 438 "app/tmpl_src/layout/layout.tmpl"
  loader() { 
#line 438 "app/tmpl_src/layout/layout.tmpl"
   my_generator.name("layout");
#line 438 "app/tmpl_src/layout/layout.tmpl"
   my_generator.add_view<layout::master,Pages::Context>("master",true);
#line 438 "app/tmpl_src/layout/layout.tmpl"
    cppcms::views::pool::instance().add(my_generator);
#line 438 "app/tmpl_src/layout/layout.tmpl"
 }
#line 438 "app/tmpl_src/layout/layout.tmpl"
 ~loader() {  cppcms::views::pool::instance().remove(my_generator); }
#line 438 "app/tmpl_src/layout/layout.tmpl"
} a_loader;
#line 438 "app/tmpl_src/layout/layout.tmpl"
} // anon 
