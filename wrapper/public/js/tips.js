'use strict';

$(function() {
	var tip_obj = $('#tips-container textarea');
	// on submit
	// server server
	$(document).on('click', '#tips-container button', function() {
		// remove all errors
		$('#tips-container').find('.form-error').each(function() {
			$(this).remove();
		});
		// ensure tip is not empty
		if(tip_obj.val().length >= 10) {
			var text = tip_obj.val();
			submitTip(text, function(res) {
				console.log(res);
				if(res.status == 'DX-OK') {
					$('#tips-header').addClass('success');
					$('#tips-header').html('Thanks, your tip has been submitted!');
					$('#tips-container').html('');
				} else if(res.status == 'DX-REJECTED') {
					generateFormError(res.message, tip_obj);
				} else if(res.status == 'DX-FAILED') {
					createAlert(res.message, 'high');
				}
			});
		} else {
			generateFormError('Tip must be at least 10 characters long', tip_obj);
		}
	});
});

function submitTip(text, callback) {
	$.ajax({
		type: 'POST',
		url: '/api/tips/process',
		data: {
			tip: text
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