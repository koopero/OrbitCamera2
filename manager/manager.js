function deletePicture ( path ) {
	console.log( this, path );
}

$(document).ready( function () {
	$('.picture').click( function () {
		var $t = $(this);
		var path = $t.data('path');
		console.log( path );
		$t.remove();

		$.ajax ( path, {
			type: 'DELETE'
		});
	});
});