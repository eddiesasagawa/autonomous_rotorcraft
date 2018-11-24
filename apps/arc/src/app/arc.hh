#ifndef ARC_ARC_HH
#define ARC_ARC_HH

#include "arc2host_interface.hh"

#include "spdlog.h"

namespace arc { namespace arc {

class Arc {
    public:
        /**
         * Constructor
         */
        Arc(
            const char* const my_ip_addr,      /**< [in] IP address that this side runs from */
            const char* const my_port,         /**< [in] Port that this side runs from */
            const char* const dest_ip_addr,     /**< [in] IP address of destination */
            const char* const dest_port        /**< [in] Port of destination */
        );

        /**
         * Destructor
         */
        ~Arc();

        /**
         * Main loop that runs everything
         */
        void Spin();

    private:
        Arc2HostInterface& a2hi_;
        std::shared_ptr<spdlog::logger> logger_;
};

}} // arc::arc

#endif //ARC_ARC_HH