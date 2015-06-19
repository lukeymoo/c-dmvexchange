'use strict';

$(function() {
	// if errors
	if(getParam('err')) {
		switch(getParam('err')) {
			case 'invalid_pwd':
				createAlert('Not a valid password', 'medium');
				break;
			case 'non_match':
				createAlert('Passwords did not match', 'medium');
				break;
			case 'invalid_token':
				createAlert('Invalid password reset token specified', 'medium');
				break;
			case 'same_password':
				createAlert('That\'s already your password!', 'medium');
				break;
			default:
				console.log('Unknown error please report to DMV Exchange => ' + getParam('err'));
				break;
		}
	}

	// set hidden token field to getParam
	$('#token').val(encodeURIComponent(getParam('token')));

	// validate password on submit
	$('#password-reset-form').on('keypress', function(e) {
		if(e.which == 13) {
			e.preventDefault();
			if($('#reset-button').is(':visible')) {
				$('#reset-button').click();
			}
		}
	});

	$('#reset-button').on('click', function() {
		// validate password
		if(!validPassword($('#np').val())) {
			createAlert('Invalid password', 'medium');
			return false;
		}
		// ensure they're the same
		if($('#np').val() != $('#npa').val()) {
			createAlert('Passwords don\'t match', 'medium');
			return false;
		}

		// hide button
		$('#reset-button').hide();

		// submit form
		$('#password-reset-form').submit();
	});
});