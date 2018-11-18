#include "user_interface.hh"

namespace arc { namespace host {

UserInterface::UserInterface() : callback_map_() {
    /* Set up ncurses */
    initscr();  // initialize
    cbreak();   // one char at a time
    nodelay(stdscr, true); // don't block on getch
    keypad(stdscr, true); // capture special keys
}

UserInterface::~UserInterface() {
    endwin(); // restore terminal settings
}

void UserInterface::ProcessInput(
    bool* const quit_now      /**< [out] indicate whether a quit character has been processed */
) {
    char ch = 0;

    while ((ch = getch()) != ERR) {
        if ('x' == ch) {
            *quit_now = true;
        } else {
            auto callback = callback_map_.find(ch);
        }
    }
}

}} // arc::host