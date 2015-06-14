'use strict';

$(function() {
	// Add a new email
	$('.add-new-button').on('click', function() {
		var DOM =
		"<div class='email add-new'>" +
			"<input type='text' placeholder='Enter email address' spellcheck='false'>" +
			"<button class='add-new-submit'>Add</button>" +
		"</div>";
		$(this).replaceWith(DOM);
	});

	// on new email submit
	$(document).on('click', '.add-new-submit', function() {
		$('.add-new input').val($('.add-new input').val().toLowerCase());
		var new_email = $('.add-new input').val(); // grab input
		// validate the email
		if(!validEmail(new_email)) {
			createAlert('Must enter a valid email address', 'medium');
			return false;
		}
		// submit
		addEmail(new_email, function(res) {
			console.log(res);
			if(res.status == 'DX-OK') {

			} else {
				createAlert(res.message, 'medium');
			}
		});
	});
});

function addEmail(email, callback) {
	$.ajax({
		type: 'POST',
		url: '/api/add_email',
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