'use strict';

$(function() {

	// Error handling on /p/new
	if(state.PAGE == 'Create Post') {
		// give focus to description
		$('#post-form-description').focus();
		if(getParam('err')) {
			// select post type
			if(getParam('type') == 'general') {
				changePostType($('#post-type-right'));
			}
			switch(getParam('err')) {
				case 'need_image':
					$('#post-form-description').val(getParam('desc'));
					createAlert('Sale post\'s need at least one valid image', 'high');
					break;
				case 'invalid_image':
					$('#post-form-description').val(getParam('desc'));
					createAlert('A selected file was not a valid image', 'high');
					break;
				case 'invalid_desc':
					$('#post-form-description').val(getParam('desc'));
					createAlert('Description must be at least 10 characters', 'high');
					break;
			}
		}
	} else if(state.PAGE == 'Home') { // Home page, fetch products
		getProductsMain(function(res) {
			$('#center-feed').html('');
			if(res.status == 'DX-OK') {
				if(res.message.length) {
					// turn into post
					for(var i = res.message.length - 1; i >= 0; --i) {
						generateProduct(res.message[i]);
					}
				} else { // empty product list
					$('#center-feed').html('<span style="display:block;text-align:center;font-family:GOOGLE;font-size:1.025em;">No products to load...</span>');
				}
			} else if(res.status == 'DX-REJECTED') {
			} else if(res.status == 'DX-FAILED') {
			}
		});
	}

	/** Open login form on menu item click **/
	$(document).on('click', '.need-login-menu-item', function() {
		toggleHeaderLoginForm();
	});

	/** Close action menus on external click **/
	$(document).on('click', function(e) {
		if(!$(e.target).is($('.post-action-menu-button'))) {
			closeAllPostActionMenus();
		} else {
			closeAllPostActionMenusExcept($(e.target));
			togglePostActionMenu($(e.target));
		}
	});

	/** show more of post description on click **/
	$(document).on('click', '.description-show-more', function() {
		var post = $(this).parents('.feed-post');
		var complete = $(post).find('.full-description').html();
		var incomplete = $(post).find('.post-description').html();
		// switcheroo
		$(post).find('.post-description').html(complete);
		$(post).find('.full-description').html(incomplete);
	});

	/** show less of post description on click **/
	$(document).on('click', '.description-show-less', function() {
		var post = $(this).parents('.feed-post');
		var complete = $(post).find('.full-description').html();
		var incomplete = $(post).find('.post-description').html();
		// switcheroo
		$(post).find('.post-description').html(complete);
		$(post).find('.full-description').html(incomplete);
	});

	/** change selected image on click **/
	$(document).on('click', '.preview', function() {
		var post = $(this).parents('.feed-post');
		var img_src = $(this).find('img').attr('src');
		// switch image in view
		$(post).find('#post-image-selected img').attr('src', img_src);
		// place border around new image
		$(post).find('.preview').each(function() {
			if($(this).hasClass('preview-selected')) {
				$(this).removeClass('preview-selected');
			}
		});
		$(this).addClass('preview-selected');
	});

	/** First post-action-menu coloring on hover **/
	$(document).on({
		mouseenter: function() {
			colorPostActionMenuItem($(this));
		},
		mouseleave: function() {
			uncolorPostActionMenuItem($(this));
		}
	}, '.post-action-menu-list li:first-child');


	/** Post description enter key **/
	$(document).on('keyup keydown', '#post-form-description', function(e) {
		/**
			Update form validity, this will enable or disable form button
		*/
		updatePostFormStatus();
	});

	/** Selection of images **/
	$(document).on('click', '.photo-handler', function() {
		$($(this).attr('data-for')).click();
	});

	/** Image removal on click **/
	$(document).on('click', '.photo-remover', function() {
		// RestoreInput
		restoreInput($(this).attr('data-for'));
		/**
			Update form validity, this will enable or disable form button
		*/
		updatePostFormStatus();
	});

	/** Switch post type **/
	$(document).on('click', '.post-type', function() {
		changePostType($(this));
		updatePostFormStatus();
	});

	/** Submit create form **/
	$(document).on('click', '#post-form-button', function() {
		// Ensure the form is valid before accepting submission
		if($('#post-form').attr('data-valid') == 'true') {
			// validate again
			updatePostFormStatus();
			// if still valid
			if($('#post-form').attr('data-valid') == 'true') {
				$('#post-form').submit();
			}
		}
	});

	/** Open create post form **/
	$(document).on('click', '#header-create-post-button', function() {
		toggleCreatePostForm();
	});

	/** Validating selection **/
	$(document).on('change', '.post-image', function() {
		var file = this.files[0]; // Get file
		var inputID = '#' + $(this).attr('id'); // input field id
		var handlerContainer = $(this).attr('data-for');

		// If user cancelled selection, restore handler ( input field is already empty )
		if(!file) {
			// Redraw handlers
			restoreHandler(handlerContainer);
			/**
				Update form validity, this will enable or disable form button
			*/
			updatePostFormStatus();
			return;
		}

		/**
			If invalid extension, restore input field & restore handler
		*/
		if(!validImageExt(getExtension(file.name))) {
			createAlert('Not a valid image extension `.' + getExtension(file.name) + '`', 'medium');
			restoreInput(inputID);
			/**
				Update form validity, this will enable or disable form button
			*/
			updatePostFormStatus();
			return;
		}

		/**
			try validating image
			all errors => restore input ( calls restore handler automatically )
		*/
		loadImage(file, function(err, result) {
			if(err) {
				switch(err) {
					case 'empty': // result == data: <= attempted empty file upload
						createAlert('Select a valid image', 'medium');
						restoreInput(inputID);
						break;
					case 'failed': // something wrong happened
						createAlert('An error occurred, try again', 'medium');
						restoreInput(inputID);
						break;
					case 'invalid_image': // Image cannot be displayed = invalid
						createAlert('Invalid file selected', 'medium');
						restoreInput(inputID);
						break;
					case 'dim_small': // Image dimensions are too small
						createAlert('Image must be at least 300x300 in dimensions', 'medium');
						restoreInput(inputID);
						break;
					case 'file_size':
						createAlert("Image size must not exceed 5 MB", 'medium');
						restoreInput(inputID);
						break;
					default:
						break;
				}
			} else {
				setHandler(handlerContainer, result);
			}
			/**
				Update form validity, this will enable or disable form button
			*/
			updatePostFormStatus();
		});

	});

	// Create comment field on click
	$(document).on('click', '#comment-field-placeholder', function() {
		var p = $(this).parent();
		$(this).parent().html(createCommentField($(this).attr('data-id')));
		$(p).find('textarea').focus();
	});

	// Remove comment field on unfocus
	$(document).on('blur', '#comment-field', function(e) {
		if(!$(this).val().length) {
			$(this).parent().parent().html(createCommentPlaceholder($(this).attr('data-id')));
		}
	});

	// Submit comment on send
	$(document).on('click', '#comment-button', function() {
		// delete old errors
		$(this).parents('#new-comment-container').find('.form-error').remove();
		var commentField = $(this).parents('#new-comment-container').find('#comment-field');
		var comment = $(commentField).val();
		// make sure the length is ok
		if(comment.length < 2 || comment.length > 700) {
			generateFormError('Comment must be 2 - 700 characters in length', $(commentField));
		}
	});

	/** Filter view when user clicks on a view type button **/
	$(document).on('click', '.view', function() {
		$(document).find('.view').each(function() {
			$(this).attr('data-selected', 'false');
		});
		$(this).attr('data-selected', 'true');
		// get view type
		var type = $(this).attr('id');
		if(type != 'all') {
			$(document).find('.feed-post').each(function() {
				if($(this).attr('data-type') != type) {
					$(this).fadeOut();
				} else {
					$(this).fadeIn();
				}
			});
		} else {
			$(document).find('.feed-post').each(function() {
				$(this).fadeIn();
			});
		}
	});

});















/*
<div class="center-col">
	<div class='feed-post'>
		<div class='post-hidden-info'>
			<span class='info-post-id'>some-post-id</span>
			<span class='info-post-timestamp'>some-timestamp</span>
			<span class='info-post-owner'>some-username</span>
			<span class='info-post-owner-id'>some-user-id</span>
		</div>
		<div class='inner-post-top'>
			<span class='post-owner'>some-username</span>
			<span class='post-timestamp'>some-timestamp</span>
			<span class='post-description'>
				This is a post description, this is for sale at low cost
				hmu for more info
			</span>
			<div class='post-image-container'>
				<img class='post-image primary-image' src='/img/landing_bg.jpg'>
			</div>
		</div>
		<div class='post-action-bar'>
			<span class='want-this'>Upvote</span>
			<div class='post-action-menu-container'>
				<img class='post-action-menu-button' src='/img/gear.png'>
				<ul class='post-action-menu-list'>
					<li>Subscribe</li>
					<li>Mark suspect</li>
				</ul>
			</div>
		</div>
	</div>
</div>
*/
function dateFromTimestamp(timestamp) {
	var measure = '';
	var js_date = new Date(timestamp);
	var ms = new Date() - js_date;
	var seconds, minutes, hours, days;
	
	seconds = Math.floor(ms / 1000);
	if(seconds > 59) {
		minutes = Math.floor(seconds / 60);
		if(minutes > 59) {
			hours = Math.floor(minutes / 60);
			if(hours > 23) {
				days = Math.floor(hours / 24);
				if(days == 1) {
					measure = 'Yesterday';
				} else {
					measure = 'default';
				}
			} else {
				if(hours == 1) {
					measure = 'hour';
				} else {
					measure = 'hours';
				}
			}
		} else {
			if(minutes == 1) {
				measure = 'minute';
			} else {
				measure = 'minutes';
			}
		}
	} else {
		if(seconds == 1) {
			measure = 'second';
		} else if(seconds == 0) {
			measure = 'Just now';
		} else {
			measure = 'seconds';
		}
	}

	switch(measure) {
		case 'second':
		case 'seconds':
			return seconds + ' ' + measure + ' ago';
			break;
		case 'minute':
		case 'minutes':
			return minutes + ' ' + measure + ' ago';
			break;
		case 'hour':
		case 'hours':
			return hours + ' ' + measure + ' ago';
			break;
		case 'day':
		case 'days':
			return days + ' ' + measure + ' ago';
			break;
		case 'Yesterday':
			var origin = new Date(timestamp);
			var time = [origin.getHours(), origin.getMinutes()];
			var suffix = (time[0] < 12) ? 'AM' : 'PM';
			time[0] = (time[0] < 12) ? time : parseInt(time) - 12;
			time[0] = time[0] || 12;
			return 'Yesterday at ' + time.join(":") + ' ' + suffix;
			break;
		default:
			var origin = new Date(timestamp);
			var date = [origin.getMonth(), origin.getDate()];
			var months = ['Jan', 'Feb', 'March', 'April', 'May', 'Jun', 'Jul', 'Aug', 'Sept', 'Oct', 'Nov', 'Dec'];
			var time = [origin.getHours(), origin.getMinutes()];
			var suffix = (time[0] < 12) ? 'AM' : 'PM';
			time[0] = (time[0] < 12) ? time : parseInt(time) - 12;
			time[0] = time[0] || 12;
			return months[date[0]] + ' ' + date[1] + ' at ' + time.join(":") + ' ' + suffix;
			break;
	}
}

function generateProduct(productObj) {
	var photoArray = [];
	if(productObj["photos"]) {
		photoArray = productObj["photos"].split(',');
	}
	var DOM =
	"<div class='feed-post' data-type='" + productObj["post_type"] + "'>" +
		"<div class='post-hidden-info'>" +
			"<span class='info-post-id'>" + productObj["id"] + "</span>" +
			"<span class='info-post-owner-id'>" + productObj["owner_id"] + "</span>" +
		"</div>" +
		"<div class='inner-post-top'>" +
			"<span class='post-owner'>" + productObj["owner_username"] + "</span>" +
			"<span class='post-timestamp'>" + dateFromTimestamp(productObj["timestamp"]) + "</span>" +
			"<span class='post-description'>";
				// if description is too long cut it short
				if(productObj["description"].length > 450) {
					DOM += escapeHTML(productObj["description"]).replace(/(?:\r\n|\r|\n)/g, '<br>').substr(0, 450);
					DOM += " <div class='description-show-more' style='display:inline-block;text-align:center;color:rgb(0, 150, 200);'>...Show more</div>";
				} else {
					DOM += escapeHTML(productObj["description"]).replace(/(?:\r\n|\r|\n)/g, '<br>');
				}
			DOM += "</span>";
			DOM += "<div class='full-description'>" + escapeHTML(productObj["description"]).replace(/(?:\r\n|\r|\n)/g, '<br>') + " <div class='description-show-less' style='display:inline-block;text-align:center;color:rgb(0, 150, 200);'>...Show less</div></div>";
			// If post has at least one image
			if(productObj["photos"].length) {
				DOM += "<div id='post-image-selected'>";
					DOM += "<img src='" + photoArray[0] + "'>"
				DOM += "</div>";
				DOM += "<div id='post-image-preview-container'>";
					if(photoArray[0]) {
						DOM += "<div class='preview-selected preview'>";
							DOM += "<img src='" + photoArray[0] + "' alt='another image'>";
						DOM += "</div>";
					}
					if(photoArray[1]) {
						DOM += "<div class='preview'>";
							DOM += "<img src='" + photoArray[1] + "' alt='another image'>";
						DOM += "</div>";
					}
					if(photoArray[2]) {
						DOM += "<div class='preview'>";
							DOM += "<img src='" + photoArray[2] + "' alt='another image'>";
						DOM += "</div>";
					}
					if(photoArray[3]) {
						DOM += "<div class='preview'>";
							DOM += "<img src='" + photoArray[3] + "' alt='another image'>";
						DOM += "</div>";
					}
				DOM += "</div>";
			}
		DOM += "</div>" +
		"<div class='post-action-bar'>" +
			"<span class='upvotes'>" + productObj["upvotes"] + "</span>" +
			"<span class='upvote-button'><img src='/img/thumb_up.png' alt='upvote button'></span>" +
			"<span class='downvote-button'><img src='/img/thumb_down.png' alt='downvote button'></span>" +
			"<span class='downvotes'>" + productObj["downvotes"] + "</span>";
			if(state.LOGGED_IN) {
				DOM +=
				"<div class='post-action-menu-container'>" +
					"<img class='post-action-menu-button' src='/img/gear.png'>" +
					"<ul class='post-action-menu-list' data-state='closed'>" +
						"<li>Subscribe</li>" +
						"<li>Report</li>" +
					"</ul>" +
				"</div>";
			} else {
				DOM +=
				"<div class='post-action-menu-container'>" +
					"<img class='post-action-menu-button' src='/img/gear.png'>" +
					"<ul class='post-action-menu-list' data-state='closed'>" +
						"<li class='need-login-menu-item'>Need login</li>" +
					"</ul>" +
				"</div>";
			}
		DOM += "</div>";
		if(state.LOGGED_IN) {
			DOM +=
			"<div id='comment-section-container'>" +
				createCommentPlaceholder(productObj["id"]) +
			"</div>";
		} else {
			DOM +=
			"<div id='comment-section-container-fake'>" +
			"</div>";
		}
	DOM += "</div>";
	$(DOM).prependTo($('#center-feed'));
	return;
}

function createCommentPlaceholder(product_id) {
	return "<div data-id='" + product_id + "' id='comment-field-placeholder'>Enter a comment...</div>";
}

function createCommentField(product_id) {
	return "<div id='new-comment-container'>" +
				"<textarea data-for='" + product_id + "' placeholder='Enter a comment...' id='comment-field'></textarea><br>" +
				"<button id='comment-button'>Post</button>" +
			"</div>";
}


function escapeHTML(str) {
	var div = document.createElement('div');
    div.appendChild(document.createTextNode(str));
    return div.innerHTML;
}


function changePostType(button) {
	// Deselect all post-types
	$('.post-type').each(function() {$(this).attr('data-selected', 'false');});
	// change post-form type
	$('#post-form').attr('data-type', $(button).attr('data-value'));
	// fill hidden input with new type
	$('#hidden-post-type').attr('value', $(button).attr('data-value'));
	// select button
	$(button).attr('data-selected', 'true');
	// change label
	if($('#post-form').attr('data-type') == 'sale') {
		$('#post-image-label').html('Sales <span style="color:rgb(190, 0, 0);">must</span> have at least 1 image');
	}
	if($('#post-form').attr('data-type') == 'general') {
		$('#post-image-label').html('Any images?');
	}
	return;
}



/**
	Update form status,
	enabling or disabling form post button
	
	ensure description has at least 5 characters

	If post type == sale {
		check if at least 1 image was selected
	}
*/
function updatePostFormStatus() {
	var validForm = true;
	
	// de-activate post button
	if(!validDescription($('#post-form-description').val())) {
		$('#post-form').attr('data-valid', 'false');
		return;
	}

	// set hidden field
	$('#posttype').val($('#post-form').attr('data-type'));

	switch($('#post-form').attr('data-type')) {
		case 'sale':
			// ensure at least 1 file is selected
			var count = 0;
			$('.post-image').each(function() {
				if($(this)[0].files.length) {
					count++;
				}
			});
			if(!count) {
				$('#post-form').attr('data-valid', 'false');
				return;
			}
			break;
		case 'general':
			break;
		default:
			break;
	}
	$('#post-form').attr('data-valid', 'true');
	return;
}

function validDescription(string) {
	return (string.length >= 4 && string.length <= 2500) ? true : false;
}

/**
	set active='true', update child photo-handler src
	Call drawHandlers
*/

function setHandler(handlerContainerID, dataURL) {
	$(handlerContainerID).find('.photo-handler').attr('src', dataURL);
	$(handlerContainerID).attr('data-active', 'true');
	drawHandlers();
	return;
}

/**
	Loads image and validates contents
*/
function loadImage(file, callback) {
	var fileReader = new FileReader();

	fileReader.onerror = function() {
		callback('failed', null);
		return;
	};

	fileReader.onloadend = function() {

		// is empty ?
		if(fileReader.result == 'data:') {
			callback('empty', null);
			return;
		} else {

			// test file size in MB ( cannot exceed 5 )
			if(bytesToMB(file.size) > 4.95) {
				callback("file_size", null);
				return;
			}

			// test image contents
			var image = new Image();

			image.onerror = function() {
				callback('invalid_image', null);
				return;
			};

			image.onload = function() {
				// Must have 
				if(image.width < 300 || image.height < 300) {
					callback('dim_small', null);
					return;
				}
				callback(null, fileReader.result);
			};

			image.src = fileReader.result;
		}

		return;
	};

	fileReader.readAsDataURL(file); // attempt reading

	return;
}

function bytesToMB(bytes) {
	return parseFloat(Number((bytes / 1024) / 1024).toFixed(2));
}

/**
	removes and restores input field
	calls restoreHandler()
*/
function restoreInput(inputID) {
	var inputHTML = $(inputID)[0].outerHTML; // copy
	restoreHandler($(inputID).attr('data-for')); // restore handler
	$(inputID).remove(); // remove
	$(inputHTML).insertAfter($('#post-form-description')); // restore input
	return;
}

/**
	Complete handler reset

	Set active='false'
	Set visible='false'
	Set src='/img/cross.png'
	Call drawHandlers()
*/
function restoreHandler(handlerContainerID) {
	$(handlerContainerID).attr('data-active', 'false'); // data-active 
	$(handlerContainerID).attr('data-visible', 'false'); // data-visible 
	$(handlerContainerID).find('.photo-handler').attr('src', '/img/cross.png'); // src
	drawHandlers();
}

/**
	First sets all containers data-visible='false'
	Displays active handlers and 1 inactive handler for use
	Removes and appends handler to preview-container
*/
function drawHandlers() {
	var newHandlerReady = false;
	// Hide
	$('.handler-container').each(function() {
		$(this).attr('data-visible', 'false');
	});
	// Show all with data-active='true'
	$('.handler-container').each(function() {
		if($(this).attr('data-active') == 'true') {
			$(this).attr('data-visible', 'true');
		}
	});
	// Show 1 inactive handler
	$('.handler-container').each(function() {
		if($(this).attr('data-active') == 'false' && !newHandlerReady) {
			$(this).attr('data-visible', 'true');
			newHandlerReady = true;
			var outerHTML = $(this)[0].outerHTML;
			$(this).remove();
			$(outerHTML).appendTo($('#preview-container'));
			return false;
		}
	});
	return;
}

/** Returns the extension as a string **/
function getExtension(filename) {
	var parts = filename.split('.');
	return parts[parts.length - 1].toLowerCase();
}

/** Determines if the extension is a valid image type **/
function validImageExt(filename) {
	var parts = filename.split('.');
	var ext = parts[parts.length - 1].toLowerCase();
	return (ext == 'jpg' || ext == 'png'
			|| ext == 'bmp' || ext == 'jpeg'
			|| ext == 'gif' || ext == 'tiff') ? true : false;
}















/**** INCOMPLETE ****/








/** Closes all post action menus except the specified one **/
function closeAllPostActionMenusExcept(button) {
	$('.post-action-menu-button').each(function() {
		if($(this).parents('.feed-post').find('.info-post-id').html() != $(button).parents('.feed-post').find('.info-post-id').html()) {
			closePostActionMenu($(this));
		}
	});
	return;
}

/** Iterates all posts and closes open action menus **/
function closeAllPostActionMenus() {
	$('.post-action-menu-list').each(function() {
		if($(this).attr('data-state') == 'opened') {
			closePostActionMenu($(this).parent().find('.post-action-menu-list'));
		}
	});
	return;
}

/** Opens/Closes post action menu **/
function togglePostActionMenu(button) {
	if($(button).parents('.feed-post').find('.post-action-menu-list').attr('data-state') == 'opened') {
		closePostActionMenu(button);
	} else if($(button).parents('.feed-post').find('.post-action-menu-list').attr('data-state') == 'closed') {
		openPostActionMenu(button);
	}
	return;
}

/** Opens a specified post action menu **/
function openPostActionMenu(button) {
	$(button).parents('.feed-post').find('.post-action-menu-list').attr('data-state', 'opened');
	$(button).parents('.feed-post').find('.post-action-menu-list').show();
	return;
}

/** Closes a specified post action menu **/
function closePostActionMenu(button) {
	$(button).parents('.feed-post').find('.post-action-menu-list').attr('data-state', 'closed');
	$(button).parents('.feed-post').find('.post-action-menu-list').hide();
	return;
}

/** Used to color the speech bubble end of a post action menu **/
function colorPostActionMenuItem(item) {
	if(!$(item).hasClass('hover')){
		$(item).addClass('hover');
	}
	return;
}

/** Used to return speech bubble end of a post action menu to default color **/
function uncolorPostActionMenuItem(item) {
	if($(item).hasClass('hover')) {
		$(item).removeClass('hover');
	}
	return;
}

function getProductsMain(callback) {
	$.ajax({
		type: 'GET',
		url: '/api/products',
		error: function(res) {
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