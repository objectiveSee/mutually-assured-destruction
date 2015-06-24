var sum = function(array) {
	var s = 0;
	for ( var t= 0; t < array.length;t++ ) {
		s += array[t];
	}
	return s;
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

// var OPENING_ON = 20;
// var OPENING_OFF = 35;
// var OPENING_LAST_ON = 80;
// var OPENING = [OPENING_ON,OPENING_OFF,OPENING_ON,OPENING_OFF,OPENING_ON,OPENING_OFF,OPENING_LAST_ON];

// OPENING_ON = 15;
// OPENING_OFF = 20;
// OPENING_LAST_ON = 40;
// var OPENING_QUICKER = [OPENING_ON,OPENING_OFF,OPENING_ON,OPENING_OFF,OPENING_ON,OPENING_OFF,OPENING_LAST_ON];

// var left = [100,100,100,100];
// var right = [100,100,100,100];

var LEFT_ON = false;
var RIGHT_ON = false;

// 30 sec version
var left = [97,6,13,6,13,6,13,175,47,7,15,7,15,7,15,285,236,6,12,6,12,6,12,99,129,6,12,6,12,6,12,86,0,6,11,6,11,6,11,60,9,6,11,6,11,6,11,6,11,60,9,6,11,6,11,6,11,6,11,30,42,32,44,37];
var right = [97,6,13,6,13,6,13,175,47,7,15,7,15,7,15,285,96,6,12,6,12,6,12,228,0,6,12,6,12,6,12,132,9,155,0,6,11,6,11,6,11,86,0,6,11,6,11,6,11,6,11,6,11,6,11,6,11,30,42,32,44,37];

// 60 seconds
// var left = [97,6,13,6,13,6,13,175,47,7,15,7,15,7,15,285,236,6,12,6,12,6,12,99,129,6,12,6,12,6,12,86,0,6,11,6,11,6,11,60,9,6,11,6,11,6,11,6,11,60,9,6,11,6,11,6,11,6,11,30,42,32,44,37,206,7,14,7,14,7,14,308,38,6,11,6,11,6,11,197,9,6,11,6,11,6,11,6,11,197,9,6,11,6,11,6,11,6,11,59,8,6,11,6,11,6,11,6,11,59,8,6,11,6,11,6,11,6,11,17,0,6,11,6,11,6,11,17,0,6,11,6,11,6,11,17,0,6,11,6,11,6,11,17,0,6,11,6,11,6,11,17,0,6,11,6,11,6,11,17,0,6,11,6,11,6,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,28,107,36,344,37,0,37,0,37,0,37,0,37,0,37,0,37,0,37,294,37,0,37,0,37,0,37,0,37,0,37,0,37,0,37,0];
// var right = [97,6,13,6,13,6,13,175,47,7,15,7,15,7,15,285,96,6,12,6,12,6,12,228,0,6,12,6,12,6,12,132,9,155,0,6,11,6,11,6,11,86,0,6,11,6,11,6,11,6,11,6,11,6,11,6,11,30,42,32,44,37,206,7,14,7,14,7,14,308,106,6,11,6,11,6,11,129,9,29,57,6,11,6,11,6,11,129,9,6,11,6,11,6,11,6,11,84,0,6,11,6,11,6,11,84,0,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,6,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,5,11,28,107,36,344,74,0,74,0,74,0,74,0,74,0,74,0,74,0,74,0,74,0,74,0,74,0,74,0];

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

// unsigned char LRM_A[5] = {0,80,40,80,STOP};
var dumpToC = function(variable_name, array) {
	var length = array.length + 1;

	var string = "unsigned char "+variable_name+"["+length+"] = {";

	for ( var i = 0; i < array.length; i++ ) {
		string += array[i]+",";
	}
	string += "STOP};";
	console.log(string);
}

var consumeEvent = function(array, isLeft) {

	if ( isLeft ) {
		LEFT_ON = !LEFT_ON;
	} else {
		RIGHT_ON = !RIGHT_ON;
	}
	printState();
	if ( array.length > 0 ) {
		consume(array, isLeft);
	} else {
		if ( isLeft ) {
			LEFT_ON = false;
		} else {
			RIGHT_ON = false;
		}
		printState();
		if ( left.length == 0 && right.length == 0 ) {
			process.stdout.write("\nSee ya!\n");
			process.exit(1);
		}
	}
}

var consume = function(array, isLeft) {

	var time = array.shift();
	setTimeout(function() {
		consumeEvent(array, isLeft);
	}, time*10);
};

var printState = function() {
	process.stdout.clearLine();  // clear current text
	process.stdout.cursorTo(0);  // move cursor to beginning of line
	var L = LEFT_ON ? "▒▒▒▒▒▒▒▒" : "        ";
	var R = RIGHT_ON ? "▒▒▒▒▒▒▒▒▒" : "        ";
	process.stdout.write(L+' '+R);
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

dumpToC("BEETHOVEN_LEFT", left);
dumpToC("BEETHOVEN_RIGHT", right);
console.log("\n");

consume(left, true);
consume(right, false);