'use strict';

var passwordSubmitted = false;
var confirmingPassword = false;
var addingEmail = false;
var block_form_open = false;
var blocking_user = false;

$(function() {
	// hide change password button until after checkAuth
	$('#change-password-button').hide();

	if(getCurrentPage() == 'Account Settings') {
		// check if user is already authenticated for page
		checkAuth(function(res) {
			// if already authenticated, replace the change password
			// with the update password form
			if(res.status == "DX-OK") {
				if(res.message == "true") {
					confirmingPassword = true;
					showChangePasswordForm();
				} else { // show change password button
					$('#change-password-button').show();
				}
			} else if(res.status == "DX-REJECTED") { // not logged in || wrong http method
				createAlert(res.message, 'medium');
			} else { // server error, display error, leave page alone
				createAlert(res.message, 'high');
			}
		});
	} else if(getCurrentPage() == 'Blocked Users') {
		var old_color = $(document).find('#account-view-label').css('color');
		$(document).find('#account-view-label').html('Fetching blocked list........');
		$(document).find('#account-view-label').css('color', 'red');
		// Get blocked list
		getBlockedList(function(res) {
			$(document).find('#account-view-label').html('Blocked Users');
			$(document).find('#account-view-label').css('color', old_color);
			if(res.status == 'DX-OK') {
				if(res.message.length) {
					var list = res.message.split(",");
					for(var name in list) {
						generateBlockedUser(list[name]);
					}
				}
			} else if(res.status == 'DX-REJECTED') {
				createAlert(res.message, 'medium');
			} else if(res.status == 'DX-FAILED') {
				createAlert(res.message, 'high');
			}
		});
	}

	// remove secondary email -- click
	$(document).on('click', '#remove-secondary-email', function() {
		removeSecondaryEmail(function(res) {
			console.log(res);
			if(res.status == 'DX-OK') {
				window.location.href = '/account';
			} else if(res.status == 'DX-REJECTED') {
				createAlert(res.message, 'medium');
			} else if(res.status == 'DX-FAILED') {
				createAlert(res.message, 'high');
			}
		});
	});

	// submit new password form -- click
	$(document).on('click', '#change-password-form-submit-button', function() {
		
		if(!passwordSubmitted) {
			passwordSubmitted = true;
			var pwd_o = $('#new_password');
			var pwd_oa = $('#new_password_again');
			var pwd_i = $('#new_password').val();
			var pwd_ia = $('#new_password_again').val();

			// remove errors
			$('#change-password-form').find('.form-error').each(function() {
				$(this).remove();
			});

			// ensure fields not empty
			var empty = false;
			if(pwd_i.length == 0) {
				generateFormError('Cannot be empty', pwd_o);
				empty = true;
			}

			if(pwd_ia.length == 0) {
				generateFormError('Cannot be empty', pwd_oa);
				empty = true;
			}

			if(empty) {
				passwordSubmitted = false;
				return false;
			}

			
			// validate password
			if(!validPassword(pwd_i)) {
				generateFormError('Not a valid password', pwd_o);
				passwordSubmitted = false;
				return false;
			}
			
			// ensure they match
			if(pwd_ia != pwd_i) {
				generateFormError('Passwords don\'t match', pwd_o);
				generateFormError('Passwords don\'t match', pwd_oa);
				passwordSubmitted = false;
				return false;
			}

			// hide submit button
			$('#change-password-form-submit-button').hide();
			
			// send to server
			submitNewPassword(pwd_i, pwd_ia, function(res) {
				console.log(res);
				if(res.status == 'DX-OK') {
					// replace form with updated message
					var DOM =
					"<div>" +
						"<span style='font-weight:bold;font-size:.8em;font-family:GOOGLE;color:rgb(0, 200, 225);'> Password updated!</span>" +
					"</div>";
				$('#change-password-form').replaceWith(DOM);
				} else if(res.status == 'DX-REJECTED') {
					$('#change-password-form-submit-button').show();
					passwordSubmitted = false;
					generateFormError('[ &#8212; ] Error :: ' + res.message, pwd_oa);
				} else if(res.status == 'DX-FAILED') {
					createAlert(res.message, 'high');
					$('#change-password-form-submit-button').show();
					passwordSubmitted = false;
				}
			});
		}
	});

	// submit new password form -- enter key
	$(document).on('keypress', '#change-password-form', function(e) {
		if(e.which == 13) {
			if(!passwordSubmitted) {
				$(document).find('#change-password-form-submit-button').click();
			}
		}
	});

	// change password button - unlocks settings
	$(document).on('click', '#change-password-button', function() {
		// request re-entry of password
		var DOM =
			"<div id='background-blur'></div><div id='confirm-password-form'><span id='cancel-button'>X</span>" +
				"<span id='confirm-password-form-header'>Unlock settings</span>" +
				"<label>Re-enter current password</label>" +
				"<input id='confirm-current-password' type='password' placeholder='Current password...'>" +
				"<button id='submit-button'>Confirm</button>" +
			"</div>";
		$(DOM).appendTo($("#wrapper"));
		$('#confirm-password-form input').focus();
		$('#change-password-button').hide();
	});

	// submit confirm password -- click
	$(document).on('click', '#confirm-password-form #submit-button', function() {
		// remove errors
		$(document).find('#confirm-password-form .form-error').remove();
		if(!confirmingPassword) {
			confirmingPassword = true;

			var pwd = $('#confirm-password-form input').val();
			
			// validate password
			if(!validPassword(pwd)) {
				createAlert('Not a valid password', 'medium');
				confirmingPassword = false;
				return false;
			}
			
			// hide button
			$('#confirm-password-form #submit-button').hide();
			// try password
			confirmPassword(pwd, function(res) {
				$('#confirm-password-form #submit-button').show();
				if(res.status == 'DX-OK') {
					// display change password form
					showChangePasswordForm();
				} else if(res.status == 'DX-REJECTED') {
					generateFormError(res.message, $('#confirm-current-password'));
					confirmingPassword = false;
				} else { // server failed
					createAlert(res.message, 'high');
					confirmingPassword = false;
				}
			});
		}
	});

	// submit confirm password - enter key
	$(document).on('keydown', '#confirm-password-form input', function(e) {
		if(e.which == 13) {
			if(!confirmingPassword) {
				$(document).find('#confirm-password-form #submit-button').click();
			}
		}
	});

	// cancel confirm password - click X
	$(document).on('click', '#confirm-password-form #cancel-button', function() {
		$(this).parent().remove();
		$(document).find('#background-blur').remove();
		$('#change-password-button').show();
	});

	// cancel confirm password - escape key
	$(document).on('keydown', function(e) {
		if(e.which == 27) {
			$(document).find('#confirm-password-form').remove();
			$(document).find('#background-blur').remove();
			$('#change-password-button').show();
		}
	});

	// cancel confirm password - background-blur click
	$(document).on('click', '#background-blur', function() {
		if($('#confirm-password-form input').val().length == 0) {
			$(document).find('#confirm-password-form').remove();
			$(document).find('#background-blur').remove();
			$('#change-password-button').show();
		}
	});

	// Add a new email button
	$(document).on('click', '.add-new-button', function() {
		var DOM =
		"<div class='email add-new'>" +
			"<input type='text' placeholder='Enter email address' spellcheck='false'>" +
			"<button class='add-new-submit'>Add</button>" +
		"</div>";
		$(this).replaceWith(DOM);
		$(document).find('.add-new input').focus();
	});

	// on new email submit -- click
	$(document).on('click', '.add-new-submit', function() {
		if(!addingEmail) {
			addingEmail = true;

			$('.add-new input').val($('.add-new input').val().toLowerCase());
			var new_email = $('.add-new input').val(); // grab input

			// validate the email
			if(!validEmail(new_email)) {
				createAlert('Must enter a valid email address', 'medium');
				addingEmail = false;
				return false;
			}

			// query server
			addEmail(new_email, function(res) {
				console.log(res);
				if(res.status == 'DX-OK') {
					// Refresh page when good response
					window.location.href = "/account";
				} else {
					createAlert(res.message, 'medium');
					addingEmail = false;
				}
			});
		}
	});

	// on new email submit -- enter key
	$(document).on('keypress', '.add-new input', function(e) {
		if(e.which == 13) {
			if(!addingEmail) {
				$(document).find('.add-new-submit').click();
			}
		}
	});


	// select user
	$(document).on('click', '.blocked-user', function() {
		if($(this).attr('data-selected') == 'false') {
			blockedDeselect();
			// select this
			$(this).attr('data-selected', 'true');
		}
	});

	// display block user form
	$(document).on('click', '#blocked-users-controls #add-user', function() {
		if(!block_form_open) {
			showBlockUserForm();
		}
	});

	// remove block user form -- click cancel
	$(document).on('click', '#block-user-cancel', function() {
		removeBlockUserForm();
	});

	// block user -- enter key
	$(document).on('keypress', '#block-user-input', function(e) {
		if(e.which == 13) {
			if(!blocking_user) {
				$('#block-user-add').click();
			}
		}
	});

	// block user -- click
	$(document).on('click', '#block-user-add', function() {
		if(!blocking_user) {
			blocking_user = true;
			var u_field = $('#block-user-input');
			var username = $('#block-user-input').val();

			// remove errors
			$('#block-user-form').find('.form-error').each(function() {
				$(this).remove();
			});

			// ensure not empty
			if(username.length == 0) {
				generateFormError('Cannot be empty', u_field);
				blocking_user = false;
				return false;
			}

			// ensure valid username
			if(!validUsername(username)) {
				generateFormError('Must be valid username', u_field);
				blocking_user = false;
				return false;
			}

			// ensure not already blocked
			if(isBlocked(username)) {
				generateFormError('Already blocking this user', u_field);
				blocking_user = false;
				return false;
			}

			// query server
			blockUser(username, function(res) {
				console.log(res);

				// added user to blocked list
				if(res.status == 'DX-OK') {
					// place username into blocked list container
					generateBlockedUser(username);
					// remove form & background blur
					removeBlockUserForm();
				} else if(res.status == 'DX-REJECTED') {
					generateFormError(res.message, u_field);
				} else if(res.status == 'DX-FAILED') {
					removeBlockUserForm();
					createAlert(res.message, 'high');
				}
			});
			blocking_user = false;
		}
	});

	// unblock user
	$(document).on('click', '#blocked-users-controls #remove-user', function() {
		// check if a username is selected
		var someone_blocked = false;
		var username = '';
		var next_user = '';
		$('.blocked-user').each(function() {
			if(next_user == '[PRIMED]') {
				next_user = $(this).html();
			}
			if($(this).attr('data-selected') == 'true') {
				username = $(this).html();
				someone_blocked = true;
				next_user = '[PRIMED]';
			}
		});
		// if someone is selected
		if(someone_blocked) {
			// unblock user
			unblockUser(username, next_user, function(res) {
				console.log(res);
			});
		}
	});
});

function checkAuth(callback) {
	$.ajax({
		type: 'GET',
		url: '/api/settings/check_auth',
		error: function(err) {
			var res = {
				status: 'DX-FAILED',
				message: 'Server error'
			};
			if(err.status == 0) {
				res.message = 'Server is currently down';
			}
			if(err.status == 404) {
				res.message = 'Something is badly programmed';
			}
			callback(res);
		}
	}).done(function(res) {
		res = JSON.parse(res);
		callback(res);
	});
	return;
}

function confirmPassword(pwd, callback) {
	$.ajax({
		type: 'POST',
		url: '/api/settings/unlock',
		data: {
			password: pwd
		},
		error: function(err) {
			var res = {
				status: 'DX-FAILED',
				message: 'Server error'
			};
			if(err.status == 0) {
				res.message = 'Server is currently down';
			}
			if(err.status == 404) {
				res.message = 'Something is badly programmed';
			}
			callback(res);
		}
	}).done(function(res) {
		res = JSON.parse(res);
		callback(res);
	});
	return;
}

function addEmail(email, callback) {
	$.ajax({
		type: 'POST',
		url: '/api/settings/add_email',
		data: {
			email: email
		},
		error: function(err) {
			var res = {
				status: 'DX-FAILED',
				message: 'Server error'
			};
			if(err.status == 0) {
				res.message = 'Server is currently down';
			}
			if(err.status == 404) {
				res.message = 'Something is badly programmed';
			}
		}
	}).done(function(res) {
		res = JSON.parse(res);
		callback(res);
	});
	return;
}

function submitNewPassword(i, ia, callback) {
	$.ajax({
		type: 'POST',
		url: '/api/settings/change_password',
		data: {
			initial: i,
			confirmed: ia
		},
		error: function(err) {
			var res = {
				status: 'DX-FAILED',
				message: 'Server error'
			};
			if(err.status == 0) {
				res.message = 'Server is currently down';
			}
			if(err.status == 404) {
				res.message = 'Something is badly programmed';
			}
			callback(res);
		}
	}).done(function(res) {
		res = JSON.parse(res);
		callback(res);
	});
	return;
}

function getBlockedList(callback) {
	$.ajax({
		type: 'GET',
		url: '/api/settings/blocked_list',
		error: function(err) {
			var res = {
				status: 'DX-FAILED',
				message: 'Server error occurred'
			};
			if(err.status == 0) {
				res.message = 'Server is currently down';
			}
			if(err.status == 404) {
				res.message = 'Something is currently down';
			}
			callback(res);
		}
	}).done(function(res) {
		res = JSON.parse(res);
		callback(res);
	});
	return;
}

function blockedDeselect() {
	$('.blocked-user').each(function() {
		$(this).attr('data-selected', 'false');
	});
	return;
}

function showBlockUserForm() {
	var DOM =
	"<div id='background-blur'></div><div id='block-user-form'>" +
		"<label>Enter a username to block</label>" +
		"<input id='block-user-input' type='text' placeholder='Username...'>" +
		"<button id='block-user-add'>Add</button><button id='block-user-cancel'>Cancel</button>" +
	"</div>";
	$(DOM).appendTo($('#wrapper'));
	$(document).find('#block-user-input').focus();
	block_form_open = true;
	return;
}

function removeBlockUserForm() {
	$(document).find('#background-blur').remove();
	$(document).find('#block-user-form').remove();
	block_form_open = false;
	return;
}

function generateBlockedUser(username) {
	// deselect all
	blockedDeselect();
	// append username with selected true
	var HTML = "<span class='blocked-user' data-selected='true'>" + username + "</span>";
	$(HTML).prependTo($('#blocked-users-list'));
	return;
}

function showChangePasswordForm() {
	var DOM = 
	"<div id='change-password-form'>" +
		"<label>Enter new password</label>" +
		"<input id='new_password' type='password' placeholder='Enter new password...'>" +
		"<label>Confirm new password</label>" +
		"<input id='new_password_again' type='password' placeholder='Confirm new password...'>" +
		"<button id='change-password-form-submit-button'>Change Password</button>" +
	"</div>";
	$('#change-password-button').show();
	$('#confirm-password-form').remove();
	$('#background-blur').remove();
	$('#change-password-button').replaceWith(DOM);
	$(document).find('#change-password-form #new_password').focus();
	return;
}

function isBlocked(username) {
	var status = false;
	$('.blocked-user').each(function() {
		if($(this).html().toLowerCase() == username.toLowerCase()) {
			status = true;
		}
	});
	return status;
}

function removeSecondaryEmail(callback) {
	$.ajax({
		type: 'POST',
		url: '/api/settings/remove_email',
		error: function(err) {
			var res = {
				status: 'DX-FAILED',
				message: 'Server error'
			};
			if(err.status == 0) {
				res.message = 'Server is currently down';
			}
			if(err.status == 404) {
				res.message = 'Something is badly programmed';
			}
			callback(res);
		}
	}).done(function(res) {
		res = JSON.parse(res);
		callback(res);
	});
	return;
}

function blockUser(usrname, callback) {
	// query server
	$.ajax({
		type: 'POST',
		url: '/api/settings/block',
		data: {
			username: usrname
		},
		error: function(err) {
			var res = {
				status: 'DX-FAILED',
				message: 'Server error occurred'
			};
			if(err.status == 0) {
				res.message = 'Server is currently down';
			}
			if(err.status == 404) {
				res.message = 'Something is badly programmed';
			}
			callback(res);
		}
	}).done(function(res) {
		res = JSON.parse(res);
		callback(res);
	});
	return;
}

function unblockUser(usrname, next_user, callback) {
	next_user = next_user || '';
	var primed = false;
	$('.blocked-user').each(function() {
		if(primed) {
			$(this).attr('data-selected', 'true');
			primed = false;
		}
		if($(this).html().toLowerCase() == usrname.toLowerCase()) {
			// remove html
			$(this).remove();
			primed = true;

			// query server
			$.ajax({
				type: 'POST',
				url: '/api/settings/unblock',
				data: {
					username: usrname.toLowerCase() || ''
				},
				error: function(err) {
					var res = {
						status: 'DX-FAILED',
						message: 'Server error occurred'
					};
					if(err.status == 0) {
						res.message = 'Server is currently down';
					}
					if(err.status == 404) {
						res.message = 'Something is badly programmed';
					}
					callback(res);
				}
			}).done(function(res) {
				res = JSON.parse(res);
				callback(res);
			});
		}
	});
	return;
}