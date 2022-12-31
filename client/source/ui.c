//Nick Sells, 2022
//CSCI 3160 Final Project

#include "ui.h"
#include <ncurses.h>
#include <assert.h>

static WINDOW* tools_win;
static int tools_height, tools_width, tools_y, tools_x;

static WINDOW* status_win;
static int status_height, status_width, status_y, status_x;

static WINDOW* servers_win;
static int servers_height, servers_width, servers_y, servers_x;

static WINDOW* messages_win;
static int messages_height, messages_width, messages_y, messages_x;

static WINDOW* users_win;
static int users_height, users_width, users_y, users_x;

static int divl, divr;
#define PROPL 0.2
#define PROPR 0.2

//TODO: handle the resize
//NOTE: if getch is waiting for input, window resizing returns KEY_RESIZE
//you can use this if the SIGWINCH handler doesn't work as expected

//calculates the dimensions for each window
//NOTE: must only be called after initscr(), as it depends on LINES and COLS
void ui_calculate(void) {

	//calculate the screen position of the division between windows
	divl = (int) (COLS * PROPL);
	divr = (int) ((COLS - 1) * (1 - PROPR));
	
	//layout logic hinges on this being true
	assert(divl < divr);
	
	//toolbar window dimensions
	tools_y = 0;
	tools_x = 0;
	tools_height = 1;
	tools_width = COLS;

	//status bar window dimensions
	status_y = LINES - 1;
	status_x = 0;
	status_height = 1;
	status_width = COLS;

	//servers window dimensions
	servers_y = 2;
	servers_x = 1;
	servers_height = LINES - 4;
	servers_width = divl - 1;
	
	//messages window dimensions
	messages_y = 2;
	messages_x = divl + 1;
	messages_height = LINES - 4;
	messages_width = (divr - divl) - 1;
	
	//users window dimensions
	users_y = 2;
	users_x = divr + 1;
	users_height = LINES - 4;
	users_width = COLS - divr - 2;
}

//draws our layout between the windows
void ui_drawborders(void) {
	
	//outer lines
	mvwhline(stdscr, 1, 1, ACS_HLINE, COLS - 2);
	mvwhline(stdscr, LINES - 2, 1, ACS_HLINE, COLS - 2);
	mvwvline(stdscr, 2, 0, ACS_VLINE, LINES - 4);
	mvwvline(stdscr, 2, COLS - 1, ACS_VLINE, LINES - 4);
	
	//inner dividers
	mvwvline(stdscr, 2, divl, ACS_VLINE, LINES - 4);
	mvwvline(stdscr, 2, divr, ACS_VLINE, LINES - 4);
	
	//corners
	mvwaddch(stdscr, 1, 0, ACS_ULCORNER);
	mvwaddch(stdscr, 1, COLS - 1, ACS_URCORNER);
	mvwaddch(stdscr, LINES - 2, 0, ACS_LLCORNER);
	mvwaddch(stdscr, LINES - 2, COLS - 1, ACS_LRCORNER);
	
	//inner tees
	mvwaddch(stdscr, 1, divl, ACS_TTEE);
	mvwaddch(stdscr, 1, divr, ACS_TTEE);
	mvwaddch(stdscr, LINES - 2, divl, ACS_BTEE);
	mvwaddch(stdscr, LINES - 2, divr, ACS_BTEE);
}

//refreshes all windows, including stdscr
void ui_refresh(void) {

	//NOTE: we do several calls to wnoutrefresh and then one call to doupdate,
	//this is because wrefresh calls wnoutrefresh and doupdate, so we save cpu
	//time by doing it this way. it is analogous to batching in openGL
	//NOTE: don't forget to refresh stdscr too, that's where our decoration is
	wnoutrefresh(stdscr);
	wnoutrefresh(tools_win);
	wnoutrefresh(status_win);
	wnoutrefresh(servers_win);
	wnoutrefresh(messages_win);
	wnoutrefresh(users_win);
	
	doupdate();
}

void ui_initcolor(void) {
	
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_BLACK, COLOR_WHITE);
}

//initializes curses, inits our windows, and draws our layout
//NOTE: call only once at the start of the program
void ui_init(void) {

	initscr();
	raw(); //don't generate signals on ctrl+whatever
	keypad(stdscr, 1); //enable f1-f12
	
	ui_initcolor();
	ui_calculate();
	
	tools_win = newwin(tools_height, tools_width, tools_y, tools_x);
	status_win = newwin(status_height, status_width, status_y, status_x);
	servers_win = newwin(servers_height, servers_width, servers_y, servers_x);
	messages_win = newwin(messages_height, messages_width, messages_y, messages_x);
	users_win = newwin(users_height, users_width, users_y, users_x);
	
	ui_drawborders();
	ui_refresh();
}

//frees our windows and exits curses mode
//NOTE: call only once at the end of the program
void ui_deinit(void) {

	delwin(tools_win);
	delwin(status_win);
	delwin(servers_win);
	delwin(messages_win);
	delwin(users_win);
	
	endwin();
}

void ui_toolbar_init(void) {
	wmove(tools_win, 0, 0);
	attron(COLOR_PAIR(2));
	waddstr(tools_win, "^X");
	attroff(COLOR_PAIR(2));
	waddstr(tools_win, " Exit");
	wrefresh(tools_win);
}

//overwrites the text shown in the status bar
void ui_status_update(char* str) {

	mvwaddstr(status_win, 0, 0, str);
	wclrtoeol(status_win);
	wrefresh(status_win);
}

//renamed to avoid conflict with the real main
int _main(int argc, char** argv) {

	ui_init();

	mvwaddstr(tools_win, 0, 0, "Toolbar");
	mvwaddstr(status_win, 0, 0, "Status Bar");
	mvwaddstr(servers_win, 0, 0, "Servers");
	mvwaddstr(messages_win, 0, 0, "Message Log");
	mvwaddstr(users_win, 0, 0, "Active Users");
	
	ui_refresh();

	wgetch(messages_win);

	ui_toolbar_init();
	ui_status_update("Suckmepls");
	wgetch(users_win);
	
	ui_deinit();
}
