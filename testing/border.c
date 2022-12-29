
#include <ncurses.h>

WINDOW* create_newwin(int, int, int, int);
void destroy_win(WINDOW*);

int main(int argc, char** argv) {

	initscr();
	cbreak();
	keypad(stdscr, TRUE);

	int height = 3;
	int width = 10;
	int starty = (LINES - height) / 2;
	int startx = (COLS - width) / 2;
	
	printw("Press F1 to exit");
	refresh();
	
	WINDOW* my_win = create_newwin(height, width, starty, startx);

	int ch;
	while((ch = getch()) != KEY_F(1)) {
		
		switch(ch) {
			
			case KEY_LEFT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty, --startx);
				break;
			case KEY_RIGHT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty, ++startx);
				break;
			case KEY_UP:
				destroy_win(my_win);
				my_win = create_newwin(height, width, --starty, startx);
				break;
			case KEY_DOWN:
				destroy_win(my_win);
				my_win = create_newwin(height, width, ++starty, startx);
				break;	
		}
	}
		
	endwin();
	return 0;
}

WINDOW* create_newwin(int height, int width, int starty, int startx) {
	
	WINDOW* win = newwin(height, width, starty, startx);
	box(win, 0 , 0); //0,0 means use default chars for lines
	wrefresh(win);

	return win;
}

void destroy_win(WINDOW* local_win) {
	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	//ls: left side
	//rs: right side
	//ts: top side
	//bs: bottom side
	//tl: top left
	//tr: top right
	//bl: bottom left
	//br: bottom right
	wrefresh(local_win);
	delwin(local_win);
}
