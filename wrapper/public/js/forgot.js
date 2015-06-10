'use strict';


$(function() {
	// uncheck all the recheck this
	$('.radio-button').on('click', function() {
		$('.radio-button').each(function() {
			$(this).removeAttr('checked');
		});
		$(this)[0].checked = true
	});

	// prevent enterkey submit
	$('#forgot-form').on('keypress', function(e) {
		if(e.which == 13) {
			e.preventDefault();
			$('#forgot-button').click();
		}
	});

	// validate as username or email
	$('#forgot-button').on('click', function() {
		var type = "";
		// ensure a radio button was checked
		$('.radio-button').each(function() {
			if($(this)[0].checked) {
				type = $(this)[0].value;
			}
		});

		if(!type) {
			createAlert('Must select one of the options...<br>Did you forget your username?<br>Did you forget your password?', 'medium');
			return false;
		}

		// if the user forgot their username, validate as email
		if(type == 'username') {
			// ensure valid email was given
			if(!validEmail($('#id').val())) {
				createAlert('Invalid email', 'medium');
				return false;
			}
		} else if(type == 'password') {
			if(!validUsername($('#id').val())) {
				if(!validEmail($('#id').val())) {
					createAlert('Invalid username or email', 'medium');
					return false;
				}
			}
		}

		// submit form
		$('#forgot-form').submit();
	});
});