
package main

import {
	fmt;
	debug;
}




fn get_level_by_score(int score) -> (int) {
	if 0<=score and score<10 {
		fmt::println_string("level 0");
	} else if 10<=score and score<20 {
		fmt::println_string("level 1");
	} else if 20<=score and score<30 {
		fmt::println_string("level 2");
	} else if 30<=score and score<40 {
		fmt::println_string("level 3");
	} else if 40<=score and score<50 {
		fmt::println_string("level 4");
	} else if 50<=score and score<60 {
		fmt::println_string("level 5");
	} else if 60<=score and score<70 {
		fmt::println_string("level 6");
	} else if 70<=score and score<80 {
		fmt::println_string("level 7");
	} else if 80<=score and score<90 {
		fmt::println_string("level 8");
	} else if 90<=score and score<=100 {
		fmt::println_string("level 9");
	} else if score == 100 {
		fmt::println_string("level 10");
	} else {
		fmt::println_string("score invalid");
		return 0;
	}

	return score/10;
}



fn main() {

	var int score;
	var int level;
	
	for score = -9; score<=110; score+=10 {
		level = get_level_by_score(score);
	
		fmt::println_string("=====");
		fmt::println_int(score);
		fmt::println_int(level);
		fmt::println_string("=====");
	
		fmt::println_string("");
	}

}

