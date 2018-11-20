#ifndef HOST_USER_INTERFACE_HH
#define HOST_USER_INTERFACE_HH

#include <unordered_map>
#include <ncurses.h>

#include <functional>

namespace arc { namespace host {

class UserInterface {
    public:
        UserInterface();
        ~UserInterface();

        /**
         * Processes all input characters in the input buffer
         */
        void ProcessInput(
            bool* const quit_now      /**< [out] indicate whether a quit character has been processed */
        );

        /**
         * Registers a callback on the particular key to trigger during ProcessInput
         * For now, only one function is supported per unique key
         */
        inline void AddCallback(
            const char trigger_key, /**< [in] char to trigger callback on */
            std::function<void()> callback  /**< [in] function pointer to callback */
        ) { callback_map_[trigger_key] = callback; }

    private:
        /** map of key presses to callbacks */
        std::unordered_map<char, std::function<void()>> callback_map_;

        SCREEN* curses_term_;
};

}} // arc::host

#endif //HOST_USER_INTERFACE_HH