#ifndef ARC_HOST_HH
#define ARC_HOST_HH

#include "arc_errors.hh"

#include "host2arc_interface.hh"
#include "user_interface.hh"

namespace arc { namespace host {

class ArcHost {
    public:
        ArcHost(
            const char* const my_ip_addr,      /**< [in] IP address that this side runs from */
            const char* const my_port,         /**< [in] Port that this side runs from */
            const char* const dest_ip_addr,     /**< [in] IP address of destination */
            const char* const dest_port        /**< [in] Port of destination */
        );

        ~ArcHost();

        void Spin();

    private:
        Host2ArcInterface&  h2ai_;
        UserInterface       user_if_;

        common::shared_log_ptr_t    logger_;
        static const std::string    kLogName;
};

}} // arc::host

#endif //ARC_HOST_HH