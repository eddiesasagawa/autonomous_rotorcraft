#include "user_interface.hh"

namespace arc { namespace host {

UserInterface::UserInterface() 
:   callback_map_(), 
    original_settings_(),
    new_settings_()
{
    /* initialize terminal io settings */
    tcgetattr(0, &original_settings_);  // save original settings
    new_settings_ = original_settings_; // initialize new settings with original

    new_settings_.c_lflag &= ~ICANON; // disable buffered i/o
    new_settings_.c_lflag |= ECHO; // set echo mode, clear the bit to unset

    tcsetattr(0, TCSANOW, &new_settings_); // apply new settings

}

UserInterface::~UserInterface() {
    tcsetattr(0, TCSANOW, &original_settings_); // restore terminal settings
}

void UserInterface::ProcessInput(
    bool* const quit_now      /**< [out] indicate whether a quit character has been processed */
) {
    char ch = 0;

    while ((ch = getchar()) != EOF) {
        if ('x' == ch) {
            *quit_now = true;
            break;
        } else {
            auto callback_it = callback_map_.find(ch);
            if (callback_map_.end() != callback_it) {
                /* valid key found, use it */
                callback_it->second();
            }
        }
    }
}

}} // arc::host