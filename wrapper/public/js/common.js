'use strict';

var state = {
	LOGGED_IN: null,
	USERNAME: null,
	PAGE: null
};

var countdown; // for logout form, will hold interval

$(function() {
	var isLoginSubmitted = false;

	// Get current page
	state.PAGE = getCurrentPage();

	// Get session variables
	getSessionState(function(res) {
		if(res.error) {
			presentError(res.error);
		} else {
			if(res.LOGGED_IN == "true") {
				state.LOGGED_IN = true;
				state.USERNAME = res.USERNAME;
			} else {
				if(getParam('err') == 'need_login') {
					toggleHeaderLoginForm();
					createAlert("Must login to view page", "medium");
				}
			}
		}
	});

	var password = {
		obj: $('#header-login-form #password'),
		val: $('#header-login-form #password').val()
	};

	/** Open advice dialog box **/
	$(document).on('click', '#header-submit-advice', function() {
		var DOM = 
		"<div id='background-blur'></div>" +
		"<div id='tips-container'>" +
			"<span id='tips-header'>Enter your suggestion below</span>" +
			"<textarea id='tips-content'></textarea>" +
			"<div class='tips-buttons-container'>" +
				"<button class='good'>Submit tip</button><button class='bad'>Cancel</button>" +
			"</div>" +
		"</div>";
		$(DOM).appendTo($('#wrapper'));
	});

	/** Close advice dialog box **/
	$(document).on('click', '#tips-container button.bad', function() {
		$(document).find('#tips-container').remove();
		$(document).find('#background-blur').remove();
	});

	/** Process tip dialog box **/
	$(document).on('click', '#tips-container button.good', function() {
		// remove all errors
		$(this).parents('#tips-container').find('.form-error').remove();
		// get text field as object and contents
		var text = $(this).parents('#tips-container').find('#tips-content').val();
		var textField = $(this).parents('#tips-container').find('#tips-content');
		// ensure not empty and at least 10 characters
		if(text.length < 10) {
			generateFormError('Tip is too short! Must be at least 10 characters', textField);
		}
	});

	/** Toggle header menu on click **/
	$(document).on('click', '#header-menu-button', function() {
		toggleHeaderMenu();
	});

	/** Toggle login form on click **/
	$(document).on('click', '#header-login-button', function() {
		toggleHeaderLoginForm();
	});

	/** Close alert on click **/
	$(document).on('click', '.close-notification-button', function(e) {
		removeAlert($(this).parent('.notification').attr('data-id'));
	});

	/** Close forms on escape key **/
	$(document).on('keydown', function(e) {
		// Close form on escape
		if(e.which == 27) {
			if(isHeaderMenuOpen()) {
				closeHeaderMenu();
			}
			if(isHeaderLoginFormOpen()) {
				closeHeaderLoginForm();
			}
		}
	});

	/** Close forms/menus on click outside element **/
	$(document).on('click', function(e) {
		if(!$(e.target).is($('.notification')) && !$(e.target).closest($('#notification-container')).length) {
			if(isHeaderMenuOpen()) {
				if(!$(e.target).is($('#header-menu-button'))) {
					closeHeaderMenu();
				}
			}
			if(isHeaderLoginFormOpen()) {
				if(!$(e.target).is($('#header-login-button'))
					&& !$(e.target).is($('.need-login-menu-item'))
					&& !$(e.target).closest($('#header-login-container')).length) {
					closeHeaderLoginForm();
				}
			}
		}
	});

	/** Header Login Form **/
	$('#header-login-form input').on('keydown', function(e) {
		if(e.which == 13) {
			$('#header-login-form-button').click();
		}
	});
	$('#header-login-form-button').on('click', function(e) {
		// Reset form
		resetLoginStyles();
		clearLoginErrors();
		if(!isLoginSubmitted) {
			// If valid login form
			if(validLoginForm()) {
				isLoginSubmitted = true;
				// Hide submit button
				$('#header-login-form-button').hide();
				// Submit
				tryLogin(function(res) {
					// If no errors, valid login
					if(res.status == "DX-OK") {
						if(getParam('next')) {
							window.location.href = getParam('next');
						} else {
							window.location.href = '/';
						}
					} else {
						isLoginSubmitted = false;
						// Show login button
						$('#header-login-form-button').show();
						// DX-REJECTED ?
						if(res.status == 'DX-REJECTED') {
							// Insert error after password field
							evalError(res.message, password.obj);
						} else if(res.status == 'DX-FAILED') {
							createAlert(res.message, 'medium');
						}
					}
				});
			}
		}
	});

	// show logout confirmation
	$(document).on('click', '#header-logout-button', function() {
		confirmLogout();
	});

	// expedite logout
	$(document).on('click', '#confirm-logout-yes', function() {
		window.location.href = '/logout';
	});

	// cancel logout
	$(document).on('click', '#confirm-logout-no', function() {
		clearInterval(countdown);
		countdown = null;
		$(this).parents('#confirm-logout-container').remove();
		$(document).find('#background-blur').remove();
	});

});






















function confirmLogout() {
	var timestamp = Date.now();
	var DOM =
	"<div id='background-blur'></div><div id='confirm-logout-container'>" +
		"<label id='confirm-logout-label'>Are you sure you want to logout?</label>" +
		"<button id='confirm-logout-yes'>Logout</button><button id='confirm-logout-no'>Cancel</button>" +
		"<label id='auto-logout'>Auto-logout in <span id='" + timestamp + "'>20</span>s</label>" +
	"</div>";
	$(DOM).appendTo($('#wrapper'));
	countdown = setInterval(function() {
		var currentT = parseInt($(document).find('#' + timestamp).html());
		if(currentT > 1) { // count down
			--currentT;
			$(document).find('#' + timestamp).html(currentT);
		} else { // logout
			window.location.href = '/logout';
		}
	}, 1050);
	return;
}














/** Querys the server for the current session USERNAME & LOGGED_IN variables **/
function getSessionState(callback) {
	$.ajax({
		type: 'GET',
		url: '/api/session/state',
		error: function(err) {
			var res = {
				error: ['Server error, please try again']
			};
			if(err.status == 0) {
				res.error[0] = 'Server is currently down';
			}
			if(err.status == 404) {
				res.error[0] = 'Something isn\'t programmed <br>right...';
			}
			callback(res);
		}
	}).done(function(res) {
		res = JSON.parse(res);
		callback(res);
	});
}

/** Presents error from request **/
function presentError(ee) {
	createAlert(ee, 'high');
	return;
}

/** Returns the current page title as string **/
function getCurrentPage() {
	var title = $('title').html();
	title = title.substring(5, title.length);
	return title;
}

/** Querys server with supplied login credits & attempts validation **/
function tryLogin(callback) {
	$.ajax({
		type: 'POST',
		url: '/login/process',
		data: {
			u: $('#header-login-form #username-or-email').val(),
			p: $('#header-login-form #password').val()
		},
		error: function(err) {
			var res = {
				status: 'DX-FAILED',
				message: 'Server error please try again'
			};
			if(err.status == 0) {
				res.Message = 'Server is currently down';
			}
			if(err.status == 404) {
				res.Message = 'Something is wrong on server';
			}
			callback(res);
		}
	}).done(function(res) {
		res = JSON.parse(res);
		callback(res);
	});
}

/** Opens/Closes header action menu **/
function toggleHeaderMenu() {
	if(isHeaderMenuOpen()) {
		closeHeaderMenu();
	} else {
		openHeaderMenu();
	}
	return;
}

/** Opens header action menu **/
function openHeaderMenu() {
	$('#header-menu-button').attr('data-state', 'opened');
	$('#header-menu').show();
	return;
}

/** Closes header action menu **/
function closeHeaderMenu() {
	$('#header-menu-button').attr('data-state', 'closed');
	$('#header-menu').hide();
	return;
}

/** Returns true/false value if header action menu is opened/closed **/
function isHeaderMenuOpen() {
	return ($('#header-menu-button').attr('data-state') == 'opened') ? true : false;
}

/** Opens/Closes header login form **/
function toggleHeaderLoginForm() {
	if(isHeaderLoginFormOpen()) {
		closeHeaderLoginForm();
	} else {
		openHeaderLoginForm();
	}
	return;
}

/** Opens header login form **/
function openHeaderLoginForm() {
	$('#header-login-button').attr('data-state', 'opened');
	$('#header-login-container').show();
	$('#header-login-form #username-or-email').focus();
	return;
}

/** Closes Header **/
function closeHeaderLoginForm() {
	$('#header-login-button').attr('data-state', 'closed');
	$('#header-login-container').hide();
}

/** Returns true/false value if header login form is opened/closed **/
function isHeaderLoginFormOpen() {
	return ($('#header-login-button').attr('data-state') == 'opened') ? true : false;
}

/** Returns URi GET variable value as string **/
function getParam(sParam) {
	var sPageURL = window.location.search.substring(1);
	var sURLVariables = sPageURL.split('&');
	for(var i = 0; i < sURLVariables.length; i++) {
		var sParameterName = sURLVariables[i].split('=');
		if (sParameterName[0] == sParam) {
			return decodeURIComponent(sParameterName[1]);
		}
	}
}

/** Returns input border to default color/opacity **/
function goodStyle(obj) {
	$(obj).css('border', '1px solid rgba(0, 0, 0, 0.15)');
}

/** Colors input border to red to indicate errors **/
function badStyle(obj) {
	$(obj).css('border', '1px solid rgb(175, 0, 0)');
	return;
}

/** Determines if the login form has all fields needed to attempt login/validation **/
function validLoginForm() {
	var login_username = {obj:$('#header-login-form #username-or-email'), val:$('#header-login-form #username-or-email').val()};
	var login_password = {obj:$('#header-login-form #password'), val:$('#header-login-form #password').val()}

	var status = true;

	if(login_password.val.length) {
		if(!validPassword(login_password.val)) {
			status = false;
			generateFormError('Invalid password', login_password.obj);
		}
	} else {
		status = false;
		generateFormError('Password field is empty', login_password.obj);
	}

	if(login_username.val.length) {
		if(!validUsername(login_username.val)) {
			if(!validEmail(login_username.val)) {
				status = false;
				generateFormError('Invalid username or email', login_password.obj);
			}
		}
	} else {
		status = false;
		generateFormError('Username field is empty', login_password.obj);
	}

	return status;
}

/** Removes all generated errors in login form **/
function clearLoginErrors() {
	$('#header-login-form .form-error').each(function() {
		$(this).remove();
	});
	return;
}


/** Returns all input border colors/opacity to default in login form **/
function resetLoginStyles() {
	$('#header-login-form input').each(function() {
		goodStyle($(this));
	});
	return;
}

/** Evaluates an error code and generates user friendly version **/
function evalError(errors, obj) {
	// Clear error fields
	clearLoginErrors();
	var humanReadable = "";
	var errArr = errors.split("|");
	var stop = false;
	for(var err in errArr) {
		if(!stop) {
			switch(errArr[err]) {
				case "logged_in":
					humanReadable = "Already logged in";
					stop = true;
					break;
				case "u_invalid": // cannot determine if username/email
					humanReadable = "Not a valid username or email";
					stop = true;
					break;
				case "U_invalid_login":
					humanReadable = "Invalid username/password combo";
					stop = true;
					break;
				case "E_invalid_login":
					humanReadable = "Invalid email/password combo";
					stop = true;
					break;
				case 'invalid_form':
					humanReadable = "Form was missing fields";
					stop = true;
					break;
				case 'U':
					humanReadable += "Invalid username<br>";
					break;
				case 'E':
					humanReadable += "Invalid Email<br>";
					break;
				case 'P':
					humanReadable += "Invalid password<br>";
					break;
				default:
					humanReadable += "Unknown error occurred<br>";
					break;
			}
		}
	}
	generateFormError(humanReadable, obj);
	return;
}

/** Generates an error and appends it to specified object/field **/
function generateFormError(string, obj) {
	$("<span class='form-error'>" + string + "</span>").insertAfter(obj);
	return;
}

/** Returns true/false if supplied string can be a valid username **/
function validUsername(string) {
	return (/^[A-Za-z0-9_]+$/.test(string)
		&& string.length >= 2
		&& string.length < 16) ? true : false;
}

/** Returns true/false if supplied string can be a valid email address **/
function validEmail(string) {
	return (/^([a-zA-Z0-9_.+-])+\@(([a-zA-Z0-9-])+\.)+([a-zA-Z0-9]{2,4})+$/.test(string)
		&& string.length < 64) ? true : false;
}

/** Returns true/false if supplied string can be a valid password **/
function validPassword(string) {
	return (string.length >= 2 && string.length <= 32) ? true : false;
}

/** Creates a pop-up notification on users screen with specified message with notice type **/
function createAlert(string, alertLevel) {
	alertLevel = alertLevel || 'low';
	var classLevel = '';
	if(alertLevel == 'high') {
		classLevel = 'high-alert-level';
	}
	if(alertLevel == 'medium') {
		classLevel = 'medium-alert-level';
	}
	if(alertLevel == 'low') {
		classLevel = 'low-alert-level';
	}
	// Gender an ID using Date.now() ( this is for setTimeout removal )
	var noticeID = Date.now();
	var DOM = 
	"<div data-id='" + noticeID + "' class='notification " + classLevel +  "'>" + 
		"<span class='close-notification-button'>&times;</span>" +
		"<span class='notification-text'>" + string +  "</span>" +
	"</div>";
	$('#notification-container').prepend(DOM);
	// change bg color in 0.5 seconds
	$('#notification-container .notification').each(function() {
		if($(this).attr('data-id') == noticeID) {
			$(this).fadeOut('medium', function() {
				$(this).fadeIn('medium');
			});
		}
	});
	// Remove this notice after 6.5 seconds
	setTimeout(function() {
		$('#notification-container .notification').each(function() {
			if($(this).attr('data-id') == noticeID) {
				$(this).fadeOut('slow', function() {
					$(this).remove();
					return false;
				});
			}
		});
	}, 6500);
	return;
}

/** Removes a specified notification from users screen **/
function removeAlert(id) {
	$('#notification-container .notification').each(function() {
		if($(this).attr('data-id') == id) {
			$(this).fadeOut('fast', function() {
				$(this).remove();
				return false;
			});
		}
	});
	return;
}