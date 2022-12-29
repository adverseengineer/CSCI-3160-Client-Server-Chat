

#include <ncurses.h>

static WINDOW* tools_win;
static int tools_height, tools_width, tools_y, tools_x;

static WINDOW* status_win;
static int status_height, status_width, status_y, status_x;

static WINDOW* servers_win;
static int servers_height, servers_width, servers_y, servers_x;

static WINDOW* users_win;
static int users_height, users_width, users_y, users_x;

static WINDOW* messages_win;
static int messages_height, messages_width, messages_y, messages_x;

void ui_init(void) {

	initscr();
	raw(); //don't generate signals on ctrl+whatever
	keypad(stdscr, 1); //enable f1-f12

	tools_y = 0;
	tools_x = 0;
	tools_height = 1;
	tools_width = COLS;

	status_y = LINES - 1;
	status_x = 0;
	status_height = 1;
	status_width = COLS;

	servers_y = 1;
	servers_x = 0;
	servers_height = LINES - 2;
	servers_width = COLS / 4;
	
	users_y = 1;
	users_x = COLS * 3 / 4;
	users_height = LINES - 2;
	users_width = COLS / 4;
	
	messages_y = 1;
	messages_x = COLS / 4;
	messages_height = LINES - 2;
	messages_width = COLS / 2;
	
	tools_win = newwin(tools_height, tools_width, tools_y, tools_x);
	status_win = newwin(status_height, status_width, status_y, status_x);
	
	servers_win = newwin(servers_height, servers_width, servers_y, servers_x);
	users_win = newwin(users_height, users_width, users_y, users_x);
	
	messages_win = newwin(messages_height, messages_width, messages_y, messages_x);
}

void ui_deinit(void) {

	delwin(tools_win);
	delwin(status_win);
	
	delwin(servers_win);
	delwin(users_win);
	
	delwin(messages_win);
	
	endwin();
}

void ui_draw(void) {
	
	wrefresh(tools_win);
	wrefresh(status_win);
	
	wborder(servers_win, 0, 0, 0, 0, 0, 0, 0, 0);
	wrefresh(servers_win);
	
	wborder(users_win, 0, 0, 0, 0, 0, 0, 0, 0);
	wrefresh(users_win);
	
	wborder(messages_win, 0, 0, 0, 0, 0, 0, 0, 0);
	wrefresh(messages_win);
}

void ui_refresh(void) {

	wrefresh(tools_win);
	wrefresh(status_win);
	
	wrefresh(servers_win);
	wrefresh(users_win);
	
	wrefresh(messages_win);
}

int main(int argc, char** argv) {

	ui_init();

	ui_draw();

	mvwprintw(tools_win, 0, 0, "Toolbar");
	mvwprintw(status_win, 0, 0, "Status Bar");
	
	mvwprintw(servers_win, 1, 1, "Servers");
	mvwprintw(users_win, 1, 1, "Active Users");

	mvwprintw(messages_win, 1, 1, "Message Log");

	ui_refresh();

	wgetch(messages_win);
	
	ui_deinit();
}