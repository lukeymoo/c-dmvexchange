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
				#line 24 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t<a href='/tips'>\n"
					"\t\t\t\t\t<span class='header-menu-option'>Submit advice</span>\n"
					"\t\t\t\t</a>\n"
					"\t\t\t";
			#line 24 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 25 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 25 "app/tmpl_src/layout/layout.tmpl"
			if(content.LOGGED_IN == "true") {
				#line 28 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t<a href=\"/account\"><span class='header-menu-option'>Account</span></a>\n"
					"\t\t\t\t<span id='header-logout-button' class=\"header-menu-option\">Logout</span>\n"
					"\t\t\t";
			#line 28 "app/tmpl_src/layout/layout.tmpl"
			}else{
				#line 32 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t<a href='/register'>\n"
					"\t\t\t\t\t<span class='header-menu-option'>Sign up</span>\n"
					"\t\t\t\t</a>\n"
					"\t\t\t";
			#line 32 "app/tmpl_src/layout/layout.tmpl"
			}
			#line 35 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t</div>\n"
				"\t\t<!-- Header controls -->\n"
				"\t\t";
			#line 35 "app/tmpl_src/layout/layout.tmpl"
			if(content.LOGGED_IN == "true") {
				#line 37 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t<div id='header-controls'>\n"
					"\t\t\t\t";
				#line 37 "app/tmpl_src/layout/layout.tmpl"
				if(content.PAGE == "CREATEPOST") {
					#line 39 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t\t<span data-active='true' id='header-create-post-button' class='header-control-button'>Create Post</span>\n"
						"\t\t\t\t";
				#line 39 "app/tmpl_src/layout/layout.tmpl"
				}else{
					#line 41 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t\t<a href='/p/new'><span data-active='false' id='header-create-post-button' class='header-control-button'>Create Post</span></a>\n"
						"\t\t\t\t";
				#line 41 "app/tmpl_src/layout/layout.tmpl"
				}
				#line 44 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\n"
					"\t\t\t\t<span data-active='false' id='header-notifications-button' class='header-control-button'>Notifications</span>\n"
					"\t\t\t\t<span data-active='false' id='header-profile-button' class='header-control-button'>";
				#line 44 "app/tmpl_src/layout/layout.tmpl"
				out()<<cppcms::filters::escape(content.USERNAME);
				#line 46 "app/tmpl_src/layout/layout.tmpl"
				out()<<"</span>\n"
					"\t\t\t</div>\n"
					"\t\t";
			#line 46 "app/tmpl_src/layout/layout.tmpl"
			}else{
				#line 50 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<div id='header-controls'>\n"
					"\t\t\t<span id='header-login-button' data-active='false' class='header-control-button' data-state='closed'>Log in</span>\n"
					"\t\t\t<a href='/register'>\n"
					"\t\t\t\t";
				#line 50 "app/tmpl_src/layout/layout.tmpl"
				if(content.PAGE == "REGISTER") {
					#line 52 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t\t<span data-active='true' class='header-control-button'>Register</span>\n"
						"\t\t\t\t";
				#line 52 "app/tmpl_src/layout/layout.tmpl"
				}else{
					#line 54 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t\t<span data-active='false' class='header-control-button'>Register</span>\n"
						"\t\t\t\t";
				#line 54 "app/tmpl_src/layout/layout.tmpl"
				}
				#line 57 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t</a>\n"
					"\t\t</div>\n"
					"\t\t";
			#line 57 "app/tmpl_src/layout/layout.tmpl"
			}
			#line 60 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t<!-- End header controls -->\n"
				"\t\t<!-- Login form -->\n"
				"\t\t";
			#line 60 "app/tmpl_src/layout/layout.tmpl"
			if(content.LOGGED_IN == "false") {
				#line 70 "app/tmpl_src/layout/layout.tmpl"
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
			#line 70 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 74 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t<!-- End login form -->\n"
				"\t</div>\n"
				"\t<div id='notification-container'></div>\n"
				"";
		#line 74 "app/tmpl_src/layout/layout.tmpl"
		} // end of template header
		#line 76 "app/tmpl_src/layout/layout.tmpl"
		virtual void create_post_form() {
			#line 119 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<div data-visible='true' id='post-form-container'>\n"
				"\t\t<form action='/p/process' method='POST' enctype='multipart/form-data' name='postform' id='post-form' data-type='sale' data-valid='false'>\n"
				"\t\t\t<img id='post-form-validator' src='/img/cross.png'>\n"
				"\t\t\t<input type='text' name='posttype' class='hidden-input' value='sale'>\n"
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
		#line 119 "app/tmpl_src/layout/layout.tmpl"
		} // end of template create_post_form
		#line 122 "app/tmpl_src/layout/layout.tmpl"
		virtual void register_form() {
			#line 173 "app/tmpl_src/layout/layout.tmpl"
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
		#line 173 "app/tmpl_src/layout/layout.tmpl"
		} // end of template register_form
		#line 177 "app/tmpl_src/layout/layout.tmpl"
		virtual void forgot_initial() {
			#line 190 "app/tmpl_src/layout/layout.tmpl"
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
		#line 190 "app/tmpl_src/layout/layout.tmpl"
		} // end of template forgot_initial
		#line 192 "app/tmpl_src/layout/layout.tmpl"
		virtual void forgot_success() {
			#line 195 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='forgot-landing-header'>An email has been sent with further information</span>\n"
				"\t<a id='forgot-return' href='/'>Click here to return home</a>\n"
				"";
		#line 195 "app/tmpl_src/layout/layout.tmpl"
		} // end of template forgot_success
		#line 197 "app/tmpl_src/layout/layout.tmpl"
		virtual void reset_password() {
			#line 207 "app/tmpl_src/layout/layout.tmpl"
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
		#line 207 "app/tmpl_src/layout/layout.tmpl"
		} // end of template reset_password
		#line 209 "app/tmpl_src/layout/layout.tmpl"
		virtual void reset_success() {
			#line 212 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='reset-landing-header'>Your password has been reset!</span>\n"
				"\t<a id='forgot-return' href='/'>Click here to return home</a>\n"
				"";
		#line 212 "app/tmpl_src/layout/layout.tmpl"
		} // end of template reset_success
		#line 214 "app/tmpl_src/layout/layout.tmpl"
		virtual void account_main() {
			#line 227 "app/tmpl_src/layout/layout.tmpl"
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
			#line 227 "app/tmpl_src/layout/layout.tmpl"
			out()<<cppcms::filters::escape(content.EMAIL);
			#line 229 "app/tmpl_src/layout/layout.tmpl"
			out()<<"</span>\n"
				"\t\t\t</div>\n"
				"\t\t\t";
			#line 229 "app/tmpl_src/layout/layout.tmpl"
			if(content.SECONDARY_EMAIL == "") {
				#line 234 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t<div id='secondary-email-row'>\n"
					"\t\t\t\t\t<span id='secondary-email-label'></span>\n"
					"\t\t\t\t\t<span class='email add-new-button'>Add new email</span>\n"
					"\t\t\t\t</div>\n"
					"\t\t\t";
			#line 234 "app/tmpl_src/layout/layout.tmpl"
			}else{
				#line 237 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t<div id='secondary-email-row'>\n"
					"\t\t\t\t\t<span id='secondary-email-label'></span>\n"
					"\t\t\t\t\t<span class='email filled'>";
				#line 237 "app/tmpl_src/layout/layout.tmpl"
				out()<<cppcms::filters::escape(content.SECONDARY_EMAIL);
				#line 239 "app/tmpl_src/layout/layout.tmpl"
				out()<<"</span><span id='remove-secondary-email'>Remove</span>\n"
					"\t\t\t\t</div>\n"
					"\t\t\t";
			#line 239 "app/tmpl_src/layout/layout.tmpl"
			}
			#line 247 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t</div>\n"
				"\t\t<div id='password-view-container'>\n"
				"\t\t\t<span id='password-label'>Password</span>\n"
				"\t\t\t<span id='change-password-button'>Change Password</span>\n"
				"\t\t</div>\n"
				"\t\t<!-- Connect Social Accounts TBAH(To be added here) -->\n"
				"\t</div>\n"
				"";
		#line 247 "app/tmpl_src/layout/layout.tmpl"
		} // end of template account_main
		#line 249 "app/tmpl_src/layout/layout.tmpl"
		virtual void account_filters() {
			#line 274 "app/tmpl_src/layout/layout.tmpl"
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
		#line 274 "app/tmpl_src/layout/layout.tmpl"
		} // end of template account_filters
		#line 276 "app/tmpl_src/layout/layout.tmpl"
		virtual void activation_success() {
			#line 279 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='activation-success'>Your account has been activated!</span>\n"
				"\t<a id='forgot-return' href='/'>Click here to return home</a>\n"
				"";
		#line 279 "app/tmpl_src/layout/layout.tmpl"
		} // end of template activation_success
		#line 281 "app/tmpl_src/layout/layout.tmpl"
		virtual void activation_empty() {
			#line 284 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='activation-empty'>No activation token specified!</span>\n"
				"\t<a id='forgot-return' href='/'>Click here to return home</a>\n"
				"";
		#line 284 "app/tmpl_src/layout/layout.tmpl"
		} // end of template activation_empty
		#line 286 "app/tmpl_src/layout/layout.tmpl"
		virtual void activation_failed() {
			#line 289 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='activation-failed'>Invalid or already used token specified!</span>\n"
				"\t<a id='forgot-return' href='/'>Click here to return home</a>\n"
				"";
		#line 289 "app/tmpl_src/layout/layout.tmpl"
		} // end of template activation_failed
		#line 291 "app/tmpl_src/layout/layout.tmpl"
		virtual void tips_main() {
			#line 297 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t<span id='tips-header'>Enter your suggestion below</span>\n"
				"\t<div id='tips-container'>\n"
				"\t\t<textarea id='tips-content'></textarea>\n"
				"\t\t<button>Submit tip</button>\n"
				"\t</div>\n"
				"";
		#line 297 "app/tmpl_src/layout/layout.tmpl"
		} // end of template tips_main
		#line 299 "app/tmpl_src/layout/layout.tmpl"
		virtual void render() {
			#line 303 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"<!doctype html>\n"
				"<html>\n"
				"<head>\n"
				"\t<title>DX &#8212; ";
			#line 303 "app/tmpl_src/layout/layout.tmpl"
			out()<<cppcms::filters::escape(content.TITLE);
			#line 308 "app/tmpl_src/layout/layout.tmpl"
			out()<<"</title>\n"
				"\t<meta name=\"viewport\" session=\"width=device-width, initial-scale=1\">\n"
				"\t<link rel='stylesheet' type='text/css' href='/css/main.css?v=1'>\n"
				"\t<link rel='stylesheet' type='text/css' href='/css/normalize.css?v=1'>\n"
				"\t<link rel='stylesheet' type='text/css' href='/css/common.css?v=1'>\n"
				"\t";
			#line 308 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "REGISTER") {
				#line 310 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<link rel='stylesheet' type='text/css' href='/css/register.css?v=1'>\n"
					"\t";
			#line 310 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 314 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"</head>\n"
				"<body>\n"
				"\t<div id='wrapper'>\n"
				"\t\t";
			#line 314 "app/tmpl_src/layout/layout.tmpl"
			header();
			#line 316 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t<div id='body-wrapper'>\n"
				"\t\t\t";
			#line 316 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "HOME") {
				#line 321 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t<!-- Products Start -->\n"
					"\t\t\t\t<div id='products-container'>\n"
					"\t\t\t\t</div>\n"
					"\t\t\t\t<!-- End Products -->\n"
					"\t\t\t";
			#line 321 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 323 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\n"
				"\t\t\t";
			#line 323 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "ACTIVATION_SUCCESS") {
				#line 324 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 324 "app/tmpl_src/layout/layout.tmpl"
				activation_success();
				#line 325 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 325 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 327 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t\n"
				"\t\t\t";
			#line 327 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "ACTIVATION_EMPTY") {
				#line 328 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 328 "app/tmpl_src/layout/layout.tmpl"
				activation_empty();
				#line 329 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 329 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 331 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\n"
				"\t\t\t";
			#line 331 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "ACTIVATION_FAILED") {
				#line 332 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 332 "app/tmpl_src/layout/layout.tmpl"
				activation_failed();
				#line 333 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 333 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 335 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t\n"
				"\t\t\t";
			#line 335 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "TIPS_MAIN") {
				#line 336 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 336 "app/tmpl_src/layout/layout.tmpl"
				tips_main();
				#line 337 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 337 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 339 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\n"
				"\t\t\t";
			#line 339 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "CREATEPOST") {
				#line 340 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 340 "app/tmpl_src/layout/layout.tmpl"
				if(content.LOGGED_IN == "true") {
					#line 341 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t\t";
					#line 341 "app/tmpl_src/layout/layout.tmpl"
					create_post_form();
					#line 342 "app/tmpl_src/layout/layout.tmpl"
					out()<<"\n"
						"\t\t\t\t";
				#line 342 "app/tmpl_src/layout/layout.tmpl"
				} // endif
				#line 343 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 343 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 344 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 344 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "REGISTER") {
				#line 345 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 345 "app/tmpl_src/layout/layout.tmpl"
				register_form();
				#line 346 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 346 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 347 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 347 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "FORGOT_INITIAL") {
				#line 348 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 348 "app/tmpl_src/layout/layout.tmpl"
				forgot_initial();
				#line 349 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 349 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 350 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 350 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "FORGOT_SUCCESS") {
				#line 351 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 351 "app/tmpl_src/layout/layout.tmpl"
				forgot_success();
				#line 352 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 352 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 353 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 353 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "PASSWORD_RESET") {
				#line 354 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 354 "app/tmpl_src/layout/layout.tmpl"
				reset_password();
				#line 355 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 355 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 356 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 356 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "RESET_SUCCESS") {
				#line 357 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 357 "app/tmpl_src/layout/layout.tmpl"
				reset_success();
				#line 358 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 358 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 359 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 359 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "ACCOUNT_MAIN") {
				#line 360 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 360 "app/tmpl_src/layout/layout.tmpl"
				account_main();
				#line 361 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 361 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 362 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t\t";
			#line 362 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "ACCOUNT_FILTERS") {
				#line 363 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t\t";
				#line 363 "app/tmpl_src/layout/layout.tmpl"
				account_filters();
				#line 364 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t\t";
			#line 364 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 370 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t\t</div>\n"
				"\t</div>\n"
				"\t<!-- JScripts -->\n"
				"\t<script src='/js/jquery.min.js'></script>\n"
				"\t<script src='/js/common.js'></script>\n"
				"\t";
			#line 370 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "HOME") {
				#line 372 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/market.js'></script>\n"
					"\t";
			#line 372 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 373 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 373 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "CREATEPOST") {
				#line 375 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/market.js'></script>\n"
					"\t";
			#line 375 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 376 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 376 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "REGISTER") {
				#line 378 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/register.js'></script>\n"
					"\t";
			#line 378 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 379 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 379 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "FORGOT_INITIAL") {
				#line 381 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/forgot.js'></script>\n"
					"\t";
			#line 381 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 382 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 382 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "PASSWORD_RESET") {
				#line 384 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/reset.js'></script>\n"
					"\t";
			#line 384 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 385 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 385 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "ACCOUNT_MAIN" || content.PAGE == "ACCOUNT_FILTERS") {
				#line 387 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/account.js'></script>\n"
					"\t";
			#line 387 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 388 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"\t";
			#line 388 "app/tmpl_src/layout/layout.tmpl"
			if(content.PAGE == "TIPS_MAIN") {
				#line 390 "app/tmpl_src/layout/layout.tmpl"
				out()<<"\n"
					"\t\t<script src='/js/tips.js'></script>\n"
					"\t";
			#line 390 "app/tmpl_src/layout/layout.tmpl"
			} // endif
			#line 393 "app/tmpl_src/layout/layout.tmpl"
			out()<<"\n"
				"</body>\n"
				"</html>\n"
				"";
		#line 393 "app/tmpl_src/layout/layout.tmpl"
		} // end of template render
	#line 395 "app/tmpl_src/layout/layout.tmpl"
	}; // end of class master
#line 396 "app/tmpl_src/layout/layout.tmpl"
} // end of namespace layout
#line 396 "app/tmpl_src/layout/layout.tmpl"
namespace {
#line 396 "app/tmpl_src/layout/layout.tmpl"
 cppcms::views::generator my_generator; 
#line 396 "app/tmpl_src/layout/layout.tmpl"
 struct loader { 
#line 396 "app/tmpl_src/layout/layout.tmpl"
  loader() { 
#line 396 "app/tmpl_src/layout/layout.tmpl"
   my_generator.name("layout");
#line 396 "app/tmpl_src/layout/layout.tmpl"
   my_generator.add_view<layout::master,Pages::Context>("master",true);
#line 396 "app/tmpl_src/layout/layout.tmpl"
    cppcms::views::pool::instance().add(my_generator);
#line 396 "app/tmpl_src/layout/layout.tmpl"
 }
#line 396 "app/tmpl_src/layout/layout.tmpl"
 ~loader() {  cppcms::views::pool::instance().remove(my_generator); }
#line 396 "app/tmpl_src/layout/layout.tmpl"
} a_loader;
#line 396 "app/tmpl_src/layout/layout.tmpl"
} // anon 
