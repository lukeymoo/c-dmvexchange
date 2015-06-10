'use strict';

$(function() {
	// validate password on submit
	$('#password-reset-form').on('keypress', function(e) {
		if(e.which == 13) {
			e.preventDefault();
			$('#reset-button').click();
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
		
		// submit form
		$('#password-reset-form').submit();
	});
});