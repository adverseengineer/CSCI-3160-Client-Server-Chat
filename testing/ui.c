

#include <ncurses.h>

void ui_init(void) {

	initscr();
	raw(); //don't generate signals on ctrl+whatever
	keypad(stdscr, 1); //enable f1-f12
	//noecho();
}

//^X: shutdown: disconnect and terminate program
//^C: disconnect: disconnect from server

//clients will have a local file containing a list of server ips to connect to
//similar to minecraft's list of servers that you enter and save yourself


void ui_deinit(void) {
	
	endwin();
}

int main(int argc, char** argv) {

	ui_init();
	
	while (1) {
		getch();
	}
	
	ui_deinit();
}