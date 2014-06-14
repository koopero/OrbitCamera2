
for ( var day = 7; day <= 8; day ++ ) {
	for ( var hour = 9; hour <= 18; hour ++ ) {
		var date = new Date( 2014, 5, day, hour );
		console.log ( date, date.getTime() );
	}
}